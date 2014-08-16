#pragma once
#include <stdarg.h>

#include "kapusha/sys.h"

#ifdef __cplusplus
extern "C" {
#endif

void kpLog(const char *prefix, const char *format, ...);

#define KP_L(...) kpLog("APP", __VA_ARGS__)
#define KP__L(...) kpLog(KP__SYS, __VA_ARGS__)

typedef struct KPheap_header_t {
  void *(*alloc)(struct KPheap_header_t *heap, KPsize size);
  void (*free)(struct KPheap_header_t *heap, void *mem);
} KPheap_header_t;

typedef KPheap_header_t* KPheap_p;

typedef struct {
  KPs32_atomic refcount;
  void (*dtor)(void *obj);
  KPheap_p heap;
} KPobject_header_t;

#define KP_O KPobject_header_t O
#define KP_THIS(name,param) name *this = (name*)(param)

inline static void *kpRetain(void *obj) {
  if (!obj) return obj;
  KP_THIS(KPobject_header_t, obj);
  kpS32AtomicInc(&this->refcount);
  return obj;
}

void kpRelease(void *obj);

typedef struct {
  KPsize size;
  const void *data;
} KPblob_desc_t;

/* generic heap allocation */

extern KPheap_p kp__heap_default;

inline static void *kpHeapAlloc(KPheap_p heap, KPsize size) {
  if (heap == 0) heap = kp__heap_default;
  return heap->alloc(heap, size);
}

inline static void kpHeapFree(KPheap_p heap, void* ptr) {
  if (heap == 0) heap = kp__heap_default;
  return heap->free(heap, ptr);
}

inline static void *kpAlloc(KPsize size) { return kpHeapAlloc(0, size); }
inline static void kpFree(void *ptr) { kpHeapFree(0, ptr); }

void *kpNew(KPheap_p heap, KPsize size);

#define KP_NEW(name,heap) (name*)kpNew(heap, sizeof(name));

static inline void kpMemcpy(void *dst, const void *src, KPsize size) {
  for (KPsize i = 0; i < size; ++i)
    ((char*)dst)[i] = ((const char*)src)[i];
}

static inline void kpMemset(void *dst, int value, KPsize size) {
  for (KPsize i = 0; i < size; ++i)
    ((char*)dst)[i] = value;
}

int kpVsnprintf(char *buffer, KPsize size, const char *format, va_list argp);
int kpSnprintf(char *buffer, KPsize size, const char *format, ...);

#ifdef __cplusplus
} // extern "C"
#endif
