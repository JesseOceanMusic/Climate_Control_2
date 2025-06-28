#pragma once
  #include <stdbool.h>
  #define ERR_USHORT_MAX 65535

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
    #include "c_errors_list.def"                                                // в папке "c_errors/include"
    #undef  ERRORS_LIST
    ERR_ID__AMOUNT,          // ДОЛЖЕН БЫТЬ ПОСЛЕДНИМ! // Количество Ошибок
  }ErrId;

  struct ErrInfo
  {
    bool           was_id_correct;
    ErrId          id;                // id ошибки
    ErrType        type;              // тип ошибки
    const char*    description_ptr;   // const* = указатель на неизменяемые данные, сам указатель менять можно.
    bool           unhandled;         // обработана?
    unsigned short counter_unhandled; // сколько необработанных ошибок
    unsigned short counter_total;     // сколько было всего ошибок за время работы программы
  };

  static const char ERR_HEADER_MESSAGE[] = "ERRORS:\nID/counter/description:\n";
 
  bool           err_has_unhandled_errors  (ErrType type);
  bool           err_raise_error           (ErrId error_id);
  struct ErrInfo err_get_info              (ErrId error_id);                    // возвращаем именно по значению, а не по указателю. безопаснее. разница в скорости минимальна.
  bool           err_reset_counter_and_flag(ErrId error_id);

  // доп функции для тестирования  
    #ifdef TURN_ON_TEST_FEATURES
      void err_reset_all();
    #endif