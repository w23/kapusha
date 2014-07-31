#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef float KPscalarf;

typedef struct { int x, y; } KPvec2i;
typedef struct { float x, y; } KPvec2f;
typedef struct { float x, y, z; } KPvec3f;
typedef struct { float x, y, z, w; } KPvec4f;

typedef struct { KPvec4f rows[4]; } KPmat4f;

typedef struct { KPvec2i bl, tr; } KPrect2i;

static inline KPvec2i kpVec2i(int x, int y) {
  KPvec2i v = { x, y };
  return v;
}

static inline KPvec3f kpVec3f(float x, float y, float z) {
  KPvec3f v = { x, y, z };
  return v;
}

static inline KPvec4f kpVec4f(float x, float y, float z, float w) {
  KPvec4f v = { x, y, z, w };
  return v;
}

#ifdef __cplusplus
} // extern "C"
#endif
