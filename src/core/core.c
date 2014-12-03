#include "kapusha/core.h"

/* \todo do not depend on libc */
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

/* \todo libc-based malloc should be separate compile-time decision */
static void *default_alloc(KPallocator_p allocator, KPsize size) {
  KP_UNUSED(allocator);
  return malloc(size);
}

static void default_free(KPallocator_p allocator, void *mem) {
  KP_UNUSED(allocator);
  free(mem);
}

static KPallocator_t kp__allocator_default = { default_alloc, default_free };
KPallocator_p kp_allocator_default = &kp__allocator_default;

void *kpNew(KPallocator_p allocator, KPsize size) {
  KP_THIS(KPobject_header_t, kpAllocatorAlloc(allocator, size));
  this->allocator = allocator;
  this->refcount = 1;
  this->dtor = 0;
  return this;
}

void kpRelease(void *obj) {
  if (!obj) return;
  KP_THIS(KPobject_header_t, obj);
  if (kpS32AtomicDec(&this->refcount) > 0) return;
  if (this->dtor) this->dtor(this);
  kpAllocatorFree(this->allocator, this);
}

int kpVsnprintf(char *buffer, KPsize size, const char *format, va_list argp) {
  /* \todo implement libc-independent snprintf */
  return vsnprintf(buffer, size, format, argp);
}

int kpSnprintf(char *buffer, KPsize size, const char *format, ...) {
  va_list argp;
  va_start(argp, format);
  int ret = kpVsnprintf(buffer, size, format, argp);
  va_end(argp);
  return ret;
}

static inline KPsize kpStrlen(const char *str) { return strlen(str); }

void kpLog(const char *prefix, const char *format, ...) {
  /* \todo multiline logs: per-line prefixes and limits */
  const int L = 1024;
  const KPsize PL = kpStrlen(prefix);
  char buffer[L];
  va_list argp;

  KP_ASSERT(PL + 6 < L);

  kpMemcpy(buffer+1, prefix, PL);
  buffer[0] = '[';
  buffer[PL+1] = ']';
  buffer[PL+2] = ' ';

  va_start(argp, format);
  KPsize len = kpVsnprintf(buffer + PL + 3, L - PL - 6, format, argp) + PL + 3;
  va_end(argp);
  if (len == L - 3) {
    buffer[L-4] = buffer[L-3] = buffer[L-2] = '.';
    buffer[L-1] = 0;
  }
  kp__SysLogOutput(buffer);
}

/******************************************************************************/
/* Cotainers */

KPbuffer_o kpBufferCreate(KPsize size, const void *source) {
  KPbuffer_o this = kpNew(kp_allocator_default, sizeof(KPbuffer_t) + size);
  this->O.dtor = 0;
  this->data = this + 1;
  this->size = size;
  if (source != 0) kpMemcpy(this->data, source, size);
  return this;
}

/******************************************************************************/
/* String */

KPstring_o kpStringCreate(const char *string) {
  return (KPstring_o)kpBufferCreate(kpStrlen(string)+1, string);
}

/******************************************************************************/
/* Surface */

static KPu32 KP__bytes_per_pixel[KPSurfaceFormat_Max] = {
  1 /*KPSurfaceFormatU8R*/,
  2 /*KPSurfaceFormatU8RG*/,
  3 /*KPSurfaceFormatU8RGB*/,
  4 /*KPSurfaceFormatU8RGBA*/,
  2 /*KPSurfaceFormatR5G6B5*/,
  16 /*KPSurfaceFormatF32RGBA*/
};

