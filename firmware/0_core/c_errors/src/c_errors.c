#include "c_errors_internal.h"
#include <stddef.h>                                                             // Для NULL

  static const char *err__description_ptr_arr [ERR__ID__AMOUNT] =                 // указатели на текстовое описание ошибок из .def файла
  {
    #define ERRORS_LIST(error_type, error_id, description) [error_id] = description,        // X-MACROS
    #include "c_errors_list.def"
    #undef ERRORS_LIST
  }; 


  static const ErrType err__type_arr[ERR__ID__AMOUNT] =
  {
    #define ERRORS_LIST(error_type, error_id, description) [error_id] = error_type,        // X-MACROS
    #include "c_errors_list.def"
    #undef ERRORS_LIST
  };

  static bool           err__unhandled_arr         [ERR__ID__AMOUNT] = {false};   // обработана?
  static unsigned short err__counter_unhandled_arr [ERR__ID__AMOUNT] = {0};       // сколько необработанных ошибок
  static unsigned short err__counter_total_arr     [ERR__ID__AMOUNT] = {0};       // сколько было всего ошибок за время работы программы

  bool err__is_id_correct(ErrId error_id)
  {
    if(error_id >= 0 && error_id < ERR__ID__AMOUNT) { return true; }
    if(error_id != ERR__ID__ERR__WRONG_ID)                                        // Защита от рекурсии, при неправильной инициализации    // LCOV_EXCL_BR_LINE // это условие необходимо, для защиты от рекурсии! Но написание теста само по себе создаст уязвимость, так как придется отключить _Static_assert и предварительные проверки в юнит тесте и намеренно сломать c_errors_list.def! defensive programming
    {                                                                           // ERR__ID__ERR__WRONG_ID, например -1
      err__raise_error(ERR__ID__ERR__WRONG_ID);
    }
    return false;
  }

  bool err__is_type_correct(ErrType error_type)
  {
    if(error_type >= 0 && error_type < ERR__TYPE__AMOUNT) { return true; }
    err__raise_error(ERR__ID__ERR__WRONG_TYPE);
    return false;
  }

  bool err__raise_error(ErrId error_id)                                          // поднять флаг ошибки (принимает id)
  {
    if(err__is_id_correct(error_id) == true)
    {
      err__unhandled_arr[error_id] = true;

      if(err__counter_total_arr[error_id] < ERR__COUNTER_MAX)
      {
        err__counter_total_arr[error_id]++;
      }

      if(err__counter_unhandled_arr[error_id] < ERR__COUNTER_MAX)
      {
        err__counter_unhandled_arr[error_id]++;
      }
      return true;
    }

    return false;
  }

  bool err__get_info(struct ErrInfo* errInfo, ErrId error_id)
  {
    if(errInfo == NULL)
    {
      err__raise_error(ERR__ID__ERR__RECEIVED_NULL);
      return false;
    }
    
    if(err__is_id_correct(error_id) == true)
    {
      errInfo->id                = error_id;
      errInfo->type              = err__type_arr              [error_id];
      errInfo->description_ptr   = err__description_ptr_arr   [error_id];
      errInfo->unhandled         = err__unhandled_arr         [error_id];
      errInfo->counter_unhandled = err__counter_unhandled_arr [error_id];
      errInfo->counter_total     = err__counter_total_arr     [error_id];
      return true;
    }
    return false;
  }

  bool err__reset_counter_and_flag(ErrId error_id)
  {
    if(err__is_id_correct(error_id) == true)
    {
      err__unhandled_arr[error_id] = false;
      err__counter_unhandled_arr[error_id] = 0;
      return true;
    }
    return false;
  }

  bool err__has_unhandled_errors(ErrType error_type)
  {
    if(err__is_type_correct(error_type) == true)
    {
      for(int id = 0; id < ERR__ID__AMOUNT; id++)
      {
        if(err__unhandled_arr[id] == true)
        {
          if(error_type == ERR__TYPE__ANY_TYPE || err__type_arr[id] == error_type)
          {
            return true;  
          }
        }
      }
    }
    return false;
  }

  // тесты
    _Static_assert(sizeof(err__description_ptr_arr)/sizeof(char*) == ERR__ID__AMOUNT, "Mismatch between ERR__ID__AMOUNT and err__description_ptr_arr");
    _Static_assert(sizeof(err__type_arr)/sizeof(ErrType)          == ERR__ID__AMOUNT, "Mismatch between ERR__ID__AMOUNT and err__type_arr");

    _Static_assert(ERR__ID__UNDEFINED         == 0,               "ERR__ID__UNDEFINED must be 0");            
    _Static_assert(ERR__ID__ERR__WRONG_ID      < ERR__ID__AMOUNT,   "ERR__ID__ERR__WRONG_ID must be defined in c_errors_list.def!");
    _Static_assert(ERR__ID__ERR__WRONG_TYPE    < ERR__ID__AMOUNT,   "ERR__ID__ERR__WRONG_TYPE must be defined in c_errors_list.def!");
    _Static_assert(ERR__ID__ERR__RECEIVED_NULL < ERR__ID__AMOUNT,   "ERR__ID__ERR__RECEIVED_NULL must be defined in c_errors_list.def!");

    _Static_assert(ERR__TYPE__ANY_TYPE        == 0,               "ERR__TYPE__ANY_TYPE must be 0!");
    _Static_assert(ERR__TYPE__ANY_TYPE        < ERR__TYPE__AMOUNT, "ERR__TYPE__ANY_TYPE must be defined in c_errors_list.def!");

  // доп функции для юнит-тестов, интеграционных тестов
    #ifdef ENABLE_UNIT_TESTING_API
      unsigned short err__get_total_counter(ErrId error_id)
      {
        if(err__is_id_correct(error_id) == true)
        {
          return err__counter_total_arr[error_id];
        }
        return 0;
      }

      void err__reset_all()
      {
        for(ErrId id = 0; id < ERR__ID__AMOUNT; id++)
        {
          err__unhandled_arr         [id] = false;   // обработана?
          err__counter_unhandled_arr [id] = 0;       // сколько необработанных ошибок
          err__counter_total_arr     [id] = 0;       // сколько было всего ошибок за время работы программы
        }
      }
    #endif