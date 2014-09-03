#include "kapusha/ooo.h"

void kpReframeMakeIdentity(KPreframe_t *f) {
  f->dq = kpDquatfMakeTransform(kpVec3f(0,0,0),0, kpVec3f(0,0,0));
  f->mat = kpMat4fMakeIdentity();
}

void kpReframeMakeDQ(KPreframe_t *f, KPdquatf dq) {
  f->dq = dq;
  kpReframeSyncMatrix(f);
}

void kpReframeMakeLookAt(KPvec3f pos, KPvec3f at, KPvec3f up);

void kpReframeSyncMatrix(KPreframe_t *f) {
  f->mat = kpMat4fMakeDquatf(f->dq);
}

KPvec3f kpReframeGetXAxis(KPreframe_t *f) {
  return kpVec3f(f->mat.r[0].x, f->mat.r[1].x, f->mat.r[2].x);
}

KPvec3f kpReframeGetYAxis(KPreframe_t *f) {
  return kpVec3f(f->mat.r[0].y, f->mat.r[1].y, f->mat.r[2].y);
}

KPvec3f kpReframeGetZAxis(KPreframe_t *f) {
  return kpVec3f(f->mat.r[0].z, f->mat.r[1].z, f->mat.r[2].z);
}

KPvec3f kpReframeGetTranslation(KPreframe_t *f) {
  return kpVec3f(f->mat.r[0].w, f->mat.r[1].w, f->mat.r[2].w);
}

KPvec4f kpReframeGetRotation(KPreframe_t *f);

void kpReframeTranslate(KPreframe_t *f, KPvec3f by) {
  f->dq = kpDquatfMul(
    kpDquatfMakeTransform(kpVec3f(0,0,0),0,by), f->dq);
}

void kpReframeRotateAroundSelfX(KPreframe_t *f, KPf32 angle) {
  kpReframeRotateAroundAxis(f, kpVec3f(1,0,0), angle);
}

void kpReframeRotateAroundSelfY(KPreframe_t *f, KPf32 angle) {
  kpReframeRotateAroundAxis(f, kpVec3f(0,1,0), angle);
}

void kpReframeRotateAroundSelfZ(KPreframe_t *f, KPf32 angle) {
  kpReframeRotateAroundAxis(f, kpVec3f(0,0,1), angle);
}

void kpReframeRotateAroundAxis(KPreframe_t *f, KPvec3f axis, KPf32 angle) {
  f->dq = kpDquatfMul(
    kpDquatfMakeTransform(axis, angle, kpVec3f(0,0,0)), f->dq);
}

/******************************************************************************/
/* camera */

typedef enum {
  KP__CameraFlagDirtyView = 0x01,
  KP__CameraFlagDirtyMVP = 0x02
} KP__CameraFlags;

static void kp__CameraUpdate(KPcamera_t *c) {
  if (c->flags_ & KP__CameraFlagDirtyView) {
    c->orient_ = kpMat4fMakeQuatf(c->qorient_);
    c->view_ = kpMat4fMul(c->orient_,
      kpMat4fMakeTranslation(kpVec3fNeg(c->pos_)));
    c->flags_ ^= KP__CameraFlagDirtyView;
    c->flags_ |= KP__CameraFlagDirtyMVP;
  }

  if (c->flags_ & KP__CameraFlagDirtyMVP) {
    c->mvp_ =kpMat4fMul(c->proj_, c->view_);
    c->flags_ ^= KP__CameraFlagDirtyMVP;
  }
}

void kpCameraMakeIdentity(KPcamera_t *c) {
  c->pos_ = kpVec3ff(0);
  c->qorient_ = kpQuatfMakeIdentity();
  c->orient_ = kpMat4fMakeIdentity();
  c->view_ = kpMat4fMakeIdentity();
  c->proj_ = kpMat4fMakeIdentity();
  c->mvp_ = kpMat4fMakeIdentity();
  c->flags_ = 0;
}

void kpCameraSetProjection(KPcamera_t *c,
  KPf32 fov, KPf32 aspect, KPf32 near, KPf32 far)
{
  c->proj_ = kpMat4fMakePerspective(fov, aspect, near, far);
  c->flags_ |= KP__CameraFlagDirtyMVP;
}

void kpCameraSetLookAt(KPcamera_t *c, KPvec3f pos, KPvec3f at, KPvec3f up);

void kpCameraSetPosition(KPcamera_t *c, KPvec3f pos) {
  c->pos_ = pos;
  c->flags_ |= KP__CameraFlagDirtyView;
}

KPmat4f kpCameraGetVP(KPcamera_t *c) {
  kp__CameraUpdate(c);
  return c->mvp_;
}

KPmat4f kpCameraGetP(KPcamera_t *c) {
  kp__CameraUpdate(c);
  return c->proj_;
}

KPmat4f kpCameraGetV(KPcamera_t *c) {
  kp__CameraUpdate(c);
  return c->view_;
}

KPvec3f kpCameraGetX(KPcamera_t *c) {
  kp__CameraUpdate(c);
  return kpVec3f(c->orient_.r[0].x, c->orient_.r[0].y, c->orient_.r[0].z);
}

KPvec3f kpCameraGetY(KPcamera_t *c) {
  kp__CameraUpdate(c);
  return kpVec3f(c->orient_.r[1].x, c->orient_.r[1].y, c->orient_.r[1].z);
}

KPvec3f kpCameraGetZ(KPcamera_t *c) {
  kp__CameraUpdate(c);
  return kpVec3f(c->orient_.r[2].x, c->orient_.r[2].y, c->orient_.r[2].z);
}

void kpCameraMove(KPcamera_t *c, KPvec3f v) {
  c->pos_ = kpVec3fAdd(c->pos_, v);
  c->flags_ |= KP__CameraFlagDirtyView;
}

void kpCameraMoveForward(KPcamera_t *c, KPf32 units) {
  c->pos_ = kpVec3fAdd(c->pos_, kpVec3fMulf(kpCameraGetZ(c), -units));
  c->flags_ |= KP__CameraFlagDirtyView;
}

void kpCameraMoveRight(KPcamera_t *c, KPf32 units) {
  c->pos_ = kpVec3fAdd(c->pos_, kpVec3fMulf(kpCameraGetX(c), -units));
  c->flags_ |= KP__CameraFlagDirtyView;
}

void kpCameraMoveUp(KPcamera_t *c, KPf32 units) {
  c->pos_ = kpVec3fAdd(c->pos_, kpVec3fMulf(kpCameraGetY(c), -units));
  c->flags_ |= KP__CameraFlagDirtyView;
}

void kpCameraRotateHead(KPcamera_t *c, KPf32 angle) {
  c->qorient_ = kpQuatfMul(
    kpQuatfMakeRotation(kpVec3f(0,1,0),angle), c->qorient_);
  c->flags_ |= KP__CameraFlagDirtyView;
}

void kpCameraRotatePitch(KPcamera_t *c, KPf32 angle) {
  c->qorient_ = kpQuatfMul(
    kpQuatfMakeRotation(kpVec3f(1,0,0),angle), c->qorient_);
  c->flags_ |= KP__CameraFlagDirtyView;
}

void kpCameraRotateRoll(KPcamera_t *c, KPf32 angle) {
  c->qorient_ = kpQuatfMul(
    kpQuatfMakeRotation(kpCameraGetZ(c),angle), c->qorient_);
  c->flags_ |= KP__CameraFlagDirtyView;
}
