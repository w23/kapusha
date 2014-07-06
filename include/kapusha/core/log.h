#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void kpLogWrite(const char *format, ...);

#define KP_L(...) kpLogWrite(__VA_ARGS__)

#ifdef __cplusplus
} // extern "C"
#endif
