#pragma once
  #include <stdbool.h>
  
  typedef enum
  {
    ERR__TYPE__ANY_TYPE = 0, // Должен быть первым! Должен быть 0 для тестов!   // Фильтр для поиска ошибок с любым типом //
    ERR__TYPE__ALERT,          
    ERR__TYPE__WARNING,
    ERR__TYPE__ERROR,
    ERR__TYPE__FATAL,

    ERR__TYPE__AMOUNT,        // ДОЛЖЕН БЫТЬ ПОСЛЕДНИМ! // Количество Типов
  } ErrType;

  typedef enum                                                                  // перечисление кодов ошибок
  {
    #define ERRORS_LIST(error_type, error_id, description) error_id,            // X-MACROS
    #include "c_errors_list.def"                                                // в папке "c_errors/include"
    #undef  ERRORS_LIST
    
    ERR__ID__AMOUNT,          // ДОЛЖЕН БЫТЬ ПОСЛЕДНИМ! // Количество Ошибок
  }ErrId;

  bool err__has_unhandled_errors (ErrType error_type);
  bool err__raise_error          (ErrId error_id);

  // доп функции для юнит-тестов, интеграционных тестов
    #ifdef ENABLE_UNIT_TESTING_API
      unsigned short err__get_total_counter(ErrId error_id);
      void           err__reset_all();
    #endif