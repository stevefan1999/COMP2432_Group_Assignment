#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct pls_date {
  int year;
  int month;
  int day;
} pls_date;

static int pls_date_comparator(const void* _a, const void* _b) {
  const pls_date* a = (const pls_date*)_a;
  const pls_date* b = (const pls_date*)_b;

  if (a->year == b->year) {
    if (a->month == b->month) {
      if (a->day == b->day) {
        return 0;
      }

      return a->day > b->day ? +1 : -1;
    }
    return a->month > b->month ? +1 : -1;
  }

  return a->year > b->year ? +1 : -1;
}

#define MAX(a, b) ((a) > (b) ? (a) : (b))

static pls_date pls_date_add_days(pls_date d, int days) {
  pls_date out = d;
  out.day += days;
  out.month += out.day / (30 + 1);
  out.year += out.month / (12 + 1);
  out.month = MAX(1, out.month % (12 + 1));
  out.day = MAX(1, out.day % (30 + 1));

  return out;
}

static void pls_date_format(pls_date d, char* buf, size_t n) {
  snprintf(buf, n, "%d-%02d-%02d", d.year, d.month, d.day);
}