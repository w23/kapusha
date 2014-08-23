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
  KPreframe_t frame;
  KPmat4f proj;
  KPmat4f mvp;
} KPcamera_t;

void kpCameraMake(KPcamera_t *c, KPvec3f pos, KPvec3f at, KPvec3f up);
void kpCameraSetPosition(KPcamera_t *c, KPvec3f pos, KPvec3f at, KPvec3f up);
void kpCameraMoveForward(KPcamera_t *c, KPf32 units);
void kpCameraMoveRight(KPcamera_t *c, KPf32 units);
void kpCameraMoveUp(KPcamera_t *c, KPf32 units);


#ifdef __cplusplus
} // extern "C"
#endif
