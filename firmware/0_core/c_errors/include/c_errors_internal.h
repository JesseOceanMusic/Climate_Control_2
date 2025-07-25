#pragma once
  #include "c_errors_public.h"

  #define ERR__USHORT_MAX 65535
  #define ERR__COUNTER_MAX ERR__USHORT_MAX

  struct ErrInfo
  {
    ErrId          id;                // id ошибки
    ErrType        type;              // тип ошибки
    const char*    description_ptr;   // const char* = указатель на неизменяемые данные, сам указатель менять можно.
    bool           unhandled;         // обработана?
    unsigned short counter_unhandled; // сколько необработанных ошибок
    unsigned short counter_total;     // сколько было всего ошибок за время работы программы
  };

  bool err__get_info               (struct ErrInfo* errInfo, ErrId error_id);
  bool err__reset_counter_and_flag (ErrId error_id);
  bool err__is_id_correct          (ErrId error_id);
  bool err__is_type_correct        (ErrType type);