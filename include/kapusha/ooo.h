#pragma once

#include "kapusha/math.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Frame of reference */
typedef struct {
  KPdquatf dq;
  KPmat4f mat;
} KPreframe_t;

void kpReframeMakeIdentity(KPreframe_t *f);
void kpReframeMakeDQ(KPreframe_t *f, KPdquatf dq);
static inline void kpReframeMakeTransform(KPreframe_t *f,
  KPvec3f axis, KPf32 angle, KPvec3f translation)
{
  kpReframeMakeDQ(f,
    kpDquatfMakeTransform(axis, angle, translation));
}

static inline void kpReframeMakeRotation(KPreframe_t *f,
  KPvec3f axis, KPf32 angle)
{
  kpReframeMakeTransform(f, axis, angle, kpVec3f(0,0,0));
}

void kpReframeMakeLookAt(KPvec3f pos, KPvec3f at, KPvec3f up);

void kpReframeSyncMatrix(KPreframe_t *f);

KPvec3f kpReframeGetXAxis(KPreframe_t *f);
KPvec3f kpReframeGetYAxis(KPreframe_t *f);
KPvec3f kpReframeGetZAxis(KPreframe_t *f);
KPvec3f kpReframeGetTranslation(KPreframe_t *f);
KPvec4f kpReframeGetRotation(KPreframe_t *f);

void kpReframeTranslate(KPreframe_t *f, KPvec3f by);
void kpReframeRotateAroundSelfX(KPreframe_t *f, KPf32 angle);
void kpReframeRotateAroundSelfY(KPreframe_t *f, KPf32 angle);
void kpReframeRotateAroundSelfZ(KPreframe_t *f, KPf32 angle);
void kpReframeRotateAroundAxis(KPreframe_t *f, KPvec3f axis, KPf32 angle);

typedef struct {
  KPvec3f pos_;
  KPquatf qorient_;
  KPmat4f orient_;
  KPmat4f view_;
  KPmat4f proj_;
  KPmat4f mvp_;
  KPu32 flags_;
} KPcamera_t;

void kpCameraMakeIdentity(KPcamera_t *c);
void kpCameraSetProjection(KPcamera_t *c,
  KPf32 fov, KPf32 aspect, KPf32 near, KPf32 far);
void kpCameraSetLookAt(KPcamera_t *c, KPvec3f pos, KPvec3f at, KPvec3f up);
void kpCameraSetPosition(KPcamera_t *c, KPvec3f pos);

KPmat4f kpCameraGetVP(KPcamera_t *c);
KPmat4f kpCameraGetP(KPcamera_t *c);
KPmat4f kpCameraGetV(KPcamera_t *c);
static inline KPvec3f kpCameraGetPosition(KPcamera_t *c) {
  return c->pos_;
}

KPvec3f kpCameraGetX(KPcamera_t *c);
KPvec3f kpCameraGetY(KPcamera_t *c);
KPvec3f kpCameraGetZ(KPcamera_t *c);
static inline KPvec3f kpCameraGetForward(KPcamera_t *c) {
  return kpVec3fNeg(kpCameraGetZ(c));
}

void kpCameraMove(KPcamera_t *c, KPvec3f v);
void kpCameraMoveForward(KPcamera_t *c, KPf32 units);
void kpCameraMoveRight(KPcamera_t *c, KPf32 units);
void kpCameraMoveUp(KPcamera_t *c, KPf32 units);

void kpCameraRotateHead(KPcamera_t *c, KPf32 angle);
void kpCameraRotatePitch(KPcamera_t *c, KPf32 angle);
void kpCameraRotateRoll(KPcamera_t *c, KPf32 angle);


#ifdef __cplusplus
} // extern "C"
#endif
