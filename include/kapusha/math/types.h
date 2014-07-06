#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  int x, y;
} KPvec2i;

static inline KPvec2i kpVec2i(int x, int y) {
  KPvec2i v = { x, y };
  return v;
}

#ifdef __cplusplus
} // extern "C"
#endif
