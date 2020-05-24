#pragma once

#include <string.h>

#include "date.h"

typedef struct pls_order {
  const char order_no[32];
  pls_date due;
  int quantity;
  const char name[32];
} pls_order;

static int pls_order_comparator_by_date(const void* _a, const void* _b) {
  const pls_order* a = (const pls_order*)_a;
  const pls_order* b = (const pls_order*)_b;

  return pls_date_comparator(&a->due, &b->due);
}

static int pls_order_comparator_by_quantity(const void* _a, const void* _b) {
  const pls_order* a = (const pls_order*)_a;
  const pls_order* b = (const pls_order*)_b;
  if (a->quantity == b->quantity) {
    return pls_date_comparator(&a->due, &b->due);
  }
  return a->quantity > b->quantity ? +1 : -1;
}

static int pls_order_comparator_get_priority(const pls_order* x) {
  return (x->name[sizeof "Product_" - 1] - 'A') % 3;
}

static int pls_order_comparator_by_priority(const void* _a, const void* _b) {
  const pls_order* a = (const pls_order*)_a;
  const pls_order* b = (const pls_order*)_b;

  int a_pr = pls_order_comparator_get_priority(a);
  int b_pr = pls_order_comparator_get_priority(b);

  if (a_pr == b_pr) {
    return pls_date_comparator(&a->due, &b->due);
  }
  return a_pr > b_pr ? +1 : -1;
}