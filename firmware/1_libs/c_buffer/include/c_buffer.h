#pragma once
  #include <stdbool.h>

  // настройки
    #define BUF__INT_MAX_SYMBOLS_LENGTH   64                                    // максимальная длина int в символах в этом микроконтроллере
    #define BUF__FLOAT_MAX_SYMBOLS_LENGTH 64                                    // максимальная длина float в символах..
    #define BUF__FLOAT_MAX_PRECISION      6                                     // максимально количество знаков после запятой для аргумента функции buf__write_float()
    //#define BUF__FULL_CLEAR_WITH_MEMSET                                       // безопаснее, но дольше. учше не включать.

    extern const char* const BUF__BAD_ID;                                       // для buf__get_arr_ptr()

  // буферы
    typedef enum                                                                // перечисление id буферов
    {
      #define BUFFER_LIST(buffer_id, buffer_name, buffer_size) buffer_id,       // X-MACROS
      #include "c_buffer_list.def"
      #undef  BUFFER_LIST
      BUF__ID__AMOUNT,                                                           // ДОЛЖЕН БЫТЬ ПОСЛЕДНИМ! // Количество Ошибок
    }BufId;

  // хелперы
    bool buf__is_id_correct     (BufId bufId);
    bool buf__will_this_data_fit(BufId bufId, const char* const data_char_ptr);
  
  // операции
    bool buf__write_char (BufId bufId, const char* const data_char_ptr);
    bool buf__write_int  (BufId bufId, const int   data_int);
    bool buf__write_float(BufId bufId, const float data_float, const unsigned short precision);
    bool buf__clear      (BufId bufId);

  // геттеры
    const char*  buf__get_arr_ptr  (BufId bufId);
    unsigned int buf__get_size_left(BufId bufId);
    bool         buf__get_is_empty (BufId bufId);