#include "j_errors_test.h"

void j_errors_test()
{
  #define VALID_ID ERROR_ID_UNDEFINED
  #define INVALID_ID J_UNSIGNED_SHORT_MAX

  printf("j_errors_test.c: ЗАПУСК ТЕСТИРОВАНИЯ\n");

  // Тест поднятия ошибки
    raise_error(VALID_ID);
    assert(unhandled_errors[VALID_ID] == true);
    assert(get_error_counter(VALID_ID) == 1);

    printf("PASS: Регистрация ошибки.\n");

  // Тест записи флагов ошибок
    assert(has_unhandled_errors() == true);

    printf("PASS: Запись флагов ошибок.\n");

  // Тест невалидного ID
    assert(get_error_counter(INVALID_ID) == UNSIGNED_SHORT_BAD_RETURN_55555);
    printf("PASS: Отправка Невалидного ID.\n");

  // Тест описания ошибки
    assert(get_error_description_ptr(VALID_ID) != NULL);
    assert(get_error_description_ptr(INVALID_ID) == get_error_description_ptr(ERROR_ID_WRONG_ERROR_ID));
    printf("PASS: Вывод текстового описание ошибок.\n");

  // Тест сброса ошибок
    for(int i = 0; i < J_ERRORS_AMOUNT; i++)
    {
      unhandled_errors[i] = false;
    }
    assert(has_unhandled_errors() == false);
    printf("PASS: Сброс ошибок через цикл.\n");

  // Тест переполнения
    for(int i = 0; i < J_UNSIGNED_SHORT_MAX + 1000; i++)
    {
      raise_error(VALID_ID);
    }
    assert(get_error_counter(VALID_ID) == J_UNSIGNED_SHORT_MAX);
    printf("PASS: Переполнение буфера.\n");

  printf("j_errors_test: ВСЕ ТЕСТЫ УСПЕШНО ПРОЙДЕНЫ!\n\n");
}