#pragma once
  #include <stdbool.h>

  enum                                                                          // перечисление кодов ошибок
                                                                                // raise_error(0) -> raise_error(ERROR_ID_UNDEFINED)
  {
    #define ERRORS_LIST(error_id, description) error_id,                        // X-MACROS
    #include "errors_list.def"
    #undef  ERRORS_LIST
    ERRORS_AMOUNT,
  };

  extern char *error_description_ptr[ERRORS_AMOUNT];
  extern bool unhandled_errors[ERRORS_AMOUNT];
  extern unsigned short error_counters_arr [ERRORS_AMOUNT];

  void raise_error (unsigned short error_id);
  bool has_unhandled_errors ();