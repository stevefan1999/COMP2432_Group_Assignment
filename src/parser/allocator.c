#include <parser/input.parse.h>
#include <parser/input.lex.h>

void* yyalloc(size_t bytes, void* scanner) {
  return malloc(bytes);
}

void* yyrealloc(void* ptr, size_t bytes, void* scanner) {
  return realloc(ptr, bytes);
}

void yyfree(void* ptr, void* scanner) {
  free(ptr);
}