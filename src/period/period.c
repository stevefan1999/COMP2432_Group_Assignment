#include "data/period.h"

// int month_extra[12] = { 1,0,1,0,1,0,1,1,0,1,0,1 };

bool pls_convert_period_to_interval(pls_period* self, int* out_max) {
  if (self->start.year > self->end.year) {
    return false;
  }
  if (self->start.month > self->end.month) {
    return false;
  }
  if (self->start.day > self->end.day) {
    return false;
  }

  if (out_max) {
    int year_delta = self->end.year - self->start.year;
    int month_delta = self->end.month - self->start.month;
    int months = year_delta * 12 + month_delta;
    *out_max = months * 30 + (self->end.day - self->start.day);
    /*
    for (int i = self->start.month; i < months * 12; i++) {
      *out_max += month_extra[(i - 1) % 12];
    }
    */

    return true;
  }

  return false;
}
