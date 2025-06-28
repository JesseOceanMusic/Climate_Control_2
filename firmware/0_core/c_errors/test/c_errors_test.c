#include "c_errors_test.h"

void c_errors_test()
{
  #define VALID_ID ERR_ID__UNDEFINED
  #define INVALID_ID ERR_USHORT_MAX

  printf("c_errors_test.c: init.\n");
  struct ErrInfo errInfo;

  // Тест инициализации ошибок
    for(ErrId id = 0; id < ERR_ID__AMOUNT; id++)
    {
      errInfo = err_get_info(id);
      assert(errInfo.id == id);
      assert(errInfo.was_id_correct == true);
      assert(errInfo.description_ptr != NULL);
      assert(errInfo.counter_unhandled == 0);
      assert(errInfo.counter_total == 0);
      assert(errInfo.unhandled == false);
      assert(errInfo.type != ERR_TYPE__ANY_TYPE);
      assert(errInfo.type != ERR_TYPE__AMOUNT);      
    }
    printf("  PASS: errors id.\n");

  // Тест наличия ERR_ID__ERR_WRONG_TYPE, ERR_ID__ERR_WRONG_ID
    errInfo = err_get_info(ERR_ID__ERR_WRONG_ID);
    assert(errInfo.was_id_correct == true);
    errInfo = err_get_info(ERR_ID__ERR_WRONG_TYPE);
    assert(errInfo.was_id_correct == true);
    printf("  PASS: errors id.\n");

  // Тест, что ошибки вообще записаны в c_errors_list.def
    assert(ERR_ID__AMOUNT != 0);
    printf("  PASS: errors id.\n");

  // Тест, что есть типы ошибок
    assert(ERR_TYPE__AMOUNT != 0);
    printf("  PASS: errors types.\n");

  // Тест поднятие ошибок
    for(ErrId id = 0; id < ERR_ID__AMOUNT; id++)
    {
      assert(err_raise_error(id) == true);
      errInfo = err_get_info(id);
      
      assert(err_has_unhandled_errors(ERR_TYPE__ANY_TYPE) == true);
      assert(errInfo.was_id_correct == true);
      assert(errInfo.id == id);
      assert(errInfo.type != ERR_TYPE__AMOUNT);
      assert(errInfo.type != ERR_TYPE__ANY_TYPE);
      assert(errInfo.description_ptr != NULL);
      assert(errInfo.unhandled == true);
      assert(errInfo.counter_unhandled == 1);
      assert(errInfo.counter_total == 1);
    }
    printf("  PASS: err_raise_error().\n");

  // Тест сброса
    for(ErrId id = 0; id < ERR_ID__AMOUNT; id++)
    {
      assert(err_reset_counter_and_flag(id) == true);
      errInfo = err_get_info(id);

      assert(errInfo.was_id_correct == true);
      assert(errInfo.id == id);
      assert(errInfo.type != ERR_TYPE__AMOUNT);
      assert(errInfo.type != ERR_TYPE__ANY_TYPE);
      assert(errInfo.description_ptr != NULL);
      assert(errInfo.unhandled == false);
      assert(errInfo.counter_unhandled == 0);
      assert(errInfo.counter_total == 1);
    }
    assert(err_has_unhandled_errors(ERR_TYPE__ANY_TYPE) == false);
    printf("  PASS: err_has_unhandled_errors().\n");

  // Тест неправильного ID
    // отправляем неправильный айди и смотрим инфо
      assert(err_reset_counter_and_flag(ERR_ID__AMOUNT) == false);   // первый неправильный ID
      assert(err_reset_counter_and_flag(-1) == false);               // второй неправильный ID
      errInfo = err_get_info(ERR_ID__AMOUNT);                        // третий неправильный ID

      assert(errInfo.was_id_correct == false);
      assert(errInfo.type != ERR_TYPE__AMOUNT);
      assert(errInfo.type != ERR_TYPE__ANY_TYPE);
      assert(errInfo.description_ptr != NULL);
      printf("  PASS: wrong id.\n");

    // смотрим зарегестрировалась ли ошибка
      errInfo = err_get_info(ERR_ID__ERR_WRONG_ID);

      assert(err_has_unhandled_errors(ERR_TYPE__ANY_TYPE) == true);
      assert(errInfo.was_id_correct == true);
      assert(errInfo.id == ERR_ID__ERR_WRONG_ID);
      assert(errInfo.type != ERR_TYPE__AMOUNT);
      assert(errInfo.type != ERR_TYPE__ANY_TYPE);
      assert(errInfo.description_ptr != NULL);
      assert(errInfo.unhandled == true);
      assert(errInfo.counter_unhandled == 3);
      printf("  PASS: register wrong id.\n");

  // Тест неправильного типа ошибки
    //сбрасиваем счётчик
      assert(err_reset_counter_and_flag(ERR_ID__ERR_WRONG_TYPE) == true);

    // Отправляем неправильный тип ошибки
      assert(err_has_unhandled_errors(ERR_TYPE__AMOUNT) == false);   // 1
      printf("  PASS: wrong types.\n");
    
    // Проверяем
      errInfo = err_get_info(ERR_ID__ERR_WRONG_TYPE);
      assert(errInfo.was_id_correct == true);
      assert(errInfo.id == ERR_ID__ERR_WRONG_TYPE);
      assert(errInfo.description_ptr != NULL);
      assert(errInfo.unhandled == true);
      assert(errInfo.counter_unhandled == 1);     // +1
      printf("  PASS: register wrong types.\n");

  // Тест фильтрации
    // сброс
      for(ErrId id = 0; id < ERR_ID__AMOUNT; id++)
      {
        assert(err_reset_counter_and_flag(id) == true);
      }

    // тест фильтров
      for(ErrId id = 0; id < ERR_ID__AMOUNT; id++)
      {
        // узнаём тип ошибки с id
          errInfo = err_get_info(id);
        ErrType type =  errInfo.type;
        // поднимаем ошибку
          assert(err_raise_error(id) == true);
        // проверяем фильтр
          assert(err_has_unhandled_errors(type) == true);
        // проверяем, что другие фильтры не реагируют
          for(ErrType type_2; type_2 < ERR_TYPE__AMOUNT; type_2++)
          {
            if(type != type_2 && type_2 != ERR_TYPE__ANY_TYPE)
            {
              assert(err_has_unhandled_errors(type) == false);
            }
          }
        // cбрасываем
          assert(err_reset_counter_and_flag(id) == true);
      }
      printf("  PASS: filter by type.\n");

    // Тест переполнения
      for(ErrId id = 0; id < ERR_USHORT_MAX + 1000; id++)
      {
        err_raise_error(ERR_ID__ERR_WR_UNLISTED_ERROR_TYPE);
      }
      errInfo = err_get_info(ERR_ID__ERR_WR_UNLISTED_ERROR_TYPE);
      assert(errInfo.counter_unhandled == ERR_USHORT_MAX);
      assert(errInfo.counter_total     == ERR_USHORT_MAX);
      printf("  PASS: counter overfill.\n");

    // Полный сброс, тест функций для тестирования (должен быть определен макрос TURN_ON_TEST_FEATURES в tasks.json)
      err_reset_all();
      for(ErrId id = 0; id < ERR_ID__AMOUNT; id++)
      {
        errInfo = err_get_info(id);
        assert(errInfo.counter_unhandled == 0);
        assert(errInfo.counter_total == 0);
        assert(errInfo.unhandled == false);
      }
      printf("  PASS: testing feature - err_reset_all().\n");

  #undef VALID_ID
  #undef INVALID_ID
  printf("c_errors_test: done.\n\n");
}