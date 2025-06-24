#include "j_errors_test.h"

void j_errors_test()
{
  #define VALID_ID ERR_ID__UNDEFINED
  #define INVALID_ID J_UNSIGNED_SHORT_MAX

  printf("j_errors_test.c: ЗАПУСК ТЕСТИРОВАНИЯ\n");

  // Тест описания ошибок на NULL
    for(int id = 0; id < J_ERRORS_AMOUNT; id++)
    {
      assert(get_error_description_ptr(id) != NULL);
    }
    printf("  PASS: description_ptr != NULL.\n");

  // Тест поднятия ошибок
    for(int id = 0; id < J_ERRORS_AMOUNT; id++)
    {
      raise_error(id);
      assert(get_unhandled_error_flag(id) == true);
      assert(get_error_counter(id) == 1);
    }
    printf("  PASS: Регистрация ошибок.\n");

  // Тест невалидного ID (отрицательное значение)
    for(int id = 0; id > -1000; id--)
    {
      raise_error(id);
    }
    assert(get_error_counter(ERR_ID__WRONG_ERR_ID) == 1000 + 1);
    printf("  PASS: Отправка Невалидного ID (меньше 0).\n");

  // Тест описания ошибки на неправильный ID
    assert(get_error_description_ptr(INVALID_ID) == get_error_description_ptr(ERR_ID__WRONG_ERR_ID));
    printf("  PASS: непрвильный ID -> правильный указатель.\n");

  // Тест невалидного ID (значение больше количества ошибок)
    assert(get_error_counter(J_ERRORS_AMOUNT) == UNSIGNED_SHORT_BAD_RETURN_55555);
    printf("  PASS: Отправка Невалидного ID (больше J_ERRORS_AMOUNT).\n");

  // Тест переполнения
    for(int i = 0; i < J_UNSIGNED_SHORT_MAX + 1000; i++)
    {
      raise_error(VALID_ID);
    }
    assert(get_error_counter(VALID_ID) == J_UNSIGNED_SHORT_MAX);
    printf("  PASS: Переполнение буфера.\n");

  // Тест сброса ошибок
    for(int id = 0; id < J_ERRORS_AMOUNT; id++)
    {
      reset_error_counter(id);
      assert(get_error_counter(id) == false);
      reset_unhandled_error_flag(id);
    }
    assert(has_unhandled_errors() == false);
    printf("  PASS: Сброс ошибок через цикл.\n");

  // Тест типов
    for(int id = 0; id < J_ERRORS_AMOUNT; id++)
    {
      ErrorsTypes testType = get_error_type(id);
      raise_error(id);
      assert(has_unhandled_errors_type(testType) == true);
      reset_error_counter(id);
      reset_unhandled_error_flag(id);
    }
    printf("  PASS: Типы ошибок.\n");

  printf("j_errors_test: ВСЕ ТЕСТЫ УСПЕШНО ПРОЙДЕНЫ!\n\n");
}