#pragma once
  #include <stdbool.h>
  enum                                                                          // перечисление кодов ошибок
                                                                                // raise_error(0) -> raise_error(ERROR_ID_UNDEFINED)
  {
    #define ERRORS_LIST(error_id, description) error_id,                        // X-MACROS
    #include "errors_list.def"                                                  // в папке "jesse_errors/include" он специально переименован
                                                                                // в "errors_list_example.def", чтобы не было конфликта
                                                                                // в основнов проекте
    #undef  ERRORS_LIST
    ERRORS_AMOUNT,
  };

  extern bool unhandled_errors[ERRORS_AMOUNT];                                  // ТОЛЬКО ДЛЯ jesse_errors_manager


  const char* get_error_description_ptr(unsigned short error_id);

  unsigned short get_error_counter(unsigned short error_id);

  void raise_error(unsigned short error_id);

  bool has_unhandled_errors();