#include "j_errors.h"

  static const char *error_description_ptr [J_ERRORS_AMOUNT] =                  // указатели на текстовое описание ошибок из .def файла
  {
    #define ERRORS_LIST(error_type, error_id, description) [error_id] = description,        // X-MACROS
    #include "j_errors_list.def"
    #undef ERRORS_LIST
  };

  static const ErrorsTypes error_type_arr[J_ERRORS_AMOUNT] =
  {
    #define ERRORS_LIST(error_type, error_id, description) [error_id] = error_type,        // X-MACROS
    #include "j_errors_list.def"
    #undef ERRORS_LIST
  };

  static bool unhandled_errors [J_ERRORS_AMOUNT] = {false};                     // необработанные ошибки
  static unsigned short error_counters_arr [J_ERRORS_AMOUNT] = {0};             // количество ошибок за всё время работы программы

  static bool is_error_id_correct(unsigned short error_id)
  {
    if(error_id < J_ERRORS_AMOUNT)
    {
      return true;
    }
    raise_error(ERR_ID__WRONG_ERR_ID);
    return false;
  }

  ErrorsTypes get_error_type(unsigned short error_id)
  {
    if(is_error_id_correct(error_id) == true)
    {
      return error_type_arr[error_id];    
    }
    return ERR_TYPE__FATAL;  // ОШИБКА, НО ЧТО-ТО ВЕРНУТЬ НАДО!
  }

  bool get_unhandled_error_flag(unsigned short error_id)
  {
    if(is_error_id_correct(error_id) == true)
    {
      return unhandled_errors[error_id];
    }
    return false;  // ОШИБКА, НО ЧТО-ТО ВЕРНУТЬ НАДО!
  }
  
  void reset_unhandled_error_flag(unsigned short error_id)
  {
    if(is_error_id_correct(error_id) == true)
    {
      unhandled_errors[error_id] = false;
    }
  }

  const char* get_error_description_ptr(unsigned short error_id)
  {
    if(is_error_id_correct(error_id) == true)
    {
      return error_description_ptr[error_id];
    }
    return error_description_ptr[ERR_ID__WRONG_ERR_ID];                      // ОШИБКА - возвращаем рабочий указатель на другую ошибку, но такого быть не должно
  }

  unsigned short get_error_counter(unsigned short error_id)
  {
    if(is_error_id_correct(error_id) == true)
    {
      return error_counters_arr[error_id];
    }
    return UNSIGNED_SHORT_BAD_RETURN_55555;                                     // ОШИБКА - возвращаем подозрительно симметричное число
  }

  void reset_error_counter(unsigned short error_id)
  {
    if(is_error_id_correct(error_id) == true)
    {
      error_counters_arr[error_id] = 0;
    }    
  }

  void raise_error(unsigned short error_id)                                     // поднять флаг ошибки (принимает id)
  {
    if(is_error_id_correct(error_id) == true)
    {
      unhandled_errors[error_id] = true;
      if(error_counters_arr[error_id] < J_UNSIGNED_SHORT_MAX)
      {
        error_counters_arr[error_id]++;
      }
    }
  }

  bool has_unhandled_errors()
  {
    for (int id = 0; id < J_ERRORS_AMOUNT; id++)
    {
      if (unhandled_errors[id] == true)
      {
        return true;
      }
    }
    return false;    
  }

  bool has_unhandled_errors_type(ErrorsTypes type)                                                   // проверяет, есть ли необработанные ошибки
  {
    for (int id = 0; id < J_ERRORS_AMOUNT; id++)
    {
      if (unhandled_errors[id] == true && error_type_arr[id] == type)
      {
        return true;
      }
    }
    return false;
  }