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

void kpMat4fTranspose(KPmat4f m) {
  KPf32 t;
  t = m.r[0].y; m.r[0].y = m.r[1].x; m.r[1].x = t;
  t = m.r[0].z; m.r[0].z = m.r[2].x; m.r[2].x = t;
  t = m.r[0].w; m.r[0].w = m.r[3].x; m.r[3].x = t;
  t = m.r[1].z; m.r[1].z = m.r[2].y; m.r[2].y = t;
  t = m.r[1].w; m.r[1].w = m.r[3].y; m.r[3].y = t;
  t = m.r[2].w; m.r[2].w = m.r[3].z; m.r[3].z = t;
}

KPvec4f kpMat4fMulv4(const KPmat4f m, KPvec4f v) {
  return kpVec4f(
    kpVec4fDot(m.r[0], v),
    kpVec4fDot(m.r[1], v),
    kpVec4fDot(m.r[2], v),
    kpVec4fDot(m.r[3], v)
  );
}

KPmat4f kpMat4fMulm4(const KPmat4f a, const KPmat4f b) {
  KPmat4f tb = b;
  kpMat4fTranspose(tb);
  return kpMat4f(
    kpVec4f(
      kpVec4fDot(a.r[0], b.r[0]),
      kpVec4fDot(a.r[0], b.r[1]),
      kpVec4fDot(a.r[0], b.r[2]),
      kpVec4fDot(a.r[0], b.r[3])),
    kpVec4f(
      kpVec4fDot(a.r[1], b.r[0]),
      kpVec4fDot(a.r[1], b.r[1]),
      kpVec4fDot(a.r[1], b.r[2]),
      kpVec4fDot(a.r[1], b.r[3])),
    kpVec4f(
      kpVec4fDot(a.r[2], b.r[0]),
      kpVec4fDot(a.r[2], b.r[1]),
      kpVec4fDot(a.r[2], b.r[2]),
      kpVec4fDot(a.r[2], b.r[3])),
    kpVec4f(
      kpVec4fDot(a.r[3], b.r[0]),
      kpVec4fDot(a.r[3], b.r[1]),
      kpVec4fDot(a.r[3], b.r[2]),
      kpVec4fDot(a.r[3], b.r[3])));
}

KPquatf kpQuatfRotaion(KPvec3f v, KPf32 a) {
  a *= .5f;
  KPf32 c = kpCosf(a), s = kpSinf(a);
  return kpQuatfv4(kpVec4f(v.x * s, v.y * s, v.z * s, c));
}

KPquatf kpQuatfMulq(KPquatf a, KPquatf b) {
  const KPvec3f av = kpVec3f(a.v.x, a.v.y, a.v.z);
  const KPvec3f bv = kpVec3f(b.v.x, b.v.y, b.v.z);
  const KPvec3f vv =
    kpVec3fAddv3(
      kpVec3fAddv3(kpVec3fMulf(bv, a.v.w), kpVec3fMulf(av, b.v.w)),
      kpVec3fCross(av, bv));
  KPvec4f v;
  v.w = a.v.w * b.v.w - kpVec3fDot(av, bv);
  v.x = vv.x; v.y = vv.y; v.z = vv.z;
  return kpQuatfv4(v);
}

KPquatf kpQuatfNormalize(KPquatf q) {
  return kpQuatfv4(kpVec4fMulf(q.v,kpRSqrtf(kpVec4fDot(q.v,q.v))));
}

KPdquatf kpDquatRotationTranslation(KPvec3f axis, KPf32 angle, KPvec3f t) {
  KPdquatf dq;
  dq.r = kpQuatfRotaion(axis, angle);
  dq.d = kpQuatfMulq(kpQuatfv4(
    kpVec4f(t.x * .5f, t.y * .5f, t.z * .5f, 0.f)), dq.r);
  return dq;
}

KPvec3f kpDquatGetTranslation(KPdquatf dq) {
  KPvec4f t = kpVec4fMulf(kpQuatfMulq(dq.d, kpQuatfConjugate(dq.r)).v, 2.f);
  return kpVec3f(t.x, t.y, t.z);
}

KPmat4f kpMat4fdq(KPdquatf dq) {
  KPmat4f m;
  KPvec3f t = kpDquatGetTranslation(dq);
  m.r[0].x = 1.f - 2.f * (dq.r.v.y * dq.r.v.y + dq.r.v.z * dq.r.v.z);
  m.r[0].y =       2.f * (dq.r.v.x * dq.r.v.y - dq.r.v.z * dq.r.v.w);
  m.r[0].z =       2.f * (dq.r.v.x * dq.r.v.z + dq.r.v.y * dq.r.v.w);
  m.r[0].w = t.x;
  m.r[1].x =       2.f * (dq.r.v.x * dq.r.v.y + dq.r.v.z * dq.r.v.w);
  m.r[1].y = 1.f - 2.f * (dq.r.v.x * dq.r.v.x + dq.r.v.z * dq.r.v.z);
  m.r[1].z =       2.f * (dq.r.v.y * dq.r.v.z - dq.r.v.x * dq.r.v.w);
  m.r[1].w = t.y;
  m.r[2].x =       2.f * (dq.r.v.x * dq.r.v.z - dq.r.v.y * dq.r.v.w);
  m.r[2].y =       2.f * (dq.r.v.y * dq.r.v.z + dq.r.v.x * dq.r.v.w);
  m.r[2].z = 1.f - 2.f * (dq.r.v.x * dq.r.v.x + dq.r.v.y * dq.r.v.y);
  m.r[2].w = t.z;
  m.r[3].x = m.r[3].y = m.r[3].z = 0.f;
  m.r[3].w = 1.f;
  return m;
}
