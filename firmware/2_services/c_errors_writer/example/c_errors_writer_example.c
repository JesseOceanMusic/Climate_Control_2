
#include "c_errors_writer.h"
#include <windows.h>                                                          // Для Sleep() // !!!ТОЛЬКО ДЛЯ ПРИМЕРА!!!

int main()
{
  // создаём буфер (должен быть static и выделятся вне функций, здесь только для теста)
    #define TEST_BUF_SIZE 1024
      char char_buffer_arr[TEST_BUF_SIZE];
      struct BufInfo bufInfo;
      assert(buf_init(&bufInfo, char_buffer_arr, sizeof(char_buffer_arr)) == true);
      printf("  PASS: buf_init().\n");
    #undef TEST_BUF_SIZE

  // вывод необработанных ошибок
    err_raise_error(ERR_ID__UNDEFINED);
    if(err_has_unhandled_errors(ERR_TYPE__ANY_TYPE) == true)
    {
      printf("%s", ERR_HEADER_MESSAGE);
      for(ErrId errId = 0; errId < ERR_ID__AMOUNT; errId++)
      {
        err_writer(&bufInfo, errId, ERR_TYPE__ANY_TYPE, true);
      }
    }
    printf("%s", bufInfo.arr_ptr);

    buf_clear(&bufInfo);

  // вывод всех ошибок
    printf("%s", ERR_HEADER_MESSAGE);
    for(ErrId errId = 0; errId < ERR_ID__AMOUNT; errId++)
    {
      err_writer(&bufInfo, errId, ERR_TYPE__ANY_TYPE, false);
    }
    printf("%s", bufInfo.arr_ptr);
}
