/// === ИНИЦИАЛИЗАЦИЯ === ///
  //добавить коды и описание ошибок в файл j_errors_list.def
  #include "j_errors.h"

/// === ИСПОЛЬЗОВАНИЕ === ///
  #include <stdio.h>

  int main()
  { 
    raise_error(ERR_ID__UNDEFINED);
    raise_error(ERR_ID__NULL_IN_BUFFER);
    if (has_unhandled_errors() == true)
    {
      printf("ERRORS:\n");
      printf("id/counter/description\n");
      for (int id = 0; id < J_ERRORS_AMOUNT; id++)
      {
        if (get_error_counter(id) > 0)
        {
          printf("%d/", id);
          printf("%d/", get_error_counter(id));
          printf("%s\n", get_error_description_ptr(id));
        }
      }
    }
    return 0;
  }