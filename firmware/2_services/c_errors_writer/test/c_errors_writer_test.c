#include <stdio.h>
#include <assert.h>
#include <string.h>                                                             // Для strcmp

#include "c_errors_internal.h"                                                  // internal а не public для проверки фильтра
#include "c_errors_writer.h"
#include "c_errors_writer_test.h"

void c_errors_writer_test()
{
  // создаём структуру и сбрасываем ошибки
    struct ErrInfo errInfo;
    assert(buf_clear(BUF_ID__TG_MSG) == true);
    err_reset_all();

  // invalid buf id
    assert(err_writer__write_one_err(BUF_ID__AMOUNT, (ErrId)0, (ErrType)0, false) == false);
    assert(err_has_unhandled_errors(ERR_TYPE__ANY_TYPE) == true);
    err_reset_all();
    printf("  PASS: invalid buf id.\n");

  // invalid err id
    assert(err_writer__write_one_err((BufId)0, ERR_ID__AMOUNT, (ErrType)0, false) == false);
    assert(err_has_unhandled_errors(ERR_TYPE__ANY_TYPE) == true);
    err_reset_all();
    printf("  PASS: invalid err id.\n");

  // invalid err type
    assert(err_writer__write_one_err((BufId)0, (ErrId)0, ERR_TYPE__AMOUNT, false) == false);
    assert(err_has_unhandled_errors(ERR_TYPE__ANY_TYPE) == true);
    err_reset_all();
    printf("  PASS: invalid err type.\n");

  // нет ошибок, вывод unhandled (ничего не должен записать)

    for(ErrType errType = 0; errType < ERR_TYPE__AMOUNT; errType++)
    {
      for(ErrId errId = 0; errId < ERR_ID__AMOUNT; errId ++)
      {
        assert(err_writer__write_one_err(BUF_ID__TG_MSG, errId, errType, true) == true);
        assert(buf_get_is_empty(BUF_ID__TG_MSG)== true);
      }
    }

    printf("  PASS: unhandled filter.\n");

  // запись всех ошибок
    assert(buf_write_char(BUF_ID__TG_MSG, ERR_HEADER_MESSAGE));
    for(ErrId errId = 0; errId < ERR_ID__AMOUNT; errId ++)
    {
      assert(err_writer__write_one_err(BUF_ID__TG_MSG, errId, ERR_TYPE__ANY_TYPE, false) == true);
    }

    printf("  PASS: errors list is written to buffer, size left: %d bytes.\n", buf_get_size_left(BUF_ID__TG_MSG));
    assert(buf_clear(BUF_ID__TG_MSG) == true);

  // проверка фильтров
    for(ErrId errId = 0; errId < ERR_ID__AMOUNT; errId ++)
    {   
      // берем одну ошибку и поднимаем флаг
        assert(err_raise_error(errId) == true);
        assert(err_get_info(&errInfo, errId) == true);

      // проверяем что другие фильтры не реагируют
        for(ErrType errType = (ErrType)0; errType < ERR_TYPE__AMOUNT; errType++)
        {
          if(errType != errInfo.type && errType != ERR_TYPE__ANY_TYPE)
          {
            for(ErrId errId2 = 0; errId2 < ERR_ID__AMOUNT; errId2 ++)
            {
              assert(err_writer__write_one_err(BUF_ID__TG_MSG, errId2, errType, true) == true);
              assert(buf_get_is_empty(BUF_ID__TG_MSG) == true);
            }
          }
        }

      // проверяем, что нужный фильтр найдет только одну ошибку, как и должен
        int errors_written_counter = 0;
        for(ErrId errId = (ErrId)0; errId < ERR_ID__AMOUNT; errId ++)
        {
          unsigned int size_left_last = buf_get_size_left(BUF_ID__TG_MSG);
          assert(err_writer__write_one_err(BUF_ID__TG_MSG, errId, errInfo.type, true) == true);
          if(size_left_last > buf_get_size_left(BUF_ID__TG_MSG))
          {
            errors_written_counter++;
          }
        }
        // проверяем, что такая ошибка была только одна
          assert(errors_written_counter == 1);

        // проверяем, что её обработали
          assert(err_has_unhandled_errors(ERR_TYPE__ANY_TYPE) == false);
      
        // сброс ошибок и буфера
          assert(buf_clear(BUF_ID__TG_MSG) == true);
          err_reset_all();
      }   
      printf("  PASS: type filter.\n");
    }
