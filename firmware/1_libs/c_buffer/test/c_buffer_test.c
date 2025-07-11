#include <stdio.h>
#include <assert.h>
#include <string.h>                                                           // Для strcmp

#include "c_buffer.h"
#include "c_errors_public.h"
#include "c_buffer_test.h"

void c_buffer_test()
{
  // Тест инициализации буферов
    assert(BUF_ID__AMOUNT > 0);
    assert(BUF_ID__AMOUNT > BUF_ID__TG_MSG);
    for(BufId bufId = 0; bufId < BUF_ID__AMOUNT; bufId++)
    {
      assert(buf_get_arr_ptr(bufId) != NULL);
      assert(buf_get_size_left(bufId) > 0);
      assert(buf_get_is_empty(bufId) == true);
      printf("  PASS: buffer init(id:%d, size available:%d byte).\n", bufId, buf_get_size_left(bufId));
    }
    

  // Тест записи символов в буфер
    char test_msg[] = "Hey!";
    unsigned int buf_size = buf_get_size_left(BUF_ID__TG_MSG);

    assert(buf_write_char(BUF_ID__TG_MSG, test_msg) == true);

    assert(buf_get_is_empty(BUF_ID__TG_MSG) == false);
    assert(strcmp(buf_get_arr_ptr(BUF_ID__TG_MSG), test_msg) == 0);
    assert(buf_size - buf_get_size_left(BUF_ID__TG_MSG) == strlen(test_msg));

    printf("  PASS: buf_write_char().\n");

  // Тест очистки буфера
    assert(buf_clear(BUF_ID__TG_MSG) == true);
    assert(buf_get_is_empty(BUF_ID__TG_MSG) == true);
    assert(buf_size == buf_get_size_left(BUF_ID__TG_MSG));
    assert(buf_get_arr_ptr(BUF_ID__TG_MSG)[0] == '\0');
    printf("  PASS: buf_clear().\n");

  // Тест записи int в буфер
    int test_int = 123;
    assert(buf_write_int(BUF_ID__TG_MSG, test_int) == true);
    assert(strcmp(buf_get_arr_ptr(BUF_ID__TG_MSG), "123") == 0);
    printf("  PASS: buf_write_int().\n");

  // Тест записи float в буфер
    float test_float = 45.67f;
    assert(buf_write_float(BUF_ID__TG_MSG, test_float, 2) == true);
    assert(strcmp(buf_get_arr_ptr(BUF_ID__TG_MSG), "12345.67") == 0);
    printf("  PASS: buf_write_float().\n");

  // Тест float с разной точностью.
    assert(buf_clear(BUF_ID__TG_MSG) == true);

    test_float = 12.1234f;
    assert(buf_write_float(BUF_ID__TG_MSG, test_float, 0) == true);
    assert(strcmp(buf_get_arr_ptr(BUF_ID__TG_MSG), "12") == 0);
    printf("  PASS: buf_write_float() - 0 precision.\n");

    unsigned short wrong_precision = BUF_FLOAT_MAX_PRECISION + 1;
    assert(buf_write_float(BUF_ID__TG_MSG, test_float, wrong_precision) == false);
    assert(strcmp(buf_get_arr_ptr(BUF_ID__TG_MSG), "12") == 0);                  // должно остаться предыдущее значение
    assert(err_has_unhandled_errors(ERR_TYPE__ANY_TYPE) == true);
    assert(err_get_total_counter(ERR_ID__BUF_WRONG_FLOAT_PRECISION) == 1);
    err_reset_all();
    printf("  PASS: buf_write_float() - bad precision.\n");

    if(BUF_FLOAT_MAX_PRECISION >= 6)
    {
      assert(buf_clear(BUF_ID__TG_MSG) == true);
      test_float = 12.123456f; 
      assert(buf_write_float(BUF_ID__TG_MSG, test_float, 6) == true);
      assert(strcmp(buf_get_arr_ptr(BUF_ID__TG_MSG), "12.123456") == 0);
      printf("  PASS: buf_write_float() - precision = 6.\n");
    }

  // Тест переполнения буфера
    // Заполняем буфер полностью
      assert(buf_clear(BUF_ID__TG_MSG) == true);
      for (unsigned int i = buf_get_size_left(BUF_ID__TG_MSG); i > 0;  i--)
      {
        assert(buf_write_char(BUF_ID__TG_MSG, "a") == true);
      }
    // Переполняем буфер
      assert(buf_write_char(BUF_ID__TG_MSG, "a") == false);
      printf("  PASS: buffer overfill.\n");

  // Невалидные аргументы
    // Сброс ошибок для тестов с NULL
      err_reset_all();

    // NULL > > buf_write_char()
      assert(buf_write_char(BUF_ID__TG_MSG, NULL) == false);
      assert(err_has_unhandled_errors(ERR_TYPE__ANY_TYPE) == true);
      assert(err_get_total_counter(ERR_ID__BUF_RECEIVED_NULL) == 1);
      err_reset_all();
      printf("  PASS: NULL > > buf_write_char().\n");

    // NULL > > buf_will_this_data_fit()
      assert(buf_will_this_data_fit(BUF_ID__TG_MSG, NULL) == false);
      assert(err_has_unhandled_errors(ERR_TYPE__ANY_TYPE) == true);
      assert(err_get_total_counter(ERR_ID__BUF_RECEIVED_NULL) == 1);
      err_reset_all();
      printf("  PASS: NULL > > buf_will_this_data_fit().\n");

    // Wrong ID
      assert(buf_write_char        (BUF_ID__AMOUNT, "hey") == false);           // 1
      assert(buf_write_int         (BUF_ID__AMOUNT, 123) == false);             // 2
      assert(buf_write_float       (BUF_ID__AMOUNT, 2, 0) == false);            // 3
      assert(buf_clear             (BUF_ID__AMOUNT) == false);                  // 4
      assert(buf_get_arr_ptr       (BUF_ID__AMOUNT) == BUF_BAD_ID);             // 5 придёт указатель на BUF_BAD_ID
      assert(buf_get_size_left     (BUF_ID__AMOUNT) == 0);                      // 6
      assert(buf_get_is_empty      (BUF_ID__AMOUNT) == false);                  // 7
      assert(buf_is_id_correct     (BUF_ID__AMOUNT) == false);                  // 8
      assert(buf_will_this_data_fit(BUF_ID__AMOUNT, "hey") == false);           // 9

      assert(err_has_unhandled_errors(ERR_TYPE__ANY_TYPE) == true);
      assert(err_get_total_counter(ERR_ID__BUF_RECEIVED_WRONG_ID) == 9);
      printf("  PASS: wrong id.\n");
  // Сброс ошибок
    err_reset_all();
}