KPsurface_o kpSurfaceCreate(KPu32 width, KPu32 height, KPSurfaceFormat fmt) {
  KP_ASSERT(fmt >= KPSurfaceFormatU8R);
  KP_ASSERT(fmt < KPSurfaceFormat_Max);

  const KPu32 bytes_per_pixel = KP__bytes_per_pixel[fmt];

  /* 4-byte scanline alignment */
  const KPu32 stride = (((bytes_per_pixel * width) + 3) / 4) * 4;

  /* 16-byte start address alignment */
  const KPsize header_size = ((sizeof(KPsurface_t) + 15) / 16) * 16;
  const KPu32 total_size = header_size + stride * height;

  KPsurface_t *this = kpNew(kp_allocator_default, total_size);
  this->width = width;
  this->height = height;
  this->format = fmt;
  this->bytes_per_pixel = bytes_per_pixel;
  this->stride = stride;
  this->buffer_size = stride * height;
  this->buffer = (KPu8*)this + header_size;
  return this;
}

/******************************************************************************/
/* Linked list */

void kpLinkInsertAfter(KPlink_t *link, KPlink_t *after) {
  KP_ASSERT(link != 0); KP_ASSERT(after != 0);
  KP_ASSERT(link->next == 0); KP_ASSERT(link->prev == 0);
  link->prev = after;
  link->next = after->next;
  after->next = link;
  if (link->next != 0)
    link->next->prev = link;
}

void kpLinkRemove(KPlink_t *link) {
  if (link->next)
    link->next->prev = link->prev;
  if (link->prev)
    link->prev->next = link->next;
  link->next = link->prev = 0;
}

/******************************************************************************/
/* Message queue */

typedef struct KP__message_queue_t {
  KPmutex_t mutex;
  KPcondvar_t condvar;
  KPmessage_carrier_t *head, *tail;
} KP__message_queue_t;

KPmessage_queue_p kpMessageQueueCreate() {
  KP__message_queue_t *this = kpAlloc(sizeof(KP__message_queue_t));
  kpMutexInit(&this->mutex);
  kpCondvarInit(&this->condvar);
  this->head = this->tail = 0;
  return this;
}

void kpMessageQueueDestroy(KPmessage_queue_p queue) {
  KP__message_queue_t *this = (KP__message_queue_t*)queue;
  kpCondvarDestroy(&this->condvar);
  kpMutexDestroy(&this->mutex);
}

void kpMessageQueuePut(KPmessage_queue_p queue, KPmessage_carrier_t *carrier) {
  KP__message_queue_t *this = (KP__message_queue_t*)queue;
  carrier->link.next = carrier->link.prev = 0;
  
  kpMutexLock(&this->mutex);
  if (this->tail == 0)
    this->head = this->tail = carrier;
  else {
    kpLinkInsertAfter(&carrier->link, &this->tail->link);
    this->tail = carrier;
  }
  kpCondvarSignal(&this->condvar);
  kpMutexUnlock(&this->mutex);
}

void kpMessageQueuePutCopy(KPmessage_queue_p queue, const KPmessage_t *message){
  /* TODO message pool */
  KPmessage_carrier_t *carrier = kpNew(
    kp_allocator_default,
    sizeof(KPmessage_carrier_t) + message->size);
  carrier->msg = *message;
  carrier->msg.data = carrier + 1;
  carrier->msg.size = message->size;
  kpMemcpy(carrier->msg.data, message->data, message->size);
  kpMessageQueuePut(queue, carrier);
}

KPmessage_t *kpMessageQueueGet(KPmessage_queue_p queue, KPtime_ms timeout) {
  KP__message_queue_t *this = (KP__message_queue_t*)queue;
  kpMutexLock(&this->mutex);
  while (this->head == 0) {
    if (timeout == 0) return 0;
    /* TODO timeout */
    kpCondvarWait(&this->condvar, &this->mutex);
  }

  KP_ASSERT(this->head != 0);
  KPmessage_o ret = &this->head->msg;

  if (this->head == this->tail)
    this->head = this->tail = 0;
  else {
    KPlink_t *head = &this->head->link;
    this->head = (KPmessage_carrier_t*)
      ((KPu8*)(head->next) - offsetof(KPmessage_carrier_t, link));
    kpLinkRemove(head);
  }

  kpMutexUnlock(&this->mutex);
  return ret;
}
