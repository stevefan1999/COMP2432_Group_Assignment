#pragma once

#include <stdbool.h>
#include <string.h>

#include <poll.h>
#include <signal.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

typedef struct pollfd poll_pair[2];

void spawn_master_worker_process();