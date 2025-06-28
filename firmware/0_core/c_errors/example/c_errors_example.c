/// === ИНИЦИАЛИЗАЦИЯ === ///
  //добавить коды и описание ошибок в файл c_errors_list.def
  #include "c_errors.h"

/// === ИСПОЛЬЗОВАНИЕ === ///
  #include <stdio.h>

  int main()
  { 
    err_raise_error(ERR_ID__UNDEFINED);
    err_raise_error(ERR_ID__BUF_RECEIVED_NULL);
    if (err_has_unhandled_errors(ERR_TYPE__ANY_TYPE) == true)
    {
      printf("%s", ERR_HEADER_MESSAGE);
    }
    return 0;
  }