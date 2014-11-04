#include <kapusha/sys.h>
#include <kapusha/core.h>

KPthread_t kpThreadSpawn(const KPthread_params_t *params) {
  pthread_t thread;
  KP_ASSERT(params->thread_func != 0);

  int result = pthread_create(&thread, 0, params->thread_func, params->user_data);
  KP_ASSERT(result == 0);
  return thread;
}

void kpThreadJoin(KPthread_t thread) {
  pthread_join(thread, 0);
}

void kpMutexInit(KPmutex_t *mutex) {
  KP_ASSERT(mutex != 0);

  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);
  //pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
 // pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST);

  pthread_mutex_init(mutex, &attr);
  pthread_mutexattr_destroy(&attr);
}

void kpMutexDestroy(KPmutex_t *mutex) {
  KP_ASSERT(mutex != 0);
  pthread_mutex_destroy(mutex);
}

void kpMutexLock(KPmutex_t *mutex) {
  KP_ASSERT(mutex != 0);
  pthread_mutex_lock(mutex);
}

void kpMutexUnlock(KPmutex_t *mutex) {
  KP_ASSERT(mutex != 0);
  pthread_mutex_unlock(mutex);
}

void kpCondvarInit(KPcondvar_t *condvar) {
  KP_ASSERT(condvar != 0);
  pthread_cond_init(condvar, 0);
}

void kpCondvarDestroy(KPcondvar_t *condvar) {
  KP_ASSERT(condvar != 0);
  pthread_cond_destroy(condvar);
}

void kpCondvarWait(KPcondvar_t *condvar, KPmutex_t *mutex) {
  KP_ASSERT(condvar != 0);
  KP_ASSERT(mutex != 0);
  pthread_cond_wait(condvar, mutex);
}

void kpCondvarSignal(KPcondvar_t *condvar) {
  KP_ASSERT(condvar != 0);
  pthread_cond_broadcast(condvar);
}
