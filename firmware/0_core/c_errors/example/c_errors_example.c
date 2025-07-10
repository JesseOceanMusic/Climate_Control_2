/// === ИНИЦИАЛИЗАЦИЯ === ///
  //добавить коды и описание ошибок в файл c_errors_list.def
  #include "c_errors_public.h"

/// === ИСПОЛЬЗОВАНИЕ === ///
  #include <stdio.h>

  int main()
  {
    if(err_has_unhandled_errors(ERR_TYPE__ANY_TYPE) == false)
    {
      printf("%s", "1. doesn't have unhandled errors =)\n");
    }

    err_raise_error(ERR_ID__UNDEFINED);
    printf("%s", "2. now i raised error...\n");

    if(err_has_unhandled_errors(ERR_TYPE__ANY_TYPE) == true)
    {
      printf("%s", "3. has unhandled errors =)\n");
    }
    return 0;
  }