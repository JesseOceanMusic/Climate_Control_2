#pragma once
  #include <stdbool.h>
  #define J_UNSIGNED_SHORT_MAX 65535

  typedef enum
  {
    ERR_TYPE__ANY_TYPE = 0,  // Должен быть первым! Должен быть 0 для тестов! // Фильтр для поиска ошибок с любым типом //
    ERR_TYPE__ALERT,          
    ERR_TYPE__WARNING,
    ERR_TYPE__ERROR,
    ERR_TYPE__FATAL,
    ERR_TYPE__AMOUNT,        // ДОЛЖЕН БЫТЬ ПОСЛЕДНИМ! // Количество Типов
  } ErrType;

  typedef enum                                                                  // перечисление кодов ошибок
  {
    #define ERRORS_LIST(error_type, error_id, description) error_id,            // X-MACROS
    #include "j_errors_list.def"                                                // в папке "j_errors/include"
    #undef  ERRORS_LIST
    ERR_ID__AMOUNT,          // ДОЛЖЕН БЫТЬ ПОСЛЕДНИМ! // Количество Ошибок
  }ErrId;

  struct ErrInfo
  {
    bool was_id_correct;
    ErrId id;
    ErrType type;
    const char* description_ptr; // const = указатель на неизменяемые данные, сам указатель менять можно.
    bool unhandled;
    unsigned short counter_cur;
    unsigned short counter_last;
  };

  static const char ERRORS_HEADER_MESSAGE[] = "ERRORS:\nID/counter/description:\n";
 
  bool           err_has_unhandled_errors  (ErrType type);
  bool           err_raise_error           (ErrId error_id);
  struct ErrInfo err_get_info              (ErrId error_id);                    // возвращаем именно по значению, а не по указателю. безопаснее. разница в скорости минимальна.
  bool           err_reset_counter_and_flag(ErrId error_id);
