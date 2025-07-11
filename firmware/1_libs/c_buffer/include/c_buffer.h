#pragma once
  #include <stdbool.h>
  #include "c_errors_public.h"

  // настройки
    #define BUF_INT_MAX_SYMBOLS_LENGTH   64                                     // максимальная длина int в символах в этом микроконтроллере
    #define BUF_FLOAT_MAX_SYMBOLS_LENGTH 64                                     // максимальная длина float в символах..
    #define BUF_FLOAT_MAX_PRECISION      6                                      // максимально количество знаков после запятой для аргумента функции buf_write_float()

    extern const char* BUF_BAD_ID;                                              // для buf_get_arr_ptr()

  // буферы
    typedef enum                                                                // перечисление id буферов
    {
      #define BUFFER_LIST(buffer_id, buffer_name, buffer_size) buffer_id,       // X-MACROS
      #include "c_buffer_list.def"
      #undef  BUFFER_LIST
      BUF_ID__AMOUNT,                                                           // ДОЛЖЕН БЫТЬ ПОСЛЕДНИМ! // Количество Ошибок
    }BufId;

  // хелперы
    bool buf_is_id_correct     (BufId bufId);
    bool buf_will_this_data_fit(BufId bufId, const char* data_char_ptr);
  
  // операции
    bool buf_write_char (BufId bufId, const char* data_char_ptr);
    bool buf_write_int  (BufId bufId, const int   data_int);
    bool buf_write_float(BufId bufId, const float data_float, const unsigned short precision);
    bool buf_clear      (BufId bufId);

  // геттеры
    const char*  buf_get_arr_ptr  (BufId bufId);
    unsigned int buf_get_size_left(BufId bufId);
    bool         buf_get_is_empty (BufId bufId);