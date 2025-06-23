#pragma once
  #include <stdio.h>                                                            // нужен для NULL
  #include <stdbool.h>
  #include "j_errors.h"

  struct CharBuffer
  {
    char *arr_ptr;
    unsigned int arr_size;
    unsigned int offset;
  };
  
  extern const char* ARRAY_IS_FULL_FOOTER_MSG;                                  // нужен в глобале для тестов                         

  bool buffer_write_char       (struct CharBuffer *struct_ptr, const char *data);
  bool buffer_write_int        (struct CharBuffer *struct_ptr, int var);
  bool buffer_write_float      (struct CharBuffer *struct_ptr, float var);
  bool is_buffer_empty         (struct CharBuffer *struct_ptr);
  unsigned int buffer_size_left(struct CharBuffer *struct_ptr);
  bool buffer_clear            (struct CharBuffer *struct_ptr);