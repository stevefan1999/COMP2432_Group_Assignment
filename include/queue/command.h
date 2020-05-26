#pragma once

#include "data/order.h"
#include "data/period.h"

enum pls_queue_command_t {
  PING,
  PONG,
  DO_SCHEDULE
};

typedef struct pls_queue_command {
  enum pls_queue_command_t cmd;
  union {
    struct {

    } ping;
    struct {

    } pong;
    struct schedule_t {
      const char algorithm[4];
      const char output_file[128];
      int n_periods;
      int n_orders;
      pls_period periods[1];
      pls_order orders[1024];
    } schedule;
  } data;
} pls_queue_command;