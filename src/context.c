#include "context.h"

#include "parser/input.parse.h"

#include "parser/input.lex.h"


#include "data/order.h"

void pls_context_init(pls_context_t* self) {
  kv_init(self->info.orders);
  kv_init(self->info.periods);
  self->should_stop = false;
}

void pls_context_add_period(pls_context_t* self, pls_period period) {
  kv_push(pls_period, self->info.periods, period);
}

void pls_context_add_order(pls_context_t* self, pls_order order) {
  kv_pushp(pls_order, self->info.orders);
  memcpy(&kv_A(self->info.orders, kv_size(self->info.orders) - 1), &order, sizeof(pls_order));

}

void pls_context_add_batch(pls_context_t* self, const char* file_name) {
  FILE* fp;
  if ((fp = fopen(file_name, "r")) != NULL) {
    yyscan_t yyscanner;
    yylex_init_extra(self, &yyscanner);
    yyrestart(fp, yyscanner);
    while (yyparse(yyscanner) == 0) {}
    yylex_destroy(yyscanner);
    fclose(fp);
  }
}

void pls_context_run_pls(pls_context_t* self, const char* algo, const char* output_file) {
  extern struct pls_queue_command next_cmd;

  struct pls_queue_command buf = pls_schedule_info_serialize_queue_command(&self->info, output_file);

  memcpy(&buf.data.schedule.algorithm, algo, sizeof(buf.data.schedule.algorithm));
  memcpy(&next_cmd, &buf, sizeof(pls_queue_command));
}

void pls_context_exit_pls(pls_context_t* self) {
  self->should_stop = true;
}

void pls_context_destroy(pls_context_t* self) {
  kv_destroy(self->info.orders);
  kv_destroy(self->info.periods);
}
