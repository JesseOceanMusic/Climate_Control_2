#include "jesse_errors.h"

  static const char *error_description_ptr [ERRORS_AMOUNT] =                    // указатели на текстовое описание ошибок из .def файла
  {
    #define ERRORS_LIST(error_id, description) [error_id] = description,        // X-MACROS
    #include "errors_list.def"
    #undef ERRORS_LIST
  };

  bool unhandled_errors [ERRORS_AMOUNT] = {false};                              // необработанные ошибки
  static unsigned short error_counters_arr [ERRORS_AMOUNT] = {0};               // количество ошибок за всё время работы программы

  static bool is_error_id_correct(unsigned short error_id)
  {
    if(error_id < ERRORS_AMOUNT)
    {
      return true;
    }
    raise_error(ERROR_ID_WRONG_ERROR_ID);
    return false;
  }

  const char* get_error_description_ptr(unsigned short error_id)
  {
    if(is_error_id_correct(error_id) == true)
    {
      return error_description_ptr[error_id];
    }
    return error_description_ptr[ERROR_ID_WRONG_ERROR_ID];                      // ОШИБКА - возвращаем рабочий указатель на другую ошибку, но такого быть не должно
  }

  unsigned short get_error_counter(unsigned short error_id)
  {
    if(is_error_id_correct(error_id) == true)
    {
      return error_counters_arr[error_id];
    }
    return 12345;                                                               // ОШИБКА - возвращаем подозрительно симметричное число
  }

  void raise_error(unsigned short error_id)                                     // поднять флаг ошибки (принимает id)
  {
    if(is_error_id_correct(error_id) == true)
    {
      error_counters_arr[error_id]++;
      unhandled_errors[error_id] = true;
    }
  }

  bool has_unhandled_errors()                                                   // проверяет, есть ли необработанные ошибки
  {
    for (int i = 0; i < ERRORS_AMOUNT; i++)
    {
      if (unhandled_errors[i] == true)
      {
        return true;
      }
    }
    return false;
  }