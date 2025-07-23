#include "c_errors_internal.h"
#include <stddef.h>                                                             // Для NULL

  static const char *err_description_ptr_arr [ERR_ID__AMOUNT] =                 // указатели на текстовое описание ошибок из .def файла
  {
    #define ERRORS_LIST(error_type, error_id, description) [error_id] = description,        // X-MACROS
    #include "c_errors_list.def"
    #undef ERRORS_LIST
  }; 


  static const ErrType err_type_arr[ERR_ID__AMOUNT] =
  {
    #define ERRORS_LIST(error_type, error_id, description) [error_id] = error_type,        // X-MACROS
    #include "c_errors_list.def"
    #undef ERRORS_LIST
  };

  static bool           err_unhandled_arr         [ERR_ID__AMOUNT] = {false};   // обработана?
  static unsigned short err_counter_unhandled_arr [ERR_ID__AMOUNT] = {0};       // сколько необработанных ошибок
  static unsigned short err_counter_total_arr     [ERR_ID__AMOUNT] = {0};       // сколько было всего ошибок за время работы программы

  bool err_is_id_correct(ErrId error_id)
  {
    if(error_id >= 0 && error_id < ERR_ID__AMOUNT) { return true; }
    if(error_id != ERR_ID__ERR_WRONG_ID)                                        // Защита от рекурсии, если кто-то неправильно инициализирует ERR_ID__ERR_WRONG_ID 
    {
      err_raise_error(ERR_ID__ERR_WRONG_ID);
    }
    return false;
  }

  bool err_is_type_correct(ErrType type)
  {
    if(type >= 0 && type < ERR_TYPE__AMOUNT) { return true; }
    err_raise_error(ERR_ID__ERR_WRONG_TYPE);
    return false;
  }

  bool err_raise_error(ErrId error_id)                                          // поднять флаг ошибки (принимает id)
  {
    if(err_is_id_correct(error_id) == true)
    {
      err_unhandled_arr[error_id] = true;

      if(err_counter_total_arr[error_id] < ERR_COUNTER_MAX)
      {
        err_counter_total_arr[error_id]++;
      }

      if(err_counter_unhandled_arr[error_id] < ERR_COUNTER_MAX)
      {
        err_counter_unhandled_arr[error_id]++;
      }
      return true;
    }

    return false;
  }

  bool err_get_info(struct ErrInfo* errInfo, ErrId error_id)
  {
    if(errInfo == NULL)
    {
      err_raise_error(ERR_ID__ERR_RECEIVED_NULL);
      return false;
    }
    
    if(err_is_id_correct(error_id) == true)
    {
      errInfo->id                = error_id;
      errInfo->type              = err_type_arr              [error_id];
      errInfo->description_ptr   = err_description_ptr_arr   [error_id];
      errInfo->unhandled         = err_unhandled_arr         [error_id];
      errInfo->counter_unhandled = err_counter_unhandled_arr [error_id];
      errInfo->counter_total     = err_counter_total_arr     [error_id];
      return true;
    }
    return false;
  }

  bool err_reset_counter_and_flag(ErrId error_id)
  {
    if(err_is_id_correct(error_id) == true)
    {
      err_unhandled_arr[error_id] = false;
      err_counter_unhandled_arr[error_id] = 0;
      return true;
    }
    return false;
  }

  bool err_has_unhandled_errors(ErrType type)
  {
    if(err_is_type_correct(type) == true)
    {
      for(int id = 0; id < ERR_ID__AMOUNT; id++)
      {
        if(err_unhandled_arr[id] == true)
        {
          if(type == ERR_TYPE__ANY_TYPE || err_type_arr[id] == type)
          {
            return true;  
          }
        }
      }
    }
    return false;
  }

  // тесты
    _Static_assert(sizeof(err_description_ptr_arr)/sizeof(char*) == ERR_ID__AMOUNT, "Mismatch between ERR_ID__AMOUNT and err_description_ptr_arr");
    _Static_assert(sizeof(err_type_arr)/sizeof(ErrType)          == ERR_ID__AMOUNT, "Mismatch between ERR_ID__AMOUNT and err_type_arr");

    _Static_assert(ERR_ID__UNDEFINED         == 0,               "ERR_ID__UNDEFINED must be 0");            
    _Static_assert(ERR_ID__ERR_WRONG_ID      < ERR_ID__AMOUNT,   "ERR_ID__ERR_WRONG_ID must be defined in c_errors_list.def!");
    _Static_assert(ERR_ID__ERR_WRONG_TYPE    < ERR_ID__AMOUNT,   "ERR_ID__ERR_WRONG_TYPE must be defined in c_errors_list.def!");
    _Static_assert(ERR_ID__ERR_RECEIVED_NULL < ERR_ID__AMOUNT,   "ERR_ID__ERR_RECEIVED_NULL must be defined in c_errors_list.def!");

    _Static_assert(ERR_TYPE__ANY_TYPE        == 0,               "ERR_TYPE__ANY_TYPE must be 0!");
    _Static_assert(ERR_TYPE__ANY_TYPE        < ERR_TYPE__AMOUNT, "ERR_TYPE__ANY_TYPE must be defined in c_errors_list.def!");

  // доп функции для юнит-тестов, интеграционных тестов
    #ifdef ENABLE_UNIT_TESTING_API
      unsigned short err_get_total_counter(ErrId error_id)
      {
        if(err_is_id_correct(error_id) == true)
        {
          return err_counter_total_arr[error_id];
        }
        return 0;
      }

      void err_reset_all()
      {
        for(ErrId id = 0; id < ERR_ID__AMOUNT; id++)
        {
          err_unhandled_arr         [id] = false;   // обработана?
          err_counter_unhandled_arr [id] = 0;       // сколько необработанных ошибок
          err_counter_total_arr     [id] = 0;       // сколько было всего ошибок за время работы программы
        }
      }
    #endif