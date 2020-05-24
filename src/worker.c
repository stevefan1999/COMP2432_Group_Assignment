#include "worker.h"

#include <stdio.h>


#include "context.h"
#include "scheduler/fcfs.h"
#include "scheduler/priority.h"
#include "scheduler/sjf.h"

void pls_run(pls_queue_command *cmd) {
  pls_schedule_info info = pls_schedule_info_deserialize_queue_command(cmd);

  pls_schedule (*fn)(pls_schedule_info *) = NULL;

  const char* canon_name = NULL;

  if (strncasecmp(cmd->data.schedule.algorithm, "fcfs", 4) == 0) {
    canon_name = "First Come First Serve";
    fn = pls_schedule_fcfs_do;
  }
  else if (strncasecmp(cmd->data.schedule.algorithm, "sjf", 3) == 0) {
    canon_name = "Shortest Job First";
    fn = pls_schedule_sjf_do;
  }
  else if (strncasecmp(cmd->data.schedule.algorithm, "pr", 2) == 0) {
    canon_name = "Priority";
    fn = pls_schedule_priority_do;
  }
  if (fn != NULL) {
    pls_schedule sched = fn(&info);
    pls_schedule_print(&sched, canon_name, cmd->data.schedule.periods[0], cmd->data.schedule.output_file);
  }

}

bool on_worker_pull_event_from_master(poll_pair fdtab, bool* init) {
  int retpoll = poll(fdtab, sizeof(poll_pair) / sizeof(struct pollfd), 1000);
  if (retpoll >= 0) {
    if (retpoll == 0) {
      return false;
    }
    bool ping_received = false;
    if (fdtab[0].revents & POLLIN) {
      struct pls_queue_command buf;
      size_t n = read(fdtab[0].fd, &buf, sizeof buf);

      switch (buf.cmd) {
      case PING: {
        ping_received = true;
      } break;
      case DO_SCHEDULE: {
        pls_run(&buf);
        ping_received = true;
      } break;
      default: ;
      }
    }

    if (ping_received && fdtab[1].revents & POLLOUT) {
      struct pls_queue_command buf = {
        .cmd = PONG
      };
      write(fdtab[1].fd, &buf, sizeof buf);
    }
  }

  return true;
}

void on_worker(poll_pair fdtab) {
  bool init = true;
  for (;; sleep(1)) {
    on_worker_pull_event_from_master(fdtab, &init);
  }
}