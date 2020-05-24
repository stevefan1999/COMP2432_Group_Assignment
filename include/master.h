#pragma once

#include "main.h"

bool on_master_pull_status_from_worker(poll_pair fdtab, bool* init);
void on_master(poll_pair fdtab);

enum state_t {
  INIT,
  READ_LINE,
  PROCESS_INPUT,
  ASK_COMMAND,
  EXIT
};