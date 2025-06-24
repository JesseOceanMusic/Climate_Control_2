#include "j_errors_writer_test.h"

struct ErrorsBufferInfo errorsBufferInfoResponce;

void j_errors_writer_test()
{
  printf("j_errors_writer_test.c: ЗАПУСК ТЕСТИРОВАНИЯ\n");  

  // проверка размера массива для ошибок
    unsigned int max_msg_size = 0;
    for(int id = 0; id < J_ERRORS_AMOUNT; id++)
    {
      unsigned int msg_size = 1 + snprintf(NULL, 0, "%s%d/%d/%s\n", 
                                          ERRORS_HEADER_MESSAGE,                             
                                          id,
                                          get_error_counter(id), 
                                          get_error_description_ptr(id));
      if(max_msg_size < msg_size)
      {
        max_msg_size = msg_size;
      }
    }
    // если прилетает assert -> размер массива/буффера "BUFFER_FOR_ERRORS_SIZE" слишком маленький.
    assert(BUFFER_FOR_ERRORS_SIZE > max_msg_size);
    printf("  PASS: BUFFER_FOR_ERRORS_SIZE - достаточный размер.\n");

  // проверка NULL
    errors_writer__set_task__all_errors(ERR_TYPE__ALL);
    int amount_NULL = get_error_counter(ERR_ID__NULL_IN_ERRORS_WRITER);
    errors_writer__write_next_part(NULL);
    assert(get_error_counter(ERR_ID__NULL_IN_ERRORS_WRITER) == amount_NULL + 1);
    printf("  PASS: NULL.\n");

  // проверка записи
    

  printf("j_errors_writer_test.c: ВСЕ ТЕСТЫ УСПЕШНО ПРОЙДЕНЫ!\n\n");
}