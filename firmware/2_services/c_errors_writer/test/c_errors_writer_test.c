#include "c_errors_writer_test.h"

void c_errors_writer_test()
{
  // создаём буфер (должен быть static и выделятся вне функций, здесь только для теста)
    #define TEST_BUF_SIZE 1024
      char char_buffer_arr[TEST_BUF_SIZE];
      struct BufHandle bufHandle;
      assert(buf_init(&bufHandle, char_buffer_arr, sizeof(char_buffer_arr)) == true);
    #undef TEST_BUF_SIZE
    printf("  PASS: buf_init().\n");

  // создаём структуру под ошибки и сбрасывает от предыдущих тестов
    struct ErrInfo errInfo;
    err_reset_all();

  // тест NULL
    assert(err_writer(NULL, ERR_ID__UNDEFINED, ERR_TYPE__ANY_TYPE, true) == false); // отправляем NULL
    assert(err_get_info(&errInfo, ERR_ID__ERR_WR_RECEIVED_NULL) == true);                             
    assert(errInfo.counter_unhandled == 1);                                     // проверяем, что ошибка NULL зарегестрированна
    printf("  PASS: NULL -> err_writer().\n");

  // неправильный id
    assert(err_writer(&bufHandle, ERR_ID__AMOUNT, ERR_TYPE__ANY_TYPE, true) == false);     // 1
    assert(err_writer(&bufHandle, -1, ERR_TYPE__ANY_TYPE, true) == false);                 // 2
    assert(bufHandle.is_empty == true);
    assert(err_get_info(&errInfo, ERR_ID__ERR_WR_WRONG_ID_OR_TYPE) == true);
    assert(errInfo.counter_unhandled == 2);
    assert(err_get_info(&errInfo, ERR_ID__ERR_WRONG_ID) == true);
    assert(errInfo.counter_unhandled == 2);
    printf("  PASS: wrong id -> err_writer().\n");

  // неправильный тип
    assert(err_writer(&bufHandle, ERR_ID__UNDEFINED, ERR_TYPE__AMOUNT, true) == false);    // 1
    assert(err_writer(&bufHandle, ERR_ID__UNDEFINED, -1, true) == false);                  // 2
    assert(bufHandle.is_empty == true);
    assert(err_get_info(&errInfo, ERR_ID__ERR_WR_WRONG_ID_OR_TYPE) == true);
    assert(errInfo.counter_unhandled == 4);
    assert(err_get_info(&errInfo, ERR_ID__ERR_WRONG_TYPE) == true);
    assert(errInfo.counter_unhandled == 2);
    printf("  PASS: wrong type -> err_writer().\n");

  // сброс ошибок
    err_reset_all();

  // нет ошибок, вывод unhandled (ничего не должен записать)
    for(ErrType errType = 0; errType < ERR_TYPE__AMOUNT; errType++)
    {
      for(ErrId errId = 0; errId < ERR_ID__AMOUNT; errId ++)
      {
        assert(err_writer(&bufHandle, errId, errType, true) == true);
        assert(bufHandle.is_empty == true);
      }
    }

    printf("  PASS: write 1.\n");

  // запись всех ошибок
    for(ErrId errId = 0; errId < ERR_ID__AMOUNT; errId ++)
    {
      unsigned int offset__last = bufHandle.offset_;
      assert(err_writer(&bufHandle, errId, ERR_TYPE__ANY_TYPE, false) == true);
      assert(bufHandle.is_empty == false);
      assert(offset__last + 5 < bufHandle.offset_);                                 // даже если описание пустое - как минимум три слэша и два кода
    }
    printf("  PASS: write 2.\n");
    printf("ERRORS LIST:\n");
    printf("%s", ERR_HEADER_MESSAGE);
    printf("%s", bufHandle.arr_ptr);
  
  // отчищаем буфер
    assert(buf_clear(&bufHandle) == true);
  // проверка фильтра
    for(ErrId errId = 0; errId < ERR_ID__AMOUNT; errId ++)
    {   
      // берем одну ошибку и поднимаем флаг
        assert(err_raise_error(errId) == true);
        assert(err_get_info(&errInfo, errId) == true);

      // проверяем что другие фильтры не реагируют
        for(ErrType errType = 0; errType < ERR_TYPE__AMOUNT; errType++)
        {
          if(errType != errInfo.type && errType != ERR_TYPE__ANY_TYPE)
          {
            for(ErrId errId2 = 0; errId2 < ERR_ID__AMOUNT; errId2 ++)
            {
              assert(err_writer(&bufHandle, errId2, errType, true) == true);
              assert(bufHandle.is_empty == true);
            }
          }
        }


      // проверяем, что нужный фильтр найдет только одну ошибку, как и должен
        int errors_written_counter = 0;
        for(ErrId errId = 0; errId < ERR_ID__AMOUNT; errId ++)
        {
          unsigned int offset__last = bufHandle.offset_;
          assert(err_writer(&bufHandle, errId, errInfo.type, true) == true);
          if(offset__last + 5 < bufHandle.offset_)                                  // даже если описание пустое - как минимум три слэша и два кода
          {
            errors_written_counter++;
          }
        }
        // проверяем, что такая ошибка была только одна
          assert(errors_written_counter == 1);

        // проверяем, что её обработали
          assert(err_get_info(&errInfo, errId) == true);
          assert(errInfo.unhandled == false);
        // отчищаем буфер
          assert(buf_clear(&bufHandle) == true);
      }   
      printf("  PASS: type filter 1.\n");

      // сброс ошибок
        err_reset_all();
    }
