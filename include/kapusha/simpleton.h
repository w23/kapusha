#pragma once

#include "kapusha/app/event.h"

#ifdef __cplusplus
extern "C" {
#endif

void simpleton_init(int argc, char *argv[]);

void simpleton_size(int width, int height);

void simpleton_draw(KP_time_ms time, KP_time_ms next_present);

#ifdef __cplusplus
} /* extern "C" */
#endif
