#include "master.h"


#include "data/period.h"
#include "parser/input.parse.h"
#include "parser/input.lex.h"

struct pls_queue_command next_cmd;

bool on_master_pull_status_from_worker(poll_pair fdtab, bool* init) {
  int retpoll = poll(fdtab, sizeof(poll_pair) / sizeof(struct pollfd), 1);
  if (retpoll >= 0) {
    if (retpoll == 0) {
      return false;
    }

    bool pong_received = false;

    if (fdtab[0].revents & POLLIN) {

      struct pls_queue_command buf;
      size_t n = read(fdtab[0].fd, &buf, sizeof buf);

      switch (buf.cmd) {
      case PONG: {
        pong_received = true;
      } break;
      default: ;
      }
    }

    if ((pong_received || *init) && fdtab[1].revents & POLLOUT) {
      write(fdtab[1].fd, &next_cmd, sizeof next_cmd);

      next_cmd.cmd = PING;
      memset(&next_cmd.data, 0, sizeof(pls_queue_command));

      if (init) *init = false;
    }
  }

  return true;
}

void on_master(poll_pair fdtab) {
  struct pollfd stdfd[2] = {
    { .fd = STDIN_FILENO, .events = POLLIN },
    { .fd = STDOUT_FILENO, .events = POLLOUT },
  };

  bool init = true;
  enum state_t state = INIT;

  char buf[512];

  pls_context_t context = {
    .child = fdtab
  };
  yyscan_t yyscanner;

  for (int retpoll; (retpoll = poll(stdfd, sizeof(poll_pair) / sizeof(struct pollfd), 1000)) >= 0;) {
    if (retpoll == 0) {
      continue;
    }

    switch (state) {
    case INIT: {
      if (stdfd[1].revents & POLLOUT) {
        pls_context_init(&context);
        yylex_init_extra(&context, &yyscanner);
        printf("~~WELCOME TO PLS~~\n");
        fflush(stdout);
        state = ASK_COMMAND;
      }
    } break;
    case ASK_COMMAND: {
      if (stdfd[1].revents & POLLOUT) {
        printf("Please enter:\n> ");
        fflush(stdout);
        state = READ_LINE;
      }
    } break;
    case READ_LINE: {
      if (stdfd[0].revents & POLLIN) {
        size_t n = read(stdfd[0].fd, buf, sizeof buf);
        buf[n] = '\0';
        fflush(stdin);
        state = PROCESS_INPUT;
      }
    } break;
    case PROCESS_INPUT: {
      YY_BUFFER_STATE yybuf = yy_scan_string(buf, yyscanner);

      if (yyparse(yyscanner) == 0) {
        printf("OK: %s\n", buf);
      }
      yy_delete_buffer(yybuf, yyscanner);

      state = ASK_COMMAND;
    } break;
    case EXIT: {
      yylex_destroy(yyscanner);
      pls_context_destroy(&context);
    } break;
    default:;
    }

    on_master_pull_status_from_worker(fdtab, &init);

  }
}