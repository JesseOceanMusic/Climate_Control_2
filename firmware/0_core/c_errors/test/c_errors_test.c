#include <stdio.h>
#include <assert.h>

#include "c_errors_internal.h"

int main()
{
  #define VALID_ID ERR__ID__UNDEFINED
  #define INVALID_ID ERR__USHORT_MAX
  
  struct ErrInfo errInfo;

  // Тест инициализации ошибок
    for(ErrId id = 0; id < ERR__ID__AMOUNT; id++)
    {
      assert(err__get_info(&errInfo, id) == true);
      assert(errInfo.id == id);
      assert(errInfo.description_ptr != NULL);
      assert(errInfo.counter_unhandled == 0);
      assert(errInfo.counter_total == 0);
      assert(errInfo.unhandled == false);
      assert(errInfo.type != ERR__TYPE__ANY_TYPE);
      assert(errInfo.type < ERR__TYPE__AMOUNT); 
    }
    printf("  PASS: errors id.\n");

  // Тест наличия базовых ошибок
    assert(err__get_info(&errInfo, ERR__ID__UNDEFINED) == true);
    assert(err__get_info(&errInfo, ERR__ID__ERR__WRONG_ID) == true);
    assert(err__get_info(&errInfo, ERR__ID__ERR__WRONG_TYPE) == true);
    assert(err__get_info(&errInfo, ERR__ID__ERR__RECEIVED_NULL) == true);
    printf("  PASS: errors id.\n");

  // Тест, что ошибки вообще записаны в c_errors_list.def
    assert(ERR__ID__AMOUNT != 0);
    printf("  PASS: errors id.\n");

  // Тест, что есть типы ошибок
    assert(ERR__TYPE__AMOUNT != 0);
    printf("  PASS: errors types.\n");

  // Тест поднятие ошибок
    for(ErrId id = 0; id < ERR__ID__AMOUNT; id++)
    {
      assert(err__raise_error(id) == true);
      assert(err__get_info(&errInfo, id) == true);
      
      assert(err__has_unhandled_errors(ERR__TYPE__ANY_TYPE) == true);
      assert(errInfo.id == id);
      assert(errInfo.type != ERR__TYPE__AMOUNT);
      assert(errInfo.type != ERR__TYPE__ANY_TYPE);
      assert(errInfo.description_ptr != NULL);
      assert(errInfo.unhandled == true);
      assert(errInfo.counter_unhandled == 1);
      assert(errInfo.counter_total == 1);
    }
    printf("  PASS: err__raise_error().\n");

  // Тест сброса
    for(ErrId id = 0; id < ERR__ID__AMOUNT; id++)
    {
      //err__raise_error("hey");
      assert(err__reset_counter_and_flag(id) == true);
      assert(err__get_info(&errInfo, id) == true);
      assert(errInfo.id == id);
      assert(errInfo.type != ERR__TYPE__AMOUNT);
      assert(errInfo.type != ERR__TYPE__ANY_TYPE);
      assert(errInfo.description_ptr != NULL);
      assert(errInfo.unhandled == false);
      assert(errInfo.counter_unhandled == 0);
      assert(errInfo.counter_total == 1);
    }
    assert(err__has_unhandled_errors(ERR__TYPE__ANY_TYPE) == false);
    printf("  PASS: err__has_unhandled_errors().\n");

  // Тест неправильного ID
    // тест функции проверки id
      assert(err__is_id_correct(-1) == false);                                   // 1
      assert(err__is_id_correct(ERR__ID__AMOUNT) == false);                       // 2

    // отправляем неправильный айди и смотрим инфо
      assert(err__reset_counter_and_flag(-1) == false);                          // 3
      assert(err__reset_counter_and_flag(ERR__ID__AMOUNT) == false);              // 4
      assert(err__get_info(&errInfo, ERR__ID__AMOUNT) == false);                  // 5
      assert(err__raise_error(ERR__ID__AMOUNT) == false);                         // 6
      assert(errInfo.type != ERR__TYPE__AMOUNT);
      assert(errInfo.type != ERR__TYPE__ANY_TYPE);
      assert(errInfo.description_ptr != NULL);
    printf("  PASS: wrong id.\n");

    // смотрим зарегестрировалась ли ошибка
      assert(err__get_info(&errInfo, ERR__ID__ERR__WRONG_ID) == true);
      assert(err__has_unhandled_errors(ERR__TYPE__ANY_TYPE) == true);
      assert(errInfo.id == ERR__ID__ERR__WRONG_ID);
      assert(errInfo.type != ERR__TYPE__AMOUNT);
      assert(errInfo.type != ERR__TYPE__ANY_TYPE);
      assert(errInfo.description_ptr != NULL);
      assert(errInfo.unhandled == true);
      assert(errInfo.counter_unhandled == 6);                                   // 6
      printf("  PASS: register wrong id.\n");

  // Тест неправильного типа ошибки
    //сбрасиваем счётчик
      assert(err__reset_counter_and_flag(ERR__ID__ERR__WRONG_TYPE) == true);

    // тест функции проверки
      assert(err__is_type_correct(-1) == false);                                 // 1
      assert(err__is_type_correct(ERR__TYPE__AMOUNT) == false);                   // 2

    // Отправляем неправильный тип ошибки
      assert(err__has_unhandled_errors(-1) == false);                            // 3
      assert(err__has_unhandled_errors(ERR__TYPE__AMOUNT) == false);              // 4
      printf("  PASS: wrong types.\n");
    
    // Проверяем
      assert(err__get_info(&errInfo, ERR__ID__ERR__WRONG_TYPE) == true);
      assert(errInfo.id == ERR__ID__ERR__WRONG_TYPE);
      assert(errInfo.description_ptr != NULL);
      assert(errInfo.unhandled == true);
      assert(errInfo.counter_unhandled == 4);                                   // +2
      printf("  PASS: register wrong types.\n");

  // Тест фильтрации
    // сброс
      for(ErrId id = 0; id < ERR__ID__AMOUNT; id++)
      {
        assert(err__reset_counter_and_flag(id) == true);
      }

    // тест фильтров
      for(ErrId id = 0; id < ERR__ID__AMOUNT; id++)
      {
        // узнаём тип ошибки с id
          assert(err__get_info(&errInfo, id) == true);
        ErrType type =  errInfo.type;
        // поднимаем ошибку
          assert(err__raise_error(id) == true);
        // проверяем фильтр
          assert(err__has_unhandled_errors(type) == true);
        // проверяем, что другие фильтры не реагируют
          for(ErrType type_2 = 0; type_2 < ERR__TYPE__AMOUNT; type_2++)
          {
            if(type != type_2 && type_2 != ERR__TYPE__ANY_TYPE)
            {
              assert(err__has_unhandled_errors(type_2) == false);
            }
          }
        // cбрасываем
          assert(err__reset_counter_and_flag(id) == true);
      }
      printf("  PASS: filter by type.\n");

    // Тест переполнения
      for(ErrId id = 0; id < ERR__USHORT_MAX + 1000; id++)
      {
        err__raise_error(ERR__ID__UNDEFINED);
      }
      assert(err__get_info(&errInfo, ERR__ID__UNDEFINED) == true);
      assert(errInfo.counter_unhandled == ERR__USHORT_MAX);
      assert(errInfo.counter_total     == ERR__USHORT_MAX);
      printf("  PASS: counter overfill.\n");

    // Тест NULL
      err__reset_all();   // сброс
      assert(err__get_info(NULL, ERR__ID__UNDEFINED) == false);
      assert(err__get_info(&errInfo, ERR__ID__ERR__RECEIVED_NULL) == true);
      assert(errInfo.counter_unhandled == 1);                                   // 1
      printf("  PASS: < < NULL.\n");

    // тест 



    // Полный сброс, тест функции для тестирования (должен быть определен макрос ENABLE_UNIT_TESTING_API в tasks.json)
      assert(err__get_total_counter(ERR__ID__ERR__RECEIVED_NULL) == 1);            // 1
      assert(err__get_total_counter(ERR__ID__AMOUNT) == 0);                       // неправильный айди отправили - вернуло 0
      assert(err__get_total_counter(ERR__ID__ERR__WRONG_ID) == 1);                 // неправильный айди + 1

      err__reset_all();
      for(ErrId id = 0; id < ERR__ID__AMOUNT; id++)
      {
        assert(err__get_info(&errInfo, id) == true);
        assert(errInfo.counter_unhandled == 0);
        assert(errInfo.counter_total == 0);
        assert(errInfo.unhandled == false);
      }
      printf("  PASS: testing feature - err__reset_all().\n");

  #undef VALID_ID
  #undef INVALID_ID

  return 0;
}