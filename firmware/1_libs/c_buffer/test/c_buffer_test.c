#include "c_buffer_test.h"

void c_buffer_test()
{
    struct ErrInfo errInfo;

  // создаём буфер (должен быть static и выделятся вне функций, здесь только для теста)
    #define TEST_BUF_SIZE 128
      char char_buffer_arr[TEST_BUF_SIZE];
      struct BufHandle BufHandle;
      assert(buf_init(&BufHandle, char_buffer_arr, sizeof(char_buffer_arr)) == true);
      printf("  PASS: buf_init().\n");
    #undef TEST_BUF_SIZE

  // Тест инициализации буфера
    assert(BufHandle.arr_ptr == char_buffer_arr);
    assert(BufHandle.arr_size_ == sizeof(char_buffer_arr));
    assert(BufHandle.is_empty == true);
    assert(BufHandle.offset_ == 0);
    assert(BufHandle.size_left == BufHandle.arr_size_ - 1);
    printf("  PASS: buffer init check - size is %d byte.\n", BufHandle.arr_size_);

  // Тест записи символов в буфер
    const char* test_msg = "Hey!";
    assert(buf_write_char(&BufHandle, test_msg) == true);
    assert(strcmp(BufHandle.arr_ptr, test_msg) == 0);
    assert(BufHandle.offset_ == strlen(test_msg));
    assert(BufHandle.is_empty == false);
    printf("  PASS: buf_write_char().\n");

  // Тест очистки буфера
    assert(buf_clear(&BufHandle) == true);
    assert(BufHandle.is_empty == true);
    assert(BufHandle.offset_ == 0);
    assert(BufHandle.size_left == BufHandle.arr_size_ - 1);
    assert(BufHandle.arr_ptr[0] == '\0');
    printf("  PASS: buf_clear().\n");

  // Тест записи int в буфер
    int test_int = 123;
    assert(buf_write_int(&BufHandle, test_int) == true);
    assert(strcmp(BufHandle.arr_ptr, "123") == 0);
    printf("  PASS: buf_write_int().\n");

  // Тест записи float в буфер
    float test_float = 45.67f;
    assert(buf_write_float(&BufHandle, test_float, 2) == true);
    assert(strcmp(BufHandle.arr_ptr, "12345.67") == 0);
    printf("  PASS: buf_write_float().\n");

  // Тест float с разной точностью.
    assert(buf_clear(&BufHandle) == true);

    test_float = 12.1234f;
    assert(buf_write_float(&BufHandle, test_float, 0) == true);
    assert(strcmp(BufHandle.arr_ptr, "12") == 0);
    printf("  PASS: buf_write_float() - 0 precision.\n");

    unsigned short wrong_precision = BUF_FLOAT_MAX_PRECISION + 1;
    assert(buf_write_float(&BufHandle, test_float, wrong_precision) == false);
    assert(strcmp(BufHandle.arr_ptr, "12") == 0);                  // должно остаться предыдущее значение
    assert(err_get_info(&errInfo, ERR_ID__BUF_BAD_FLOAT_PRECISION) == true);                             
    assert(errInfo.unhandled == true);                                          // проверяем, что ошибка NULL зарегестрированна
    printf("  PASS: buf_write_float() - bad precision.\n");

    if(BUF_FLOAT_MAX_PRECISION >= 6)
    {
      assert(buf_clear(&BufHandle) == true);
      test_float = 12.123456f; 
      assert(buf_write_float(&BufHandle, test_float, 6) == true);
      assert(strcmp(BufHandle.arr_ptr, "12.123456") == 0);
      printf("  PASS: buf_write_float() - precision = 6.\n");
    }

  // Тест переполнения буфера
    // Заполняем буфер полностью
    assert(buf_clear(&BufHandle) == true);
    for (unsigned int i = 0; i < BufHandle.arr_size_ - 1;  i++)
    {
      assert(buf_write_char(&BufHandle, "a") == true);
    }
    // Переполняем буфер
    assert(buf_write_char(&BufHandle, "a") == false);
    printf("  PASS: buffer overfill.\n");

  // Тесты с NULL указателями
    // Сброс ошибок для тестов с NULL
      for(ErrId id = 0; id < ERR_ID__AMOUNT; id++)
      {
        assert(err_reset_counter_and_flag(id) == true);
      }
      assert(err_has_unhandled_errors(ERR_TYPE__ANY_TYPE) == false);
    
    // buf_init()
      assert(err_reset_counter_and_flag(ERR_ID__BUF_RECEIVED_NULL) == true);    // Сброс ошибки
      assert(buf_init(NULL, NULL, 2) == false);                                 // отправляем NULL
      assert(buf_init(&BufHandle, NULL, 2) == false);                             // отправляем NULL
      assert(buf_init(NULL, char_buffer_arr, 2) == false);                      // отправляем NULL
      assert(err_get_info(&errInfo, ERR_ID__BUF_RECEIVED_NULL) == true);                             
      assert(errInfo.counter_unhandled == 3);                                   // проверяем, что ошибка NULL зарегестрированна
      printf("  PASS: NULL -> buf_init().\n");


    // buf_write_char()
      assert(err_reset_counter_and_flag(ERR_ID__BUF_RECEIVED_NULL) == true);    // Сброс ошибки
      assert(buf_write_char(NULL, "test") == false);                            // отправляем NULL
      assert(buf_write_char(&BufHandle, NULL) == false);                          // отправляем NULL
      assert(err_get_info(&errInfo, ERR_ID__BUF_RECEIVED_NULL) == true);                             
      assert(errInfo.counter_unhandled == 2);                                   // проверяем, что ошибка NULL зарегестрированна
      printf("  PASS: NULL -> buf_write_char().\n");
    
    // buf_write_int()
      assert(err_reset_counter_and_flag(ERR_ID__BUF_RECEIVED_NULL) == true);    // Сброс ошибки
      assert(buf_write_int(NULL, 123) == false);                                // отправляем NULL
      assert(err_get_info(&errInfo, ERR_ID__BUF_RECEIVED_NULL) == true);                             
      assert(errInfo.counter_unhandled == 1);                                   // проверяем, что ошибка NULL зарегестрированна
      printf("  PASS: NULL -> buf_write_int().\n");
    
    // buf_write_float()
      assert(err_reset_counter_and_flag(ERR_ID__BUF_RECEIVED_NULL) == true);    // Сброс ошибки
      assert(buf_write_float(NULL, 4.56, 2) == false);                          // отправляем NULL
      assert(err_get_info(&errInfo, ERR_ID__BUF_RECEIVED_NULL) == true);                             
      assert(errInfo.counter_unhandled == 1);                                    // проверяем, что ошибка NULL зарегестрированна
      printf("  PASS: NULL -> buf_write_float().\n");

    // buf_clear()
      assert(err_reset_counter_and_flag(ERR_ID__BUF_RECEIVED_NULL) == true);    // Сброс ошибки
      assert(buf_clear(NULL) == false);                                         // отправляем NULL
      assert(err_get_info(&errInfo, ERR_ID__BUF_RECEIVED_NULL) == true);                             
      assert(errInfo.counter_unhandled == 1);                                   // проверяем, что ошибка NULL зарегестрированна
      printf("  PASS: NULL -> buf_clear().\n");

  // Сброс ошибок
    err_reset_all();
}