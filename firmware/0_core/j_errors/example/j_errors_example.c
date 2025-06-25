/// === ИНИЦИАЛИЗАЦИЯ === ///
  //добавить коды и описание ошибок в файл j_errors_list.def
  #include "j_errors.h"

/// === ИСПОЛЬЗОВАНИЕ === ///
  #include <stdio.h>

  int main()
  { 
    err_raise_error(ERR_ID__UNDEFINED);
    err_raise_error(ERR_ID__NULL_IN_BUFFER);
    if (err_has_unhandled_errors(ERR_TYPE__ANY_TYPE) == true)
    {
      printf("%s", ERRORS_HEADER_MESSAGE);
    }
    return 0;
  }