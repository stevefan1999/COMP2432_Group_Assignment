#include "scheduler/sjf.h"

extern pls_schedule pls_schedule_internal(pls_schedule_info* info);

pls_schedule pls_schedule_sjf_do(pls_schedule_info* info) {
  qsort(info->orders.a, kv_size(info->orders), sizeof(pls_order), pls_order_comparator_by_quantity);

  return pls_schedule_internal(info);
}
