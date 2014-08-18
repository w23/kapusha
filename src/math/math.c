#include "kapusha/math.h"

KPf32 kpVec3fLength(KPvec3f v) {
  return kpSqrtf(kpVec3fDot(v, v));
}

KPvec3f kpVec3fNormalize(KPvec3f v) {
  return kpVec3fMulf(v, kpRSqrtf(kpVec3fDot(v,v)));
}

KPvec3f kpVec3fCross(KPvec3f a, KPvec3f b) {
  KPvec3f ret = {
    a.y * b.z - a.z * b.y,
    a.z * b.x - a.x * b.z,
    a.x * b.y - a.y * b.x};
  return ret;
}

KPf32 kpVec4fLength(KPvec4f v) {
  return kpSqrtf(kpVec4fDot(v, v));
}

KPvec4f kpVec4fNormalize(KPvec4f v) {
  return kpVec4fMulf(v, kpRSqrtf(kpVec4fDot(v,v)));
}

void kpMat4fTranspose(KPmat4f *m) {
  KPf32 t;
  t = m->r[0].y; m->r[0].y = m->r[1].x; m->r[1].x = t;
  t = m->r[0].z; m->r[0].z = m->r[2].x; m->r[2].x = t;
  t = m->r[0].w; m->r[0].w = m->r[3].x; m->r[3].x = t;
  t = m->r[1].z; m->r[1].z = m->r[2].y; m->r[2].y = t;
  t = m->r[1].w; m->r[1].w = m->r[3].y; m->r[3].y = t;
  t = m->r[2].w; m->r[2].w = m->r[3].z; m->r[3].z = t;
}

KPvec4f kpMat4fMulv4(const KPmat4f *m, KPvec4f v) {
  return kpVec4f(
    kpVec4fDot(m->r[0], v),
    kpVec4fDot(m->r[1], v),
    kpVec4fDot(m->r[2], v),
    kpVec4fDot(m->r[3], v)
  );
}

KPmat4f kpMat4fMulm4(const KPmat4f *a, const KPmat4f *b) {
  KPmat4f tb = *b;
  kpMat4fTranspose(&tb);
  return kpMat4f(
    kpVec4f(
      kpVec4fDot(a->r[0], b->r[0]),
      kpVec4fDot(a->r[0], b->r[1]),
      kpVec4fDot(a->r[0], b->r[2]),
      kpVec4fDot(a->r[0], b->r[3])),
    kpVec4f(
      kpVec4fDot(a->r[1], b->r[0]),
      kpVec4fDot(a->r[1], b->r[1]),
      kpVec4fDot(a->r[1], b->r[2]),
      kpVec4fDot(a->r[1], b->r[3])),
    kpVec4f(
      kpVec4fDot(a->r[2], b->r[0]),
      kpVec4fDot(a->r[2], b->r[1]),
      kpVec4fDot(a->r[2], b->r[2]),
      kpVec4fDot(a->r[2], b->r[3])),
    kpVec4f(
      kpVec4fDot(a->r[3], b->r[0]),
      kpVec4fDot(a->r[3], b->r[1]),
      kpVec4fDot(a->r[3], b->r[2]),
      kpVec4fDot(a->r[3], b->r[3])));
}
