#pragma once
#include "main.h"

bool on_worker_pull_event_from_master(poll_pair fdtab, bool* init);
void on_worker(poll_pair fdtab);