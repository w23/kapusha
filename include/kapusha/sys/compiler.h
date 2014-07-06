#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/* TODO
KP_ATOMIC_INC
KP_ATOMIC_DEC
KP_ATOMIC_CAS

KP_LIKELY
KP_UNLIKELY

KP_ASSERT
*/

#define KP_ASSERT(v)
#define KP_RASSERT(v) \
  if (!(v)) { \
    KP_L("KP_RASSERT(" #v ") failed at %s:%d", __FILE__, __LINE__); \
    kpSysExit(-1); \
  }

#ifdef __cplusplus
} // extern "C"
#endif
