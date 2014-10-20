#include "kapusha/ooo.h"

/******************************************************************************/
/* reframe */

typedef enum {
  KP__ReframeFlagDirtyMat = 0x01,
} KP__ReframeFlags;

static void kp__ReframeUpdate(KPreframe_t *this) {
  if (this->flags_ & KP__ReframeFlagDirtyMat) {
    this->mat_ = kpMat4fMakeDquatf(this->dq_);
    this->flags_ ^= KP__ReframeFlagDirtyMat;
  }
}

void kpReframeMakeIdentity(KPreframe_t *this) {
  this->dq_ = kpDquatfMakeTransform(kpVec3f(0,0,0),0, kpVec3f(0,0,0));
  this->mat_ = kpMat4fMakeIdentity();
  this->flags_ = 0;
}

void kpReframeMakeDquatf(KPreframe_t *this, KPdquatf dq) {
  this->dq_ = dq;
  this->flags_ |= KP__ReframeFlagDirtyMat;
}

void kpReframeMakeTransform(KPreframe_t *this,
  KPvec3f axis, KPf32 angle, KPvec3f translation)
{
  kpReframeMakeDquatf(this,
    kpDquatfMakeTransform(axis, angle, translation));
}

void kpReframeMakeRotation(KPreframe_t *this,
  KPvec3f axis, KPf32 angle)
{
  kpReframeMakeTransform(this, axis, angle, kpVec3f(0,0,0));
}

/* \todo void kpReframeMakeLookAt(KPvec3f pos, KPvec3f at, KPvec3f up); */

const KPmat4f *kpReframeGetMatrix(KPreframe_t *this) {
  kp__ReframeUpdate(this);
  return &this->mat_;
}

KPvec3f kpReframeGetXAxis(KPreframe_t *this) {
  kp__ReframeUpdate(this);
  return kpVec3f(this->mat_.r[0].x, this->mat_.r[1].x, this->mat_.r[2].x);
}

KPvec3f kpReframeGetYAxis(KPreframe_t *this) {
  kp__ReframeUpdate(this);
  return kpVec3f(this->mat_.r[0].y, this->mat_.r[1].y, this->mat_.r[2].y);
}

KPvec3f kpReframeGetZAxis(KPreframe_t *this) {
  kp__ReframeUpdate(this);
  return kpVec3f(this->mat_.r[0].z, this->mat_.r[1].z, this->mat_.r[2].z);
}

KPvec3f kpReframeGetTranslation(KPreframe_t *this) {
  kp__ReframeUpdate(this);
  return kpVec3f(this->mat_.r[0].w, this->mat_.r[1].w, this->mat_.r[2].w);
}

void kpReframeTranslate(KPreframe_t *this, KPvec3f by) {
  this->dq_ = kpDquatfMul(
    kpDquatfMakeTransform(kpVec3f(0,0,0),0,by), this->dq_);
  this->flags_ |= KP__ReframeFlagDirtyMat;
}

void kpReframeRotateAroundSelfX(KPreframe_t *this, KPf32 angle) {
  kpReframeRotateAroundAxis(this, kpVec3f(1,0,0), angle);
}

void kpReframeRotateAroundSelfY(KPreframe_t *this, KPf32 angle) {
  kpReframeRotateAroundAxis(this, kpVec3f(0,1,0), angle);
}

void kpReframeRotateAroundSelfZ(KPreframe_t *this, KPf32 angle) {
  kpReframeRotateAroundAxis(this, kpVec3f(0,0,1), angle);
}

void kpReframeRotateAroundAxis(KPreframe_t *this, KPvec3f axis, KPf32 angle) {
  this->dq_ = kpDquatfMul(
    kpDquatfMakeTransform(axis, angle, kpVec3f(0,0,0)), this->dq_);
  this->flags_ |= KP__ReframeFlagDirtyMat;
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
