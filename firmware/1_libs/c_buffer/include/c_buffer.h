#pragma once
  #include <stdio.h>                                                            // нужен для NULL
  #include <stdbool.h>
  #include "c_errors_public.h"

  // настройки
    #define BUF_INT_MAX_SYMBOLS_LENGTH   64  // максимальная длина int в символах в этом микроконтроллере
    #define BUF_FLOAT_MAX_SYMBOLS_LENGTH 64  // максимальная длина float в символах..
    #define BUF_FLOAT_MAX_PRECISION      6   // максимально количество знаков после запятой для аргумента функции buf_write_float()

  struct BufHandle
  {
    char*        arr_ptr;      // READ ONLY // указатель на буфер
    unsigned int size_left;    // READ ONLY // сколько символом можно ещё записать ('\0' учитывать не надо, 1 байт под '\0' уже зарезервирован).
    bool         is_empty;     // READ ONLY // буфер пуст?
    unsigned int arr_size_;    // PRIVATE // полный размер буфера
    unsigned int offset_;      // PRIVATE // хранит конец сообщения (индекс , в котором находится '\0')
  };
    
  // инициализация
    bool buf_init       (struct BufHandle* bufHandle_ptr, char* arr_ptr, const unsigned int arr_size);

  // операции
    bool buf_write_char (struct BufHandle* bufHandle_ptr, const char* data_char_ptr);
    bool buf_write_int  (struct BufHandle* bufHandle_ptr, const int   data_int);
    bool buf_write_float(struct BufHandle* bufHandle_ptr, const float data_float, const unsigned short precision);
    bool buf_clear      (struct BufHandle* bufHandle_ptr);