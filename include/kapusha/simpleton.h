#pragma once

#include "sys.h"

#ifdef __cplusplus
extern "C" {
#endif

void simpleton_init(int argc, const char *argv[]);

void simpleton_size(int width, int height);

void simpleton_draw(KPtime_ms pts);

#ifdef __cplusplus
} /* extern "C" */
#endif
