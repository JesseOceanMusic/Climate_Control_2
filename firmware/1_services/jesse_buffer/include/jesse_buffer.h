#pragma once
  #include <stdio.h>                                                            // нужен для NULL
  #include <stdbool.h>
  
  struct CharBuffer
  {
    char *arr_ptr;
    int arr_size;
    int offset;                                                                 // int по тому, что snprintf возвращает int
  };

  typedef enum
  {
    BUFFER_SUCCESS,
    BUFFER_ERROR_NULL,
    BUFFER_ERROR_OVERFILL,
  }BufferStatus;

  BufferStatus buffer_write_char(struct CharBuffer *struct_ptr, const char *data);

  BufferStatus buffer_write_int(struct CharBuffer *struct_ptr, int var);

  BufferStatus buffer_write_float(struct CharBuffer *struct_ptr, float var);

  BufferStatus buffer_clear(struct CharBuffer *struct_ptr);

  bool is_buffer_empty(struct CharBuffer *struct_ptr);

  int buffer_size_left(struct CharBuffer *struct_ptr);