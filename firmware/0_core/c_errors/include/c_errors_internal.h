#pragma once
  #include "c_errors_public.h"

  #define ERR_USHORT_MAX 65535
  #define ERR_COUNTER_MAX ERR_USHORT_MAX

  struct ErrInfo
  {
    ErrId          id;                // id ошибки
    ErrType        type;              // тип ошибки
    const char*    description_ptr;   // const* = указатель на неизменяемые данные, сам указатель менять можно.
    bool           unhandled;         // обработана?
    unsigned short counter_unhandled; // сколько необработанных ошибок
    unsigned short counter_total;     // сколько было всего ошибок за время работы программы
  };

  bool err_get_info              (struct ErrInfo* errInfo, ErrId error_id);
  bool err_reset_counter_and_flag(ErrId error_id);
  bool err_is_id_correct         (ErrId error_id);
  bool err_is_type_correct       (ErrType type);