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
typedef struct {KPf32 x, y;} KPvec2f;
typedef struct {KPf32 x, y, z;} KPvec3f;
typedef struct {KPf32 x, y, z, w;} KPvec4f;

typedef struct {KPvec4f r[4];} KPmat4f;

typedef struct {KPvec2i bl, tr;} KPrect2i;

#define KPf32_pi (3.14159265358979323844f)
#define KPf32_2pi (KPf32_pi * 2.f)
#define KPf32_pi2 (KPf32_pi * .5f)
#define KPf32_pi4 (KPf32_pi * .25f)
#define KPf32_kdeg2rad (KPf32_pi / 180.f)
#define KPf32_krad2deg (180.f / KPf32_pi)

static inline KPvec2i kpVec2i(int x, int y) {
  KPvec2i v = {x, y};
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

static inline KPvec3f kpVec3fAddv3(KPvec3f a, KPvec3f b) {
  return kpVec3f(a.x + b.x, a.y + b.y, a.z + b.z);
}

static inline KPvec3f kpVec3fMulv3(KPvec3f a, KPvec3f b) {
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

static inline KPvec4f kpVec4fAddv4(KPvec4f a, KPvec4f b) {
  KPvec4f ret = {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
  return ret;
}

static inline KPvec4f kpVec4fMulv4(KPvec4f a, KPvec4f b) {
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

void kpMat4fTranspose(KPmat4f *m);
KPvec4f kpMat4fMulv4(const KPmat4f *m, KPvec4f v);
KPmat4f kpMat4fMulm4(const KPmat4f *a, const KPmat4f *b);


#ifdef __cplusplus
} // extern "C"
#endif
