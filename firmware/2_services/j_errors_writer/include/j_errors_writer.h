#pragma once
  #include "j_buffer.h"
  #include "j_errors.h"
  #include <stdbool.h>
  #include <string.h>                                                           // для strlen()

  /*
    1. Хранит в себе структуру и массив буфера для ошибок
    2. Записывает в лог текущие ошибки.
    3. Записывает в лог ошибки за всё время.
    4. Возвращает структуру со стейтом и указателем на массив.
  */

  struct ErrorsBufferInfo
  {
    char *arr_ptr;
    bool is_errors_writer_written_anything;
  };

  void write_unhandled_errors_to_buffer(struct ErrorsBufferInfo *errorsBufferInfo);
  void write_all_errors_to_buffer(struct ErrorsBufferInfo *errorsBufferInfo);