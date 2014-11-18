#pragma once
#include <stdarg.h>

#include "kapusha/sys.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/* Debug log */

void kpLog(const char *prefix, const char *format, ...);

#define KP_L(...) kpLog("APP", __VA_ARGS__)
#define KP__L(...) kpLog(KP__SYS, __VA_ARGS__)

/******************************************************************************/
/* Heaps and allocation */

typedef struct KPheap_header_t {
  void *(*alloc)(struct KPheap_header_t *heap, KPsize size);
  void (*free)(struct KPheap_header_t *heap, void *mem);
} KPheap_header_t;

typedef KPheap_header_t* KPheap_p;

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

/******************************************************************************/
/* Base building block refcounted object */

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

void *kpNew(KPheap_p heap, KPsize size);

#define KP_NEW(name,heap) (name*)kpNew(heap, sizeof(name));

/******************************************************************************/
/* Misc memory stuff */

typedef struct {
  KPsize size;
  const void *data;
} KPblob_desc_t;

static inline void kpMemcpy(void *dst, const void *src, KPsize size) {
  KPsize i;
  for (i = 0; i < size; ++i)
    ((char*)dst)[i] = ((const char*)src)[i];
}

static inline void kpMemset(void *dst, int value, KPsize size) {
  KPsize i;
  for (i = 0; i < size; ++i)
    ((char*)dst)[i] = value;
}

static inline int kpMemcmp(const void *left, const void *right, KPsize size) {
  KPsize i;
  for (i = 0; i < size; ++i) {
    /* TODO optimize */
    if (((const KPu8*)left)[i] < ((const KPu8*)right)[i]) return -1;
    if (((const KPu8*)left)[i] > ((const KPu8*)right)[i]) return 1;
  }
  return 0;
}

int kpVsnprintf(char *buffer, KPsize size, const char *format, va_list argp);
int kpSnprintf(char *buffer, KPsize size, const char *format, ...);

/******************************************************************************/
/* Message queue */

typedef void* KPmessage_queue_t;

typedef struct KPmessage_t {
  KPu32 tag;
  void *origin;
  KPu32 type, param;
  KPsize size;
  void *data;
} KPmessage_t;

KPmessage_queue_t kpMessageQueueCreate();
void kpMessageQueueDestroy(KPmessage_queue_t queue);

void kpMessageQueuePut(KPmessage_queue_t queue,
  KPu32 tag, void *origin, KPu32 type, KPu32 param,
  const void *data, KPsize size);
KPmessage_t *kpMessageQueueGet(KPmessage_queue_t queue, KPtime_ms timeout);
void kpMessageDiscard(KPmessage_t *message);

/******************************************************************************/
/* Lock-free queue */

/* todo linked-list stack
 *
 * void kpLockFreeQueuePut(); push on top
 * void kpLockFreeQueuePollAll(); xchg top, reverse links
 *
 * */

/******************************************************************************/
/* Cotainers */

typedef struct KPbuffer_t { KP_O;
  KPsize size;
  void *data;
} KPbuffer_t, *KPbuffer_o;

KPbuffer_o kpBufferCreate(KPsize size, const void *source);

/******************************************************************************/
/* String */

typedef struct KPstring_t { KPbuffer_t buf;
} KPstring_t, *KPstring_o;

KPstring_o kpStringCreate(const char *string);
static inline KPsize kpStringLength(KPstring_o str) {
  return str->buf.size - 1;
}
static inline const char *kpStringCString(KPstring_o str) {
  return str->buf.data;
}

/******************************************************************************/
/* Surface */

typedef enum {
  KPSurfaceFormatU8R,
  KPSurfaceFormatU8RG,
  KPSurfaceFormatU8RGB,
  KPSurfaceFormatU8RGBA,
  KPSurfaceFormatR5G6B5,
  KPSurfaceFormatF32RGBA,
  KPSurfaceFormat_Max
} KPSurfaceFormat;

typedef struct { KP_O;
  KPu32 width, height;
  KPSurfaceFormat format;
  KPu32 bytes_per_pixel;
  KPu32 stride, buffer_size;
  void *buffer;
} KPsurface_t;

typedef KPsurface_t *KPsurface_o;

KPsurface_o kpSurfaceCreate(KPu32 width, KPu32 height, KPSurfaceFormat fmt);

#ifdef __cplusplus
} // extern "C"
#endif
