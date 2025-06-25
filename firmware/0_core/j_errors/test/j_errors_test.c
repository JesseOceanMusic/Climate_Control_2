#include "j_errors_test.h"

static struct ErrInfo errInfo;

void j_errors_test()
{
  #define VALID_ID ERR_ID__UNDEFINED
  #define INVALID_ID J_UNSIGNED_SHORT_MAX

  printf("j_errors_test.c: test init.\n");

  // Тест инициализации ошибок
    for(ErrId id = 0; id < ERR_ID__AMOUNT; id++)
    {
      errInfo = err_get_info(id);
      assert( errInfo.id == id);
      assert( errInfo.was_id_correct == true);
      assert( errInfo.description_ptr != NULL);
      assert( errInfo.counter_cur == 0);
      assert( errInfo.counter_last == 0);
      assert( errInfo.unhandled == false);
      assert( errInfo.type != ERR_TYPE__ANY_TYPE);
      assert( errInfo.type != ERR_TYPE__AMOUNT);      
    }
    printf("  PASS: errors id.\n");

  // Тест наличия ERR_ID__WRONG_ERR_TYPE, ERR_ID__WRONG_ERR_ID
    errInfo = err_get_info(ERR_ID__WRONG_ERR_ID);
    assert( errInfo.was_id_correct == true);
    errInfo = err_get_info(ERR_ID__WRONG_ERR_TYPE);
    assert( errInfo.was_id_correct == true);
    printf("  PASS: errors id.\n");

  // Тест, что ошибки вообще записаны в j_errors_list.def
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
      assert( errInfo.was_id_correct == true);
      assert( errInfo.id == id);
      assert( errInfo.type != ERR_TYPE__AMOUNT);
      assert( errInfo.type != ERR_TYPE__ANY_TYPE);
      assert( errInfo.description_ptr != NULL);
      assert( errInfo.unhandled == true);
      assert( errInfo.counter_cur == 1);
      assert( errInfo.counter_last == 0);
    }
    printf("  PASS: err_raise_error().\n");

  // Тест сброса
    for(ErrId id = 0; id < ERR_ID__AMOUNT; id++)
    {
      assert(err_reset_counter_and_flag(id) == true);
      errInfo = err_get_info(id);

      assert( errInfo.was_id_correct == true);
      assert( errInfo.id == id);
      assert( errInfo.type != ERR_TYPE__AMOUNT);
      assert( errInfo.type != ERR_TYPE__ANY_TYPE);
      assert( errInfo.description_ptr != NULL);
      assert( errInfo.unhandled == false);
      assert( errInfo.counter_cur == 1);
      assert( errInfo.counter_last == 1);
    }
    assert(err_has_unhandled_errors(ERR_TYPE__ANY_TYPE) == false);
    printf("  PASS: err_has_unhandled_errors().\n");

  // Тест неправильного ID
    // отправляем неправильный айди и смотрим инфо
      assert(err_reset_counter_and_flag(ERR_ID__AMOUNT) == false);   // первый неправильный ID
      errInfo = err_get_info(ERR_ID__AMOUNT);                        // второй неправильный ID

      assert( errInfo.was_id_correct == false);
      assert( errInfo.type != ERR_TYPE__AMOUNT);
      assert( errInfo.type != ERR_TYPE__ANY_TYPE);
      assert( errInfo.description_ptr != NULL);
      printf("  PASS: wrong id.\n");

    // смотрим зарегестрировалась ли ошибка
      errInfo = err_get_info(ERR_ID__WRONG_ERR_ID);

      assert(err_has_unhandled_errors(ERR_TYPE__ANY_TYPE) == true);
      assert( errInfo.was_id_correct == true);
      assert( errInfo.id == ERR_ID__WRONG_ERR_ID);
      assert( errInfo.type != ERR_TYPE__AMOUNT);
      assert( errInfo.type != ERR_TYPE__ANY_TYPE);
      assert( errInfo.description_ptr != NULL);
      assert( errInfo.unhandled == true);
      assert( errInfo.counter_cur ==  errInfo.counter_last + 2);
      printf("  PASS: register wrong id.\n");

  // Тест неправильного типа ошибки
    //сбрасиваем счётчик
      assert(err_reset_counter_and_flag(ERR_ID__WRONG_ERR_TYPE) == true);

    // Отправляем неправильные типы
      assert(err_has_unhandled_errors(ERR_TYPE__AMOUNT) == false);   // 1
      printf("  PASS: wrong types.\n");
    
    // Проверяем
      errInfo = err_get_info(ERR_ID__WRONG_ERR_TYPE);
      assert( errInfo.was_id_correct == true);
      assert( errInfo.id == ERR_ID__WRONG_ERR_TYPE);
      assert( errInfo.description_ptr != NULL);
      assert( errInfo.unhandled == true);
      assert( errInfo.counter_cur ==  errInfo.counter_last + 1);     // +1
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
      for(int i = 0; i < J_UNSIGNED_SHORT_MAX + 1000; i++)
      {
        err_raise_error(ERR_ID__NO_ERR_WITH_THIS_TYPE);
      }
      errInfo = err_get_info(ERR_ID__NO_ERR_WITH_THIS_TYPE);
      assert( errInfo.counter_cur == J_UNSIGNED_SHORT_MAX);
      printf("  PASS: counter overfill.\n");
  
  printf("j_errors_test: done.\n\n");
}