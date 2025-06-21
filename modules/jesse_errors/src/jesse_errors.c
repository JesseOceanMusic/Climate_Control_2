#include "jesse_errors.h"

  char *error_description_ptr [ERRORS_AMOUNT] =                                 // указатели на текстовое описание ошибок из .def файла
  {
    #define ERRORS_LIST(error_id, description) [error_id] = description,        // X-MACROS
    #include "errors_list.def"
    #undef ERRORS_LIST
  };

  bool unhandled_errors [ERRORS_AMOUNT] = {false};                              // необработанные ошибки
  unsigned short error_counters_arr [ERRORS_AMOUNT] = {0};                      // количество ошибок за всё время работы программы

  void raise_error (unsigned short error_id)                                    // поднять флаг ошибки (принимает id)
  {
    if (error_id < ERRORS_AMOUNT)
    {
      error_counters_arr[error_id]++;
      unhandled_errors[error_id] = true;
    }
  }

  bool has_unhandled_errors ()                                                  // проверяет, есть ли необработанные ошибки
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