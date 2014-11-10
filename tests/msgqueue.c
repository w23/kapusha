#include <stdlib.h>
#include <stdio.h>
#include <kapusha/core.h>

#include <unistd.h>

void kpSysAbort() {
  abort();
}

void kp__SysLogOutput(const char *s) {
  printf("%s\n", s);
}

void *consumer(void *arg) {
  KPmessage_queue_t queue = (KPmessage_queue_t)arg;
  KPu32 last_tag = 0;
  for (;;) {
    KPmessage_t *msg = kpMessageQueueGet(queue, 1000);
    KP_ASSERT(msg);
    if (msg->tag == 0)
      break;
    KP_ASSERT(msg->tag == last_tag+1);
    last_tag = msg->tag;
    kpMessageDiscard(msg);
  }
  return 0;
}

int main(int argc, char *argv[]) {
  KP_UNUSED(argc);
  KP_UNUSED(argv);
  KPmessage_queue_t queue = kpMessageQueueCreate();
  KPthread_params_t tp;
  tp.thread_func = consumer;
  tp.user_data = queue;
  tp.name = "test";
  KPthread_t thread = kpThreadSpawn(&tp);

  KPu32 i;
  for (i = 1; i < 1000000; ++i) {
    kpMessageQueuePut(queue, i, 0, 0, 0);
  }

  kpMessageQueuePut(queue, 0, 0, 0, 0);

  kpThreadJoin(thread);
  kpMessageQueueDestroy(queue);

  printf("%s: OK\n", argv[0]);
  return 0;
}
