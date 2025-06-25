#include "j_errors.h"

  static const char *err_description_ptr_arr [ERR_ID__AMOUNT] =                 // указатели на текстовое описание ошибок из .def файла
  {
    #define ERRORS_LIST(error_type, error_id, description) [error_id] = description,        // X-MACROS
    #include "j_errors_list.def"
    #undef ERRORS_LIST
  };


  static const ErrType err_type_arr[ERR_ID__AMOUNT] =
  {
    #define ERRORS_LIST(error_type, error_id, description) [error_id] = error_type,        // X-MACROS
    #include "j_errors_list.def"
    #undef ERRORS_LIST
  };

  static bool           err_unhandled_errors [ERR_ID__AMOUNT] = {false};        // необработанные ошибки
  static unsigned short err_counter_cur_arr  [ERR_ID__AMOUNT] = {0};            // количество ошибок за всё время работы программы
  static unsigned short err_counter_last_arr [ERR_ID__AMOUNT] = {0};            // кол-во ошибок до сброса

  static struct ErrInfo errInfo =
  {
    .was_id_correct  = false,
    .id              = ERR_ID__WRONG_ERR_ID,
    .type            = ERR_TYPE__FATAL,
    .description_ptr = "errInfo_ini",
    .unhandled       = false,
    .counter_cur     = 0,
    .counter_last    = 0,
  };  

  static bool is_err_id_correct(ErrId error_id)
  {
    if(error_id != ERR_ID__AMOUNT)
    {
      return true;
    }
    if (error_id != ERR_ID__WRONG_ERR_ID)  // Защита от рекурсии
    {
      err_raise_error(ERR_ID__WRONG_ERR_ID);
    }
    return false;
  }

  static bool is_err_type_correct(ErrType type)
  {
    if(type != ERR_TYPE__AMOUNT)
    {
      return true;
    }
    err_raise_error(ERR_ID__WRONG_ERR_TYPE);
    return false;
  }

  bool err_raise_error(ErrId error_id)                                          // поднять флаг ошибки (принимает id)
  {
    if(is_err_id_correct(error_id) == true)
    {
      err_unhandled_errors[error_id] = true;
      if(err_counter_cur_arr[error_id] < J_UNSIGNED_SHORT_MAX)
      {
        err_counter_cur_arr[error_id]++;
      }
      return true;
    }

    return false;
  }

  struct ErrInfo err_get_info(ErrId error_id)
  {
    if(is_err_id_correct(error_id) == true)
    {
      errInfo.was_id_correct  = true;
      errInfo.id              = error_id;
      errInfo.type            = err_type_arr           [error_id];
      errInfo.description_ptr = err_description_ptr_arr[error_id];
      errInfo.unhandled       = err_unhandled_errors   [error_id];
      errInfo.counter_cur     = err_counter_cur_arr    [error_id];
      errInfo.counter_last    = err_counter_last_arr   [error_id];
      return errInfo;
    }

    errInfo.was_id_correct = false;
    return errInfo;
  }

  bool err_reset_counter_and_flag(ErrId error_id)
  {
    if(is_err_id_correct(error_id) == true)
    {
      err_unhandled_errors[error_id] = false;
      err_counter_last_arr[error_id] = err_counter_cur_arr[error_id];
      return true;
    }
    return false;
  }

  bool err_has_unhandled_errors(ErrType type)
  {
    if(is_err_type_correct(type) == true)
    {
      
      for(int id = 0; id < ERR_ID__AMOUNT; id++)
      {
        if(err_unhandled_errors[id] == true)
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

    _Static_assert(ERR_ID__WRONG_ERR_ID   < ERR_ID__AMOUNT,   "ERR_ID__WRONG_ERR_ID must be defined in j_errors_list.def!");
    _Static_assert(ERR_ID__WRONG_ERR_TYPE < ERR_ID__AMOUNT,   "ERR_ID__WRONG_ERR_TYPE must be defined in j_errors_list.def!");
    _Static_assert(ERR_TYPE__ANY_TYPE     < ERR_TYPE__AMOUNT, "ERR_TYPE__ANY_TYPE must be defined in j_errors_list.def!");
    _Static_assert(ERR_TYPE__ANY_TYPE     == 0,               "ERR_TYPE__ANY_TYPE must be 0!");
