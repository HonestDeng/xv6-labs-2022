#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>

static int nthread = 1;
static int round = 0;

struct barrier {
  pthread_mutex_t barrier_mutex;
  pthread_cond_t barrier_cond;
  int nthread;      // Number of threads that have reached this round of the barrier
  int round;     // Barrier round
} bstate;

static void
barrier_init(void)
{
  assert(pthread_mutex_init(&bstate.barrier_mutex, NULL) == 0);
  assert(pthread_cond_init(&bstate.barrier_cond, NULL) == 0);
  bstate.nthread = 0;
}

static void 
barrier()
{
  // YOUR CODE HERE
  //
  // Block until all threads have called barrier() and
  // then increment bstate.round.
  //
  pthread_mutex_lock(&bstate.barrier_mutex);
  bstate.nthread ++;
  if(bstate.nthread >= nthread){
    // 如果所有线程都已经到达barrier这个点
    bstate.round++;
    bstate.nthread = 0;
    pthread_mutex_unlock(&bstate.barrier_mutex);
    pthread_cond_broadcast(&bstate.barrier_cond);  // 唤醒所有进程
  }else{
    // 在这里等
    // 在wait函数内部会释放barrier_mutex锁
    pthread_cond_wait(&bstate.barrier_cond, &bstate.barrier_mutex);
    // 当等待互斥量为真之后又会重新抢夺barrier_mutex锁，
    // 所以需要使用下面的代码释放锁
    pthread_mutex_unlock(&bstate.barrier_mutex);
  }
}

static void *
thread(void *xa)
{
  long n = (long) xa;
  long delay;
  int i;

  for (i = 0; i < 20000; i++) {
    int t = bstate.round;
    assert (i == t);
    barrier();
    usleep(random() % 100);
  }

  return 0;
}

int
main(int argc, char *argv[])
{
  pthread_t *tha;
  void *value;
  long i;
  double t1, t0;

  if (argc < 2) {
    fprintf(stderr, "%s: %s nthread\n", argv[0], argv[0]);
    exit(-1);
  }
  nthread = atoi(argv[1]);
  tha = malloc(sizeof(pthread_t) * nthread);
  srandom(0);

  barrier_init();

  for(i = 0; i < nthread; i++) {
    assert(pthread_create(&tha[i], NULL, thread, (void *) i) == 0);
  }
  for(i = 0; i < nthread; i++) {
    assert(pthread_join(tha[i], &value) == 0);
  }
  printf("OK; passed\n");
}

/*
pthread_cond_wait:等待条件变量变为真
如何使用：参数mutex互斥量提前锁定，然后该互斥量对条件进行保护，
等待参数1cond条件变量变为真。在等待条件变量变为真的过程中，
此函数一直处于阻塞状态。但是处于阻塞状态的时候，
mutex互斥量被解锁（因为其他线程需要使用到这个锁来使条件变量变为真）
*/