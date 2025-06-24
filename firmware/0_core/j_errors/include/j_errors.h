#pragma once
  #include <stdbool.h>
  #define J_UNSIGNED_SHORT_MAX 65535
  #define UNSIGNED_SHORT_BAD_RETURN_55555 55555                                 // чтобы было видно в отчете подозрительно симметричное число

  typedef enum
  {
    ERR_TYPE__ALERT,
    ERR_TYPE__WARNING,
    ERR_TYPE__ERROR,
    ERR_TYPE__FATAL,
    ERR_TYPE__ALL,
  } ErrorsTypes;

  enum                                                                          // перечисление кодов ошибок
  {
    #define ERRORS_LIST(error_type, error_id, description) error_id,            // X-MACROS
    #include "j_errors_list.def"                                                // в папке "j_errors/include"
    #undef  ERRORS_LIST
    J_ERRORS_AMOUNT,
  };

  static const char ERRORS_HEADER_MESSAGE[] = "ERRORS:\nID/counter/description:\n";
 
  bool has_unhandled_errors();
  void raise_error(unsigned short error_id);
  
  const char* get_error_description_ptr(unsigned short error_id);
  unsigned short get_error_counter(unsigned short error_id);

  bool get_unhandled_error_flag(unsigned short error_id);
  void reset_unhandled_error_flag(unsigned short error_id);

  ErrorsTypes get_error_type(unsigned short error_id);
  


  