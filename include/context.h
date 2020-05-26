#pragma once

#include "main.h"

#include "data/period.h"
#include "data/order.h"

#include <kvec.h>

#include "scheduler/common.h"

typedef struct pls_context_t {
  pls_schedule_info info;
  poll_pair child;
  bool should_stop;
} pls_context_t;

void pls_context_init(pls_context_t* self);

void pls_context_add_period(pls_context_t *self, pls_period period);
void pls_context_add_order(pls_context_t* self, pls_order order);
void pls_context_add_batch(pls_context_t* self, const char *file_name);
void pls_context_run_pls(pls_context_t* self, const char* algo, const char* output_file);
void pls_context_exit_pls(pls_context_t* self);

void pls_context_destroy(pls_context_t* self);
