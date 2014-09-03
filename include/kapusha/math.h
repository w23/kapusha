#pragma once

/* \todo libc-free */
#include <math.h>

#include "kapusha/sys.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/* Basic unaccelerated types with no storage requirements */
/* \todo devise accel vs non-accel strategy */

typedef struct {int x, y;} KPvec2i;
typedef struct {KPu8 x, y, z;} KPvec3b;
typedef struct {KPu8 x, y, z, w;} KPvec4b;

typedef struct {KPf32 x, y;} KPvec2f;
typedef struct {KPf32 x, y, z;} KPvec3f;
typedef struct {KPf32 x, y, z, w;} KPvec4f;

typedef struct {KPvec4f r[4];} KPmat4f;

typedef struct {KPvec2i bl, tr;} KPrect2i;

#define KPF32_PI (3.14159265358979323844F)
#define KPF32_2PI (KPF32_PI * 2.F)
#define KPF32_PI2 (KPF32_PI * .5F)
#define KPF32_PI4 (KPF32_PI * .25F)
#define KPF32_KDEG2RAD (KPF32_PI / 180.F)
#define KPF32_KRAD2DEG (180.F / KPF32_PI)

static inline KPvec2i kpVec2i(int x, int y) {
  KPvec2i v = {x, y};
  return v;
}

static inline KPvec3b kpVec3b(KPu8 x, KPu8 y, KPu8 z) {
  KPvec3b v = {x, y, z};
  return v;
}

/******************************************************************************/
/* Scalar operations */

static inline KPf32 kpAbsf(KPf32 f) { return fabsf(f); }
static inline KPf32 kpDistancef(KPf32 a, KPf32 b) { return kpAbsf(a-b); }
static inline KPf32 kpFloorf(KPf32 f) { return floorf(f); }
static inline KPf32 kpFractf(KPf32 f) { return f - kpFloorf(f); }
static inline KPf32 kpRecipf(KPf32 f) { return 1.f / f; }
static inline KPf32 kpSqrtf(KPf32 f) { return sqrtf(f); }
static inline KPf32 kpRSqrtf(KPf32 f) { return 1.f / sqrtf(f); }
static inline KPf32 kpSinf(KPf32 f) { return sinf(f); }
static inline KPf32 kpCosf(KPf32 f) { return cosf(f); }
static inline KPf32 kpTanf(KPf32 f) { return tanf(f); }
static inline KPf32 kpCopysign(KPf32 value, KPf32 sign) {
#if defined(_MSC_VER) && _MSC_VER < 1800
  return (KPf32)(_copysign(value, sign));
#else
  return copysign(value, sign);
#endif
}

/******************************************************************************/
/* Vector3 operations */

static inline KPvec3f kpVec3f(KPf32 x, KPf32 y, KPf32 z) {
  KPvec3f v = {x, y, z};
  return v;
}

static inline KPvec3f kpVec3ff(KPf32 f) {
  return kpVec3f(f, f, f);
}

static inline KPvec3f kpVec3fNeg(KPvec3f a) {
  return kpVec3f(-a.x, -a.y, -a.z);
}

static inline KPvec3f kpVec3fAdd(KPvec3f a, KPvec3f b) {
  return kpVec3f(a.x + b.x, a.y + b.y, a.z + b.z);
}

static inline KPvec3f kpVec3fSub(KPvec3f a, KPvec3f b) {
  return kpVec3f(a.x - b.x, a.y - b.y, a.z - b.z);
}

static inline KPvec3f kpVec3fMul(KPvec3f a, KPvec3f b) {
 return kpVec3f(a.x * b.x, a.y * b.y, a.z * b.z);
}

static inline KPvec3f kpVec3fMulf(KPvec3f v, KPf32 f) {
  return kpVec3f(v.x * f, v.y * f, v.z * f);
}

