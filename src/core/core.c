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

