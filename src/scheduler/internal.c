#include "scheduler/common.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

pls_schedule pls_schedule_internal(pls_schedule_info* info) {
  pls_schedule sched = {};
  pls_schedule_init(&sched);

  pls_period* p = &kv_A(info->periods, 0);

  struct plant_s {
    int units;
    const char* taken_by;
  } plants[3] = {
    { .units = 300, .taken_by = NULL },
    { .units = 400, .taken_by = NULL },
    { .units = 500, .taken_by = NULL }
  };

  int end;
  pls_convert_period_to_interval(p, &end);
  for (int i = 0; i < end; i++) {
    for (int j = 0; j < kv_size(info->orders); j++) {
      int* quantity = &kv_A(info->orders, j).quantity;
      if (*quantity > 0) {
        int t2, t3;
        pls_date due_date = kv_A(info->orders, j).due;
        pls_period p2 = { .start = p->start, .end = due_date };
        pls_period p3 = { .start = due_date, .end = p->end };

        if (pls_convert_period_to_interval(&p2, &t2) && pls_convert_period_to_interval(&p3, &t3)) {
          for (int k = 0; k < sizeof plants / sizeof(struct plant_s); k++) {
            struct plant_s* plant = &plants[k];

            if (plant->taken_by == NULL && *quantity > 0) {

              int min_cap_each = MIN(*quantity, plant->units);
              plant->units -= min_cap_each;
              *quantity -= min_cap_each;

              plant->taken_by = kv_A(info->orders, j).order_no;
              kvec_t(pls_schedule_entry) * entry;

              if (k == 0) {
                entry = &sched.plant.x;
              } else if (k == 1) {
                entry = &sched.plant.y;
              } else if (k == 2) {
                entry = &sched.plant.z;
              }

              pls_schedule_entry item = { .identifier = plant->taken_by, .count = min_cap_each, .day = i };
              kv_push(pls_schedule_entry, *entry, item);
            }
          }
        }
      }
    }
  day_over:
    plants[0] = (struct plant_s) { .units = 300, .taken_by = NULL };
    plants[1] = (struct plant_s) { .units = 400, .taken_by = NULL };
    plants[2] = (struct plant_s) { .units = 500, .taken_by = NULL };
  }

  return sched;
}
