#include "j_buffer_test.h"

void j_buffer_test()
{
  printf("j_buffer_test.c: ЗАПУСК ТЕСТИРОВАНИЯ\n");

  unsigned int array_is_full_footer_msg_size = 1 + snprintf(NULL, 0, "%s", ARRAY_IS_FULL_FOOTER_MSG);
  char char_buffer_arr[array_is_full_footer_msg_size];
  struct CharBuffer charBuffer = {
    .arr_ptr = char_buffer_arr,
    .arr_size = sizeof(char_buffer_arr),
    .offset = 0
  };

  // Тест инициализации буфера
    assert(is_buffer_empty(&charBuffer) == true);
    assert(buffer_size_left(&charBuffer) == sizeof(char_buffer_arr));
    printf("PASS: Инициализация буфера, размер %d байта.\n", charBuffer.arr_size);

  // Тест записи символов в буфер
    const char *test_msg = "Hey!";
    assert(buffer_write_char(&charBuffer, test_msg) == true);
    assert(strcmp(charBuffer.arr_ptr, test_msg) == 0);
    assert(charBuffer.offset == strlen(test_msg));
    assert(is_buffer_empty(&charBuffer) == false);
    printf("PASS: Запись первой строки.\n");

  // Тест записи int в буфер
    int test_int = 123;
    assert(buffer_write_int(&charBuffer, test_int) == true);
    assert(strcmp(charBuffer.arr_ptr, "Hey!123") == 0);
    printf("PASS: Запись int.\n");

  // Тест записи float в буфер
    float test_float = 45.67f;
    assert(buffer_write_float(&charBuffer, test_float) == true);
    // Из-за особенностей форматирования float, сравниваем только начало строки
    assert(strncmp(charBuffer.arr_ptr, "Hey!12345.67", strlen("Hey!12345.67")) == 0);
    printf("PASS: Запись float.\n");

  // Тест очистки буфера
    assert(buffer_clear(&charBuffer) == true);
    assert(is_buffer_empty(&charBuffer) == true);
    assert(charBuffer.arr_ptr[0] == '\0');
    printf("PASS: Очистка буфера.\n");

  // Тест переполнения буфера
    // Заполняем буфер почти до конца
    for (unsigned int i = 0; i < charBuffer.arr_size - 5; ++i)                  // Оставляем немного места
    {
      buffer_write_char(&charBuffer, "a");
    }
    // Пытаемся записать строку, которая переполнит буфер
    assert(buffer_write_char(&charBuffer, "THIS WILL OVERFLOW THE BUFFER") == false);
    assert(strcmp(charBuffer.arr_ptr, ARRAY_IS_FULL_FOOTER_MSG) == 0);
    assert(has_unhandled_errors() == true);
    assert(charBuffer.offset == charBuffer.arr_size - 1);
    printf("PASS: Переполнение буфера.\n");

  // Тесты с NULL указателями
    // Сброс ошибок для тестов с NULL
      for(int i = 0; i < J_ERRORS_AMOUNT; i++)
      {
        unhandled_errors[i] = false;
      }
      assert(has_unhandled_errors() == false);
    assert(buffer_clear(&charBuffer) == true);

    assert(buffer_write_char(NULL, "test") == false);
    assert(has_unhandled_errors() == true);
    assert(unhandled_errors[ERROR_ID_BUFFER_NULL] == true);
    printf("PASS: Обработка NULL указателя buffer_write_char().\n");
    unhandled_errors[ERROR_ID_BUFFER_NULL] = false; // Сброс ошибки

    assert(buffer_write_int(NULL, 123) == false);
    assert(has_unhandled_errors() == true);
    assert(unhandled_errors[ERROR_ID_BUFFER_NULL] == true);
    printf("PASS: Обработка NULL указателя buffer_write_int().\n");
    unhandled_errors[ERROR_ID_BUFFER_NULL] = false; // Сброс ошибки

    assert(buffer_write_float(NULL, 4.56) == false);
    assert(has_unhandled_errors() == true);
    assert(unhandled_errors[ERROR_ID_BUFFER_NULL] == true);
    printf("PASS: Обработка NULL указателя buffer_write_float().\n");
    unhandled_errors[ERROR_ID_BUFFER_NULL] = false; // Сброс ошибки    

    assert(buffer_clear(NULL) == false);
    assert(has_unhandled_errors() == true);
    assert(unhandled_errors[ERROR_ID_BUFFER_NULL] == true);
    printf("PASS: Обработка NULL указателя buffer_clear().\n");
    unhandled_errors[ERROR_ID_BUFFER_NULL] = false; // Сброс ошибки

    assert(is_buffer_empty(NULL) == false); // При NULL должен вернуть false и поднять ошибку
    assert(has_unhandled_errors() == true);
    assert(unhandled_errors[ERROR_ID_BUFFER_NULL] == true);
    printf("PASS: Обработка NULL is_buffer_empty().\n");
    unhandled_errors[ERROR_ID_BUFFER_NULL] = false; // Сброс ошибки

    assert(buffer_size_left(NULL) == 0); // При NULL должен вернуть 0 и поднять ошибку
    assert(has_unhandled_errors() == true);
    assert(unhandled_errors[ERROR_ID_BUFFER_NULL] == true);
    printf("PASS: Обработка NULL указателя buffer_size_left().\n");
    unhandled_errors[ERROR_ID_BUFFER_NULL] = false; // Сброс ошибки

  printf("j_buffer_test: ВСЕ ТЕСТЫ УСПЕШНО ПРОЙДЕНЫ!\n\n");
}