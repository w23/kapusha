#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void kpLog(const char *format, ...);

#define KP_L(...) kpLog(__VA_ARGS__)

#ifdef __cplusplus
} // extern "C"
#endif
