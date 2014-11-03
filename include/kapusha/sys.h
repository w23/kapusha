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

typedef void *(*KPthread_f)(void *user_data);

typedef struct {
  const char *name;
  void *user_data;
  KPthread_f thread_func;
} KPthread_params_t;

#ifdef KP_OS_POSIX
#include <pthread.h>
typedef pthread_mutex_t KPmutex_t;
typedef pthread_cond_t KPcondvar_t;
#else
#error define os-specifics for threading
#endif

void kpThreadSpawn(const KPthread_params_t *params);

void kpMutexInit(KPmutex_t *mutex);
void kpMutexDestroy(KPmutex_t *mutex);
void kpMutexLock(KPmutex_t *mutex);
void kpMutexUnlock(KPmutex_t *mutex);

void kpCondvarInit(KPcondvar_t *condvar);
void kpCondvarDestroy(KPcondvar_t *condvar);
void kpCondvarWait(KPcondvar_t *condvar, KPmutex_t *mutex);
void kpCondvarSignal(KPcondvar_t *condvar);

/******************************************************************************/
/* assert */

#define KP_UNUSED(v) (void)(v);

#define KP_RASSERT(v) \
  if (!(v)) { \
    KP_L("KP_RASSERT(" #v ") failed at %s:%d", __FILE__, __LINE__); \
    kpSysExit(-1); \
  }

#ifdef KP_SIZECODING
#define KP_ASSERT(v)
#define KP_FAIL(...) kpSysExit(-1)
#else
#define KP_ASSERT(v) KP_RASSERT(v)
#define KP_FAIL(...) \
  { \
    KP_L("KP_FAIL: " __VA_ARGS__); \
    kpSysExit(-1); \
  }
#endif

/* Kill current process */
void kpSysExit(int code);

/* Get current precise monotonic system time in nanoseconds */
KPtime_ns kpSysTimeNs();

/* System-specific way to output debug log messages */
void kp__LogOutput(const char *output);

#ifdef __cplusplus
} // extern "C"
#endif
