#pragma once

#include "date.h"

typedef struct pls_period {
  pls_date start;
  pls_date end;
} pls_period;

bool pls_convert_period_to_interval(pls_period* self, int *out_max);
