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

KPmat4f kpMat4fTranspose(KPmat4f m) {
  return kpMat4f(
    kpVec4f(m.r[0].x, m.r[1].x, m.r[2].x, m.r[3].x),
    kpVec4f(m.r[0].y, m.r[1].y, m.r[2].y, m.r[3].y),
    kpVec4f(m.r[0].z, m.r[1].z, m.r[2].z, m.r[3].z),
    kpVec4f(m.r[0].w, m.r[1].w, m.r[2].w, m.r[3].w));
}

KPvec4f kpMat4fMulv(const KPmat4f m, KPvec4f v) {
  return kpVec4f(
    kpVec4fDot(m.r[0], v),
    kpVec4fDot(m.r[1], v),
    kpVec4fDot(m.r[2], v),
    kpVec4fDot(m.r[3], v)
  );
}

KPmat4f kpMat4fMul(KPmat4f a, KPmat4f b) {
  b = kpMat4fTranspose(b);
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

KPmat4f kpMat4fMakePerspective(KPf32 fov, KPf32 aspect, KPf32 near, KPf32 far) {
  KPmat4f m = kpMat4fMakeIdentity();
  KPf32 fn = near - far;
  m.r[0] = kpVec4f(2.f * near / aspect, 0, 0, 0);
  m.r[1] = kpVec4f(0, 2.f * near, 0, 0);
  m.r[2] = kpVec4f(0, 0, (far + near) / fn, 2.f * far * near / fn);
  m.r[3] = kpVec4f(0, 0, -1, 0);
  return m;
}

/******************************************************************************/
/* Quaternions */

KPquatf kpQuatfMakeRotation(KPvec3f v, KPf32 a) {
  a *= .5f;
  KPf32 c = kpCosf(a), s = kpSinf(a);
  return kpQuatfVec4(kpVec4f(v.x * s, v.y * s, v.z * s, c));
}

KPquatf kpQuatfMul(KPquatf a, KPquatf b) {
  const KPvec3f av = kpVec3f(a.v.x, a.v.y, a.v.z);
  const KPvec3f bv = kpVec3f(b.v.x, b.v.y, b.v.z);
  const KPvec3f vv =
    kpVec3fAdd(
      kpVec3fAdd(kpVec3fMulf(bv, a.v.w), kpVec3fMulf(av, b.v.w)),
      kpVec3fCross(av, bv));
  KPvec4f v;
  v.w = a.v.w * b.v.w - kpVec3fDot(av, bv);
  v.x = vv.x; v.y = vv.y; v.z = vv.z;
  return kpQuatfVec4(v);
}

KPquatf kpQuatfNormalize(KPquatf q) {
  return kpQuatfVec4(kpVec4fMulf(q.v,kpRSqrtf(kpVec4fDot(q.v,q.v))));
}

KPquatf kpQuatfMakeMat4f(KPmat4f m) {
  KPquatf q;
  const KPf32 tr = m.r[0].x + m.r[1].y + m.r[2].z;
  if (tr > 0.f) { /* w is fine */
    q.v.w = kpSqrtf(tr + 1.f) * .5f;
    const KPf32 wwww = q.v.w * 4.f;
    q.v.x = (m.r[2].y - m.r[1].z) / wwww;
    q.v.y = (m.r[0].z - m.r[2].x) / wwww;
    q.v.z = (m.r[1].x - m.r[0].y) / wwww;
  } else if (m.r[0].x > m.r[1].y && m.r[0].x > m.r[2].z) {
    /* x is the largest */
    q.v.x = kpSqrtf(1.f + m.r[0].x - m.r[1].y - m.r[2].z) * .5f;
    const KPf32 xxxx = q.v.x * 4.f;
    q.v.y = (m.r[0].y + m.r[1].x) / xxxx;
    q.v.z = (m.r[0].z + m.r[2].x) / xxxx;
    q.v.w = (m.r[2].y - m.r[1].z) / xxxx;
  } else if (m.r[1].y > m.r[2].z) { /* y is the largest */
    q.v.y = kpSqrtf(1.f - m.r[0].x + m.r[1].y - m.r[2].z) * .5f;
    const KPf32 yyyy = q.v.y * 4.f;
    q.v.x = (m.r[0].y + m.r[1].x) / yyyy;
    q.v.z = (m.r[1].z + m.r[2].y) / yyyy;
    q.v.w = (m.r[0].z - m.r[2].x) / yyyy;
  } else { /* nokori wa z */
    q.v.z = kpSqrtf(1.f - m.r[0].x - m.r[1].y + m.r[2].z) * .5f;
    const KPf32 zzzz = q.v.z * 4.f;
    q.v.x = (m.r[0].z + m.r[2].x) / zzzz;
    q.v.y = (m.r[1].z + m.r[2].y) / zzzz;
    q.v.w = (m.r[1].x - m.r[0].y) / zzzz;
  }
  return kpQuatfNormalize(q);
}

KPmat4f kpMat4fMakeQuatf(KPquatf q) {
  KPmat4f m;
  m.r[0].x = 1.f - 2.f * (q.v.y * q.v.y + q.v.z * q.v.z);
  m.r[0].y =       2.f * (q.v.x * q.v.y - q.v.z * q.v.w);
  m.r[0].z =       2.f * (q.v.x * q.v.z + q.v.y * q.v.w);
  m.r[0].w = 0.f;
  m.r[1].x =       2.f * (q.v.x * q.v.y + q.v.z * q.v.w);
  m.r[1].y = 1.f - 2.f * (q.v.x * q.v.x + q.v.z * q.v.z);
  m.r[1].z =       2.f * (q.v.y * q.v.z - q.v.x * q.v.w);
  m.r[1].w = 0.f;
  m.r[2].x =       2.f * (q.v.x * q.v.z - q.v.y * q.v.w);
  m.r[2].y =       2.f * (q.v.y * q.v.z + q.v.x * q.v.w);
  m.r[2].z = 1.f - 2.f * (q.v.x * q.v.x + q.v.y * q.v.y);
  m.r[2].w = 0.f;
  m.r[3].x = m.r[3].y = m.r[3].z = 0.f;
  m.r[3].w = 1.f;
  return m;
}

/******************************************************************************/
/* Dual quaternions */

KPdquatf kpDquatfMakeTransform(KPvec3f axis, KPf32 angle, KPvec3f t) {
  KPdquatf dq;
  dq.r = kpQuatfMakeRotation(axis, angle);
  dq.d = kpQuatfMul(kpQuatfVec4(
    kpVec4f(t.x * .5f, t.y * .5f, t.z * .5f, 0.f)), dq.r);
  return dq;
}

KPdquatf kpDquatfMakeMat4f(KPmat4f m) {
  KPdquatf dq;
  dq.r = kpQuatfMakeMat4f(m);
  dq.d = kpQuatfMul(kpQuatfVec4(
    kpVec4f(m.r[0].w * .5f, m.r[1].w * .5f, m.r[2].w * .5f, 0.f)), dq.r);
  return kpDquatfNormalize(dq);
}

KPvec3f kpDquatfGetTranslation(KPdquatf dq) {
  KPvec4f t = kpVec4fMulf(kpQuatfMul(dq.d, kpQuatfConjugate(dq.r)).v, 2.f);
  return kpVec3f(t.x, t.y, t.z);
}

KPmat4f kpMat4fMakeDquatf(KPdquatf dq) {
  KPmat4f m = kpMat4fMakeQuatf(dq.r);
  const KPvec3f t = kpDquatfGetTranslation(dq);
  m.r[0].w = t.x;
  m.r[1].w = t.y;
  m.r[2].w = t.z;
  return m;
}

KPdquatf kpDquatfMakeInverse(KPdquatf dq) {
  KPdquatf ret;
  ret.r = kpQuatfConjugate(dq.r);
  ret.d = kpQuatfMul(ret.r, kpQuatfMul(dq.d, ret.r));
  ret.d.v.x = -ret.d.v.x;
  ret.d.v.y = -ret.d.v.y;
  ret.d.v.z = -ret.d.v.z;
  ret.d.v.w = -ret.d.v.w;
  return ret;
}

KPdquatf kpDquatfMul(KPdquatf a, KPdquatf b) {
  KPdquatf dq;
  dq.r = kpQuatfMul(a.r, b.r);
  dq.d = kpQuatfAdd(kpQuatfMul(a.r,b.d),kpQuatfMul(a.d,b.r));
  return dq;
}

KPdquatf kpDquatfNormalize(KPdquatf dq) {
  const KPf32 scale = kpRSqrtf(kpVec4fDot(dq.r.v, dq.r.v));
  dq.r.v = kpVec4fMulf(dq.r.v, scale);
  dq.d.v = kpVec4fMulf(dq.d.v, scale);
  return dq;
}
