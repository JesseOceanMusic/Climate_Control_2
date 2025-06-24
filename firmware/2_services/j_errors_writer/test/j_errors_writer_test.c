#include "j_errors_writer_test.h"

ErrBufferInfo errBufferInfoResponce;

bool is_this_structs_same(ErrBufferInfo str1, ErrBufferInfo str2)
{
  if(str1.arr_ptr     != str2.arr_ptr ||
     str1.state       != str2.state ||
     str1.type_filter != str2.type_filter)
  {
    return false;
  }
  return true;
}

void j_errors_writer_test()
{
  char test_arr[BUFFER_FOR_ERRORS_SIZE * 2];  // больше чем буфер

  printf("j_errors_writer_test.c: ЗАПУСК ТЕСТИРОВАНИЯ\n");  

  // проверка размера массива для ошибок
    unsigned int max_msg_size = 1 + snprintf(NULL, 0, "%s", ERRORS_HEADER_MESSAGE); 
    
    for(int id = 0; id < J_ERRORS_AMOUNT; id++)
    {
      unsigned int msg_size = 1 + snprintf(NULL, 0, "%d/%d/%s\n",                             
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
    printf("  PASS: BUFFER_FOR_ERRORS_SIZE(%d) - достаточный размер(%d).\n",
                    BUFFER_FOR_ERRORS_SIZE, max_msg_size);

  // Провека info return
    errBufferInfoResponce = err_writer__set_task__all_errors(ERR_TYPE__ERROR);
    is_this_structs_same(errBufferInfoResponce, err_writer__get_status());
    assert(errBufferInfoResponce.arr_ptr     != NULL);
    assert(errBufferInfoResponce.state       == ERR_WRITER__CUR_TASK__WRITE_ALL_ERRORS);
    assert(errBufferInfoResponce.type_filter == ERR_TYPE__ERROR);
    printf("  PASS: return Info 1\n");
    
    errBufferInfoResponce = err_writer__set_task__unhandled_errors(ERR_TYPE__FATAL);
    is_this_structs_same(errBufferInfoResponce, err_writer__get_status());
    assert(errBufferInfoResponce.arr_ptr     != NULL);
    assert(errBufferInfoResponce.state       == ERR_WRITER__CUR_TASK__WRITE_UNHANDLED_ERRORS);
    assert(errBufferInfoResponce.type_filter == ERR_TYPE__FATAL);
    printf("  PASS: return Info 2\n");

    errBufferInfoResponce = err_writer__write_next_part();
    is_this_structs_same(errBufferInfoResponce, err_writer__get_status());
    assert(errBufferInfoResponce.arr_ptr     != NULL);
    assert(errBufferInfoResponce.state       == ERR_WRITER__CUR_TASK__COMPLETE);
    printf("  PASS: return Info 3\n");

  // Проверка записи
    errBufferInfoResponce = err_writer__set_task__all_errors(ERR_TYPE__ERROR);
    //errBufferInfoResponce = err_writer__write_next_part();
    //assert(strcmp(errBufferInfoResponce.arr_ptr, "") == 0);

    errBufferInfoResponce = err_writer__write_next_part();
    assert(strcmp(errBufferInfoResponce.arr_ptr, ERRORS_HEADER_MESSAGE) == 0);

    for(int id = 0; id < J_ERRORS_AMOUNT; id++)
    {
      errBufferInfoResponce = err_writer__write_next_part();
      if(get_error_type(id) == ERR_TYPE__ERROR)
      {
        snprintf(test_arr, sizeof(test_arr), "%d/%d/%s\n", id, get_error_counter(id), get_error_description_ptr(id));
        printf("  %d   %s", (int)errBufferInfoResponce.state, test_arr);
        printf(errBufferInfoResponce.arr_ptr);
        assert(strcmp(errBufferInfoResponce.arr_ptr, test_arr) == 0);
      }
      else
      {
        assert(strcmp(errBufferInfoResponce.arr_ptr, "") == 0);
      }
    }

  printf("j_errors_writer_test.c: ВСЕ ТЕСТЫ УСПЕШНО ПРОЙДЕНЫ!\n\n");

  // Сброс ошибок
    for(int id = 0; id < J_ERRORS_AMOUNT; id++)
    {
      reset_error_counter(id);
      reset_unhandled_error_flag(id);
    }
}