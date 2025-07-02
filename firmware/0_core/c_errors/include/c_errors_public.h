#pragma once
  typedef enum
  {
    ERR_TYPE__ANY_TYPE = 0,  // Должен быть первым! Должен быть 0 для тестов!   // Фильтр для поиска ошибок с любым типом //
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

  bool           err_has_unhandled_errors  (ErrType type);
  bool           err_raise_error           (ErrId error_id);

  // доп функции для юнит-тестов, интеграционных тестов
    #ifdef ENABLE_UNIT_TESTING_API
      unsigned short err_get_total_counter(ErrId error_id);
      void           err_reset_all();
    #endif