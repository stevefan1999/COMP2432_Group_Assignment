// comp2432_group_assignment.cpp : Defines the entry point for the application.
//

#include "main.h"
#include "master.h"
#include "worker.h"

static void attach_zombie_reaper() {
  struct sigaction sa = {
    .sa_handler = SIG_DFL,
    .sa_flags = SA_NOCLDWAIT
  };

  sigaction(SIGCHLD, &sa, NULL);
}

void spawn_master_worker_process() {
  struct pollfd fdtab[2] = {
    { .fd = -1, .events = POLLIN },
    { .fd = -1, .events = POLLOUT },
  };

  void (*fn)(poll_pair) = NULL;

  int fd1[2], fd2[2];
  pipe(fd1), pipe(fd2);

  int ret = fork();
  if (ret > 0) {
    close(fd2[0]), close(fd1[1]);
    fdtab[0].fd = fd1[0], fdtab[1].fd = fd2[1];
    fn = on_master;
  } else if (ret == 0) {
    close(fd1[0]), close(fd2[1]);
    fdtab[0].fd = fd2[0], fdtab[1].fd = fd1[1];
    fn = on_worker;
  }
  fn(fdtab);
}

int main() {
  attach_zombie_reaper();

  spawn_master_worker_process();
  return 0;
}
