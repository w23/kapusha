#include "kapusha/core.h"

/* \todo do not depend on libc */
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

static void *default_alloc(struct KPheap_header_t *heap, KPsize size) {
  /* \todo libc-based malloc should be separate compile-time decision */
  KP_UNUSED(heap);
  return malloc(size);
}

static void default_free(struct KPheap_header_t *heap, void *mem) {
  KP_UNUSED(heap);
  free(mem);
}

static KPheap_header_t kp__heap_default_impl = {
  default_alloc, default_free
};

KPheap_p kp__heap_default = &kp__heap_default_impl;

void *kpNew(KPheap_p heap, KPsize size) {
  KP_THIS(KPobject_header_t, kpHeapAlloc(heap, size));
  this->heap = heap;
  this->refcount = 1;
  this->dtor = 0;
  return this;
}

void kpRelease(void *obj) {
  if (!obj) return;
  KP_THIS(KPobject_header_t, obj);
  if (kpS32AtomicDec(&this->refcount) > 0) return;
  if (this->dtor) this->dtor(this);
  kpHeapFree(this->heap, this);
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

  KPsurface_t *this = kpNew(0, total_size);
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

typedef struct KPlink_t {
  struct KPlink_t *next;
  struct KPlink_t *prev;
} KPlink_t;

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

typedef struct KP__message_t {
  KPlink_t link;
  KPmessage_t msg;
} KP__message_t;

typedef struct KP__message_queue_t {
  KPmutex_t mutex;
  KPcondvar_t condvar;
  KP__message_t *head, *tail;
} KP__message_queue_t;

KPmessage_queue_t kpMessageQueueCreate() {
  KP__message_queue_t *this = kpAlloc(sizeof(KP__message_queue_t));
  kpMutexInit(&this->mutex);
  kpCondvarInit(&this->condvar);
  this->head = this->tail = 0;
  return this;
}

void kpMessageQueueDestroy(KPmessage_queue_t queue) {
  KP__message_queue_t *this = (KP__message_queue_t*)queue;
  kpCondvarDestroy(&this->condvar);
  kpMutexDestroy(&this->mutex);
}

void kpMessageQueuePut(
  KPmessage_queue_t queue, KPu32 tag, KPu32 type, const void *data, KPsize size)
{
  KP__message_queue_t *this = (KP__message_queue_t*)queue;

  /* TODO message pool */
  KP__message_t *msg = kpAlloc(sizeof(KP__message_t) + size);
  msg->link.next = msg->link.prev = 0;
  msg->msg.tag = tag;
  msg->msg.type = type;
  msg->msg.data = msg + 1;
  msg->msg.size = size;
  kpMemcpy(msg->msg.data, data, size);

  kpMutexLock(&this->mutex);
  if (this->tail == 0)
    this->head = this->tail = msg;
  else {
    kpLinkInsertAfter(&msg->link, &this->tail->link);
    this->tail = msg;
  }
  kpCondvarSignal(&this->condvar);
  kpMutexUnlock(&this->mutex);
}

KPmessage_t *kpMessageQueueGet(KPmessage_queue_t queue, KPtime_ms timeout) {
  KP__message_queue_t *this = (KP__message_queue_t*)queue;
  kpMutexLock(&this->mutex);
  while (this->head == 0) {
    if (timeout == 0) return 0;
    /* TODO timeout */
    kpCondvarWait(&this->condvar, &this->mutex);
  }

  KP_ASSERT(this->head != 0);
  KPmessage_t *ret = &this->head->msg;

  if (this->head == this->tail)
    this->head = this->tail = 0;
  else {
    KPlink_t *head = &this->head->link;
    this->head = (KP__message_t*)head->next;
    kpLinkRemove(head);
  }

  kpMutexUnlock(&this->mutex);
  return ret;
}

void kpMessageDiscard(KPmessage_t *message) {
  /* TODO return to pool */
  kpFree(((KPu8*)message) - sizeof(KPlink_t));
}
