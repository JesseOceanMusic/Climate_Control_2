/// === ИНИЦИАЛИЗАЦИЯ === ///
  //добавить коды и описание ошибок в файл c_errors_list.def
  #include "c_errors_public.h"

/// === ИСПОЛЬЗОВАНИЕ === ///
  #include <stdio.h>

  int main()
  {
    if(err__has_unhandled_errors(ERR__TYPE__ANY_TYPE) == false)
    {
      printf("%s", "1. doesn't have unhandled errors =)\n");
    }

    err__raise_error(ERR__ID__UNDEFINED);
    printf("%s", "2. now i raised error...\n");

    if(err__has_unhandled_errors(ERR__TYPE__ANY_TYPE) == true)
    {
      printf("%s", "3. has unhandled errors =)\n");
    }
    return 0;
  }