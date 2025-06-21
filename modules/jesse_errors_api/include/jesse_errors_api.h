#pragma once
  #include "jesse_buffer.h"
  #include "jesse_errors.h"
  #include <stdbool.h>

  void print_errors_log_to_buffer (struct CharBuffer *struct_ptr);
  bool tick_ERRORS_handler (struct CharBuffer *struct_ptr);