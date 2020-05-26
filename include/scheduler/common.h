#pragma once

#include <kvec.h>
#include <stdio.h>

#include "data/order.h"
#include "data/period.h"

#include "queue/command.h"

typedef struct pls_schedule_entry {
  const char* identifier;
  int count;
  int day;
} pls_schedule_entry;

typedef struct pls_schedule_info {
  kvec_t(pls_period) periods;
  kvec_t(pls_order) orders;
} pls_schedule_info;

static void pls_schedule_info_init(pls_schedule_info* self) {
  kv_init(self->periods);
  kv_init(self->orders);
}

static void pls_schedule_info_destroy(pls_schedule_info* self) {
  kv_destroy(self->periods);
  kv_destroy(self->orders);
}

typedef struct {
  struct {
    kvec_t(pls_schedule_entry) x, y, z;
  } plant;
} pls_schedule;

static void pls_schedule_init(pls_schedule* self) {
  kv_init(self->plant.x);
  kv_init(self->plant.y);
  kv_init(self->plant.z);
}

static void pls_schedule_print(pls_schedule* self, const char* algo, pls_period period, const char* output_file) {

  int total_days;
  pls_convert_period_to_interval(&period, &total_days);

  FILE* fp = NULL;
  if (output_file != NULL) {
    fp = fopen(output_file, "a+");
    if (fp != NULL) {
      fclose(fp);
      fp = fopen(output_file, "w+");
    } else {
      // could not create the file, set it to null
      fp = NULL;
    }
  }

  if (fp == NULL) {
    fp = stdout;
  }

  fprintf(fp, "***PLS Schedule Analysis Report***\n\n");
  fprintf(fp, "Algorithm used: %s\n\n", algo);

  int accepted = kv_size(self->plant.x) + kv_size(self->plant.y) + kv_size(self->plant.z);
  fprintf(fp, "There are %d Orders ACCEPTED. Details are as follows:\n", accepted);

  fprintf(fp, "\nFactory X:\n");
  fprintf(fp, "%-16s %-16s %-16s\n", "ORDER NUMBER", "DATE", "QUANTITY");

  const int paddings = 16 * 3;
  char padding_buf[paddings + 1];
  memset(padding_buf, '=', paddings);
  padding_buf[paddings] = '\0';

  char date_buf[32] = {};
  fprintf(fp, "%s\n", padding_buf);

  int accum = 0;

  int sum = 0;
  for (int i = 0; i < kv_size(self->plant.x); i++) {
    pls_date_format(pls_date_add_days(period.start, kv_A(self->plant.x, i).day), date_buf, sizeof date_buf);
    fprintf(fp, "%-16s %-16s %-16d\n", kv_A(self->plant.x, i).identifier, date_buf, kv_A(self->plant.x, i).count);
    sum += kv_A(self->plant.x, i).count;
  }

  fprintf(fp, "Days in use: %d days\n", kv_size(self->plant.x));
  fprintf(fp, "Products produced: %d in total\n", sum);
  fprintf(fp, "Utilization of the plant: %g%%\n", 100.0 * kv_size(self->plant.x) / total_days * 1.0);

  fprintf(fp, "\nFactory Y:\n");
  fprintf(fp, "%-16s %-16s %-16s\n", "ORDER NUMBER", "DATE", "QUANTITY");
  fprintf(fp, "%s\n", padding_buf);
  accum += sum;

  sum = 0;
  for (int i = 0; i < kv_size(self->plant.y); i++) {
    pls_date_format(pls_date_add_days(period.start, kv_A(self->plant.y, i).day), date_buf, sizeof date_buf);
    fprintf(fp, "%-16s %-16s %-16d\n", kv_A(self->plant.y, i).identifier, date_buf, kv_A(self->plant.y, i).count);
    sum += kv_A(self->plant.y, i).count;
  }

  fprintf(fp, "Days in use: %d days\n", kv_size(self->plant.y));
  fprintf(fp, "Products produced: %d in total\n", sum);
  fprintf(fp, "Utilization of the plant: %g%%\n", 100.0 * kv_size(self->plant.y) / total_days * 1.0);
  accum += sum;

  sum = 0;
  fprintf(fp, "\nFactory Z:\n");
  fprintf(fp, "%-16s %-16s %-16s\n", "ORDER NUMBER", "DATE", "QUANTITY");
  fprintf(fp, "%s\n", padding_buf);
  for (int i = 0; i < kv_size(self->plant.z); i++) {
    pls_date_format(pls_date_add_days(period.start, kv_A(self->plant.z, i).day), date_buf, sizeof date_buf);
    fprintf(fp, "%-16s %-16s %-16d\n", kv_A(self->plant.z, i).identifier, date_buf, kv_A(self->plant.z, i).count);
    sum += kv_A(self->plant.z, i).count;
  }
  accum += sum;

  fprintf(fp, "Days in use: %d days\n", kv_size(self->plant.z));
  fprintf(fp, "Products produced: %d in total\n", sum);
  fprintf(fp, "Utilization of the plant: %g%%\n", 100.0 * kv_size(self->plant.z) / total_days * 1.0);
  fprintf(fp, "Overall utilization: %g%%\n", 100.0 * accum / (300 * kv_size(self->plant.x) + 400 * kv_size(self->plant.y) + 500 * kv_size(self->plant.z)) * 1.0);

  if (fp != stdout) {
    fclose(fp);
  }
}

static void pls_schedule_destroy(pls_schedule* self) {
  kv_destroy(self->plant.x);
  kv_destroy(self->plant.y);
  kv_destroy(self->plant.z);
}

static pls_queue_command pls_schedule_info_serialize_queue_command(pls_schedule_info* info, const char* output_file) {
  pls_queue_command out = {
    .cmd = DO_SCHEDULE,
    .data = {
        .schedule = (struct schedule_t) {
            .periods = kv_A(info->periods, 0),
            .n_periods = kv_size(info->periods),
            .n_orders = kv_size(info->orders),
        } }
  };

  memset(out.data.schedule.output_file, 0, sizeof out.data.schedule.output_file);
  memcpy(out.data.schedule.output_file, output_file, strlen(output_file));
  memcpy(out.data.schedule.periods, info->periods.a, sizeof(pls_period) * out.data.schedule.n_periods);
  memcpy(out.data.schedule.orders, info->orders.a, sizeof(pls_order) * out.data.schedule.n_orders);

  return out;
}

static pls_schedule_info pls_schedule_info_deserialize_queue_command(pls_queue_command* cmd) {
  pls_schedule_info info;
  pls_schedule_info_init(&info);

  for (int i = 0; i < cmd->data.schedule.n_periods; i++) {
    pls_period data = cmd->data.schedule.periods[i];
    kv_push(pls_period, info.periods, data);
  }

  for (int i = 0; i < cmd->data.schedule.n_orders; i++) {
    kv_a(pls_order, info.orders, i);
    memcpy(&kv_A(info.orders, i), &cmd->data.schedule.orders[i], sizeof(pls_order));
  }

  return info;
}