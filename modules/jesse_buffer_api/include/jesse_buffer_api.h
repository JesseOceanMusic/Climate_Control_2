#pragma once
  #include "jesse_buffer.h"
  #include "jesse_errors.h"
  #include <stdio.h>                                                            // нужен для NULL
  
  void buffer_write_char_and_error_check(struct CharBuffer *struct_ptr, const char *data);
  void buffer_write_int_and_error_check(struct CharBuffer *struct_ptr, int var);
  void buffer_write_float_and_error_check(struct CharBuffer *struct_ptr, float var);