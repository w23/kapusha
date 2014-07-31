#include "kapusha/core.h"

extern KPheap_p heap_default;

void *kpNew(KPheap_p heap, KPsize size) {
  KP_THIS(KPobject_header_t, kpHeapAlloc(heap, size));
  this->heap = heap;
  this->refcount = 1;
  return this;
}

void kpRelease(void *obj) {
  KP_THIS(KPobject_header_t, obj);
  if (kpS32AtomicDec(&this->refcount) > 0) return;
  if (this->dtor) this->dtor(this);
  kpHeapFree(this->heap, this);
}