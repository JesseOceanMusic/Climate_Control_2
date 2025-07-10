#include "c_errors_writer.h"
#include "stdio.h"

int main()
{
  // вывод необработанных ошибок
    printf("Есть необработанные ошибки? %d\n", err_has_unhandled_errors(ERR_TYPE__ANY_TYPE));
    printf("поднимаем ошибку ERR_ID__UNDEFINED...\n");
      err_raise_error(ERR_ID__UNDEFINED);
    printf("Есть необработанные ошибки? %d\n", err_has_unhandled_errors(ERR_TYPE__ANY_TYPE));
    printf("Ищем и выводим необработанные ошибки и заголовок:\n");
      printf("%s", ERR_HEADER_MESSAGE);
      for(ErrId errId = 0; errId < ERR_ID__AMOUNT; errId++)
      {
        err_writer__write_one_err(BUF_ID__TG_MSG, errId, ERR_TYPE__ANY_TYPE, true);
      }
    printf("%s", buf_get_arr_ptr(BUF_ID__TG_MSG));

    buf_clear(BUF_ID__TG_MSG);

  // вывод всех ошибок
    printf("вывод всего списка ошибок...\n");
    printf("%s", ERR_HEADER_MESSAGE);
    for(ErrId errId = 0; errId < ERR_ID__AMOUNT; errId++)
    {
      err_writer__write_one_err(BUF_ID__TG_MSG, errId, ERR_TYPE__ANY_TYPE, false);
    }
    printf("%s", buf_get_arr_ptr(BUF_ID__TG_MSG));

    buf_clear(BUF_ID__TG_MSG);
}