static inline KPf32 kpVec3fDot(KPvec3f a, KPvec3f b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

KPf32 kpVec3fLength(KPvec3f v);
KPvec3f kpVec3fNormalize(KPvec3f v);
KPvec3f kpVec3fCross(KPvec3f a, KPvec3f b);

/******************************************************************************/
/* Vector4 operations */

static inline KPvec4f kpVec4f(KPf32 x, KPf32 y, KPf32 z, KPf32 w) {
  KPvec4f v = {x, y, z, w};
  return v;
}

static inline KPvec4f kpVec4fNeg(KPvec4f v) {
  return kpVec4f(-v.x, -v.y, -v.z, -v.w);
}

static inline KPvec4f kpVec4fAdd(KPvec4f a, KPvec4f b) {
  KPvec4f ret = {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
  return ret;
}

static inline KPvec4f kpVec4fMul(KPvec4f a, KPvec4f b) {
  KPvec4f ret = {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
  return ret;
}

static inline KPvec4f kpVec4fMulf(KPvec4f v, KPf32 f) {
  KPvec4f ret = {v.x * f, v.y * f, v.z * f, v.w * f};
  return ret;
}

static inline KPf32 kpVec4fDot(KPvec4f a, KPvec4f b) {
  return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

KPf32 kpVec4fLength(KPvec4f v);
KPvec4f kpVec4fNormalize(KPvec4f v);

/******************************************************************************/
/* Mat4f operations */

static inline KPmat4f kpMat4f(KPvec4f r0, KPvec4f r1, KPvec4f r2, KPvec4f r3) {
  KPmat4f ret = {{r0, r1, r2, r3}};
  return ret;
}

static inline KPmat4f kpMat4fMakeIdentity() {
  return kpMat4f(kpVec4f(1,0,0,0),kpVec4f(0,1,0,0),
    kpVec4f(0,0,1,0),kpVec4f(0,0,0,1));
}

static inline KPmat4f kpMat4fMakeTranslation(KPvec3f t) {
  return kpMat4f(kpVec4f(1,0,0,t.x),kpVec4f(0,1,0,t.y),
    kpVec4f(0,0,1,t.z),kpVec4f(0,0,0,1));
}

KPmat4f kpMat4fTranspose(KPmat4f m);
KPvec4f kpMat4fMulv(const KPmat4f m, KPvec4f v);
KPmat4f kpMat4fMul(const KPmat4f a, const KPmat4f b);

KPmat4f kpMat4fMakePerspective(KPf32 fov, KPf32 aspect, KPf32 near, KPf32 far);

/******************************************************************************/
/* Quaternion operations */

typedef struct { KPvec4f v; } KPquatf;

static inline KPquatf kpQuatfVec4(KPvec4f v) {
  const KPquatf q = {v};
  return q;
}

static inline KPquatf kpQuatfMakeIdentity() {
  return kpQuatfVec4(kpVec4f(0.f,0.f,0.f,1.f));
}

KPquatf kpQuatfMakeRotation(KPvec3f v, KPf32 a);

static inline KPquatf kpQuatfMulf(KPquatf q, KPf32 f) {
  return kpQuatfVec4(kpVec4fMulf(q.v, f));
}

static inline KPquatf kpQuatfAdd(KPquatf a, KPquatf b) {
  return kpQuatfVec4(kpVec4fAdd(a.v, b.v));
}

KPquatf kpQuatfMul(KPquatf a, KPquatf b);

static inline KPquatf kpQuatfConjugate(KPquatf q) {
  return kpQuatfVec4(kpVec4f(-q.v.x, -q.v.y, -q.v.z, q.v.w));
}

static inline KPf32 kpQuatfMagnitude(KPquatf q) {
  return kpVec4fLength(q.v);
}

KPquatf kpQuatfNormalize(KPquatf q);
KPquatf kpQuatfMakeMat4f(KPmat4f m);
KPmat4f kpMat4fMakeQuatf(KPquatf q);

/******************************************************************************/
/* Dual quaternion operations */

typedef struct { KPquatf r, d; } KPdquatf;

KPdquatf kpDquatfMakeIdentity();
KPdquatf kpDquatfMakeQuatf(KPquatf q);
KPdquatf kpDquatfMakeTransform(KPvec3f axis, KPf32 angle, KPvec3f transl);
KPdquatf kpDquatfMakeMat4f(KPmat4f m);
KPdquatf kpDquatfMakeInverse(KPdquatf dq);

KPvec3f kpDquatfGetTranslation(KPdquatf dq);

KPdquatf kpDquatfMul(KPdquatf a, KPdquatf b);
KPdquatf kpDquatfNormalize(KPdquatf dq);

KPmat4f kpMat4fMakeDquatf(KPdquatf dq);

#ifdef __cplusplus
} // extern "C"
#endif
