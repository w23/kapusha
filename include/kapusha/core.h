#pragma once

#include "kapusha/sys.h"

#ifdef __cplusplus
extern "C" {
#endif

void kpLog(const char *format, ...);

#define KP_L(...) kpLog(__VA_ARGS__)

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
  if (heap == NULL) heap = kp__heap_default;
  return heap->alloc(heap, size);
}

inline static void kpHeapFree(KPheap_p heap, void* ptr) {
  if (heap == NULL) heap = kp__heap_default;
  return heap->free(heap, ptr);
}

inline static void *kpAlloc(KPsize size) { return kpHeapAlloc(NULL, size); }
inline static void kpFree(void *ptr) { kpHeapFree(NULL, ptr); }

void *kpNew(KPheap_p heap, KPsize size);

#define KP_NEW(name,heap) (name*)kpNew(heap, sizeof(name));


#ifdef __cplusplus
} // extern "C"
#endif
