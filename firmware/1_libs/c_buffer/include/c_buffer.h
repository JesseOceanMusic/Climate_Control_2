#pragma once
  #include <stdio.h>                                                            // нужен для NULL
  #include <stdbool.h>
  #include "c_errors.h"

  #define BUF_INT_MAX_SYMBOLS_LENGTH   64  // максимальная длина int в символах в этом микроконтроллере
  #define BUF_FLOAT_MAX_SYMBOLS_LENGTH 64  // максимальная длина float в символах..
  #define BUF_FLOAT_MAX_PRECISION      6   // максимально количество знаков после запятой для аргумента

  struct BufInfo
  {
    char*        arr_ptr;      // указатель на буфер
    unsigned int arr_size;     // полный размер буфера
    unsigned int offset;       // хранит конец сообщения (индекс , в котором находится '\0')
    unsigned int size_left;    // сколько символом можно ещё записать ('\0' учитывать не надо, 1 байт под '\0' уже зарезервирован).
    bool         is_empty;     // буфер пуст?
  };

  bool buf_init       (struct BufInfo *struct_ptr, char* arr_ptr, unsigned int arr_size);
  bool buf_write_char (struct BufInfo *struct_ptr, const char* data_char);
  bool buf_write_int  (struct BufInfo *struct_ptr, int         data_int);
  bool buf_write_float(struct BufInfo *struct_ptr, float       data_float, unsigned short precision);
  bool buf_clear      (struct BufInfo *struct_ptr);