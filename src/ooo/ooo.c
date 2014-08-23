#include "kapusha/ooo.h"

void kpReframeMakeIdentity(KPreframe_t *f) {
  f->dq = kpDquatfRotationTranslation(kpVec3f(0,0,0),0, kpVec3f(0,0,0));
  f->mat = kpMat4fIdentity();
}

void kpReframeMakeDQ(KPreframe_t *f, KPdquatf dq) {
  f->dq = dq;
  kpReframeSyncMatrix(f);
}

void kpReframeMakeLookAt(KPvec3f pos, KPvec3f at, KPvec3f up);

void kpReframeSyncMatrix(KPreframe_t *f) {
  f->mat = kpMat4fdq(f->dq);
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
  f->dq = kpDquatfMuldq(
    kpDquatfRotationTranslation(kpVec3f(0,0,0),0,by), f->dq);
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
  f->dq = kpDquatfMuldq(
    kpDquatfRotationTranslation(axis, angle, kpVec3f(0,0,0)), f->dq);
}
