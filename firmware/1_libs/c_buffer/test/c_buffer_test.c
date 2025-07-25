#include <stdio.h>
#include <assert.h>
#include <string.h>                                                           // Для strcmp

#include "c_buffer.h"
#include "c_errors_public.h"

int main()
{
  // Тест инициализации буферов
    assert(BUF__ID__AMOUNT > 0);
    assert(BUF__ID__AMOUNT > BUF__ID__TG_MSG);
    for(BufId bufId = 0; bufId < BUF__ID__AMOUNT; bufId++)
    {
      assert(buf__get_arr_ptr(bufId) != NULL);
      assert(buf__get_size_left(bufId) > 0);
      assert(buf__get_is_empty(bufId) == true);
      printf("  PASS: buffer init(id:%d, size available:%d byte).\n", bufId, buf__get_size_left(bufId));
    }
    

  // Тест записи символов в буфер
    char test_msg[] = "Hey!";
    unsigned int buf_size = buf__get_size_left(BUF__ID__TG_MSG);

    assert(buf__write_char(BUF__ID__TG_MSG, test_msg) == true);

    assert(buf__get_is_empty(BUF__ID__TG_MSG) == false);
    assert(strcmp(buf__get_arr_ptr(BUF__ID__TG_MSG), test_msg) == 0);
    assert(buf_size - buf__get_size_left(BUF__ID__TG_MSG) == strlen(test_msg));

    printf("  PASS: buf__write_char().\n");

  // Тест очистки буфера
    assert(buf__clear(BUF__ID__TG_MSG) == true);
    assert(buf__get_is_empty(BUF__ID__TG_MSG) == true);
    assert(buf_size == buf__get_size_left(BUF__ID__TG_MSG));
    assert(buf__get_arr_ptr(BUF__ID__TG_MSG)[0] == '\0');
    printf("  PASS: buf__clear().\n");

  // Тест записи int в буфер
    int test_int = 123;
    assert(buf__write_int(BUF__ID__TG_MSG, test_int) == true);
    assert(strcmp(buf__get_arr_ptr(BUF__ID__TG_MSG), "123") == 0);
    printf("  PASS: buf__write_int().\n");

  // Тест записи float в буфер
    float test_float = 45.67f;
    assert(buf__write_float(BUF__ID__TG_MSG, test_float, 2) == true);
    assert(strcmp(buf__get_arr_ptr(BUF__ID__TG_MSG), "12345.67") == 0);
    printf("  PASS: buf__write_float().\n");

  // Тест float с разной точностью.
    assert(buf__clear(BUF__ID__TG_MSG) == true);

    test_float = 12.1234f;
    assert(buf__write_float(BUF__ID__TG_MSG, test_float, 0) == true);
    assert(strcmp(buf__get_arr_ptr(BUF__ID__TG_MSG), "12") == 0);
    printf("  PASS: buf__write_float() - 0 precision.\n");

    unsigned short wrong_precision = BUF__FLOAT_MAX_PRECISION + 1;
    assert(buf__write_float(BUF__ID__TG_MSG, test_float, wrong_precision) == false);
    assert(strcmp(buf__get_arr_ptr(BUF__ID__TG_MSG), "12") == 0);                  // должно остаться предыдущее значение
    assert(err__has_unhandled_errors(ERR__TYPE__ANY_TYPE) == true);
    assert(err__get_total_counter(ERR__ID__BUF__WRONG_FLOAT_PRECISION) == 1);
    err__reset_all();
    printf("  PASS: buf__write_float() - bad precision.\n");

    if(BUF__FLOAT_MAX_PRECISION >= 6)
    {
      assert(buf__clear(BUF__ID__TG_MSG) == true);
      test_float = 12.123456f; 
      assert(buf__write_float(BUF__ID__TG_MSG, test_float, 6) == true);
      assert(strcmp(buf__get_arr_ptr(BUF__ID__TG_MSG), "12.123456") == 0);
      printf("  PASS: buf__write_float() - precision = 6.\n");
    }

  // Тест переполнения буфера
    // Заполняем буфер полностью
      assert(buf__clear(BUF__ID__TG_MSG) == true);
      for (unsigned int i = buf__get_size_left(BUF__ID__TG_MSG); i > 0;  i--)
      {
        assert(buf__write_char(BUF__ID__TG_MSG, "a") == true);
      }
    // Переполняем буфер
      assert(buf__write_char(BUF__ID__TG_MSG, "a") == false);
      printf("  PASS: buffer overfill.\n");

  // Невалидные аргументы
    // Сброс ошибок для тестов с NULL
      err__reset_all();

    // NULL > > buf__write_char()
      assert(buf__write_char(BUF__ID__TG_MSG, NULL) == false);
      assert(err__has_unhandled_errors(ERR__TYPE__ANY_TYPE) == true);
      assert(err__get_total_counter(ERR__ID__BUF__RECEIVED_NULL) == 1);
      err__reset_all();
      printf("  PASS: NULL > > buf__write_char().\n");

    // NULL > > buf__will_this_data_fit()
      assert(buf__will_this_data_fit(BUF__ID__TG_MSG, NULL) == false);
      assert(err__has_unhandled_errors(ERR__TYPE__ANY_TYPE) == true);
      assert(err__get_total_counter(ERR__ID__BUF__RECEIVED_NULL) == 1);
      err__reset_all();
      printf("  PASS: NULL > > buf__will_this_data_fit().\n");

    // Wrong ID
      assert(buf__write_char        (BUF__ID__AMOUNT, "hey") == false);           // 1
      assert(buf__write_int         (BUF__ID__AMOUNT, 123) == false);             // 2
      assert(buf__write_float       (BUF__ID__AMOUNT, 2, 0) == false);            // 3
      assert(buf__clear             (BUF__ID__AMOUNT) == false);                  // 4
      assert(buf__get_arr_ptr       (BUF__ID__AMOUNT) == BUF__BAD_ID);             // 5 придёт указатель на BUF__BAD_ID
      assert(buf__get_size_left     (BUF__ID__AMOUNT) == 0);                      // 6
      assert(buf__get_is_empty      (BUF__ID__AMOUNT) == false);                  // 7
      assert(buf__is_id_correct     (BUF__ID__AMOUNT) == false);                  // 8
      assert(buf__will_this_data_fit(BUF__ID__AMOUNT, "hey") == false);           // 9

      assert(err__has_unhandled_errors(ERR__TYPE__ANY_TYPE) == true);
      assert(err__get_total_counter(ERR__ID__BUF__RECEIVED_WRONG_ID) == 9);
      printf("  PASS: wrong id.\n");
  // Сброс ошибок
    err__reset_all();

  return 0;
}