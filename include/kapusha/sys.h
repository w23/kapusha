#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/* types */
typedef unsigned char KPu8;
typedef signed char KPs8;
typedef unsigned short KPu16;
typedef signed short KPs16;
typedef unsigned int KPu32;
typedef signed int KPs32;
typedef unsigned long long KPu64;
typedef signed long long KPs64;
typedef float KPf32;
typedef double KPf64;

typedef uintptr_t KPuptr;
typedef KPu64 KPsize;

typedef KPu64 KPtime_ns;
typedef KPu32 KPtime_ms;

/******************************************************************************/
/* threading and sync */

#ifdef KP_OS_MACH
#include <libkern/OSAtomic.h>
typedef volatile int32_t KPs32_atomic;
inline static KPs32 kpS32AtomicRead(KPs32_atomic* v) { return *v; }
inline static KPs32 kpS32AtomicInc(KPs32_atomic* v) {
  return OSAtomicIncrement32(v); }
inline static KPs32 kpS32AtomicDec(KPs32_atomic* v) {
  return OSAtomicDecrement32(v); }
#elif defined(KP_GCC_ATOMICS)
typedef volatile int KPs32_atomic;
inline static KPs32 kpS32AtomicRead(KPs32_atomic* v) {
  return __sync_fetch_and_add(v, 0); }
inline static KPs32 kpS32AtomicInc(KPs32_atomic* v) {
  return __sync_fetch_and_add(v, 1); }
inline static KPs32 kpS32AtomicDec(KPs32_atomic* v) {
  return __sync_fetch_and_sub(v, 1); }
#else
#error define os/compiler-specifics for atomic ops
#endif

/******************************************************************************/
/* assert */

#define KP_ASSERT(v)
#define KP_RASSERT(v) \
  if (!(v)) { \
    KP_L("KP_RASSERT(" #v ") failed at %s:%d", __FILE__, __LINE__); \
    kpSysExit(-1); \
  }

#define KP_FAIL(...)

#define KP_UNUSED(v) (void)(v);

/* Kill current process */
void kpSysExit(int code);

/* Get current precise monotonic system time in nanoseconds */
KPtime_ns kpSysTimeNs();

/* System-specific way to output debug log messages */
void kp__LogOutput(const char *output);

#ifdef __cplusplus
} // extern "C"
#endif
