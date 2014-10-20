#pragma once

#include "kapusha/math.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Frame of reference */
typedef struct {
  KPdquatf dq_;
  KPmat4f mat_;
  KPu32 flags_;
} KPreframe_t;

void kpReframeMakeIdentity(KPreframe_t *f);
void kpReframeMakeDquatf(KPreframe_t *f, KPdquatf dq);
void kpReframeMakeTransform(KPreframe_t *f,
  KPvec3f axis, KPf32 angle, KPvec3f translation);
void kpReframeMakeRotation(KPreframe_t *f,
  KPvec3f axis, KPf32 angle);
void kpReframeMakeLookAt(KPvec3f pos, KPvec3f at, KPvec3f up);

const KPmat4f *kpReframeGetMatrix(KPreframe_t *f);
static inline const KPdquatf *kpReframeGetDquat(KPreframe_t *f) {
  return &f->dq_;
}

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

/******************************************************************************/
/* camera */

typedef struct {
  KPreframe_t invframe_;
  KPmat4f proj_;
  KPmat4f mvp_;
  KPu32 flags_;
} KPcamera_t;

void kpCameraMakeIdentity(KPcamera_t *c);
void kpCameraSetProjection(KPcamera_t *c,
  KPf32 fov, KPf32 aspect, KPf32 near, KPf32 far);
/* \todo void kpCameraSetLookAt(KPcamera_t *c, KPvec3f pos, KPvec3f at, KPvec3f up);*/
void kpCameraSetPosition(KPcamera_t *c, KPvec3f pos);
void kpCameraSetReframe(KPcamera_t *c, const KPreframe_t *frame);

KPmat4f kpCameraGetVP(KPcamera_t *c);
KPmat4f kpCameraGetP(KPcamera_t *c);
KPmat4f kpCameraGetV(KPcamera_t *c);
KPvec3f kpCameraGetPosition(KPcamera_t *c);

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
