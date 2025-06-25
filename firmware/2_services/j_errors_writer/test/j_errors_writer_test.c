#include "j_errors_writer_test.h"

ErrBufferInfo errBufferInfoResponce;

static bool is_this_structs_same(ErrBufferInfo str1, ErrBufferInfo str2)
{
  if(str1.arr_ptr     != str2.arr_ptr ||
     str1.state       != str2.state ||
     str1.type_filter != str2.type_filter)
  {
    return false;
  }
  return true;
}

static bool is_there_any_error_with_cur_type(ErrType type)
{
  for(int id = 0; id < ERR_ID__AMOUNT; id++)
  {
    if(get_error_type(id) == type)
    {
      return true;
    }
  }
  return false;
}

void j_errors_writer_test()
{
  char test_arr[BUFFER_FOR_ERRORS_SIZE * 2];  // больше чем буфер

  printf("j_errors_writer_test.c: ЗАПУСК ТЕСТИРОВАНИЯ\n");  

  // проверка размера массива для ошибок
    unsigned int max_msg_size = 1 + snprintf(NULL, 0, "%s", ERRORS_HEADER_MESSAGE); 
    
    for(int id = 0; id < ERR_ID__AMOUNT; id++)
    {
      unsigned int msg_size = 1 + snprintf(NULL, 0, "%d/%d/%s\n",                             
                                          id,
                                          get_error_counter(id), 
                                          get_err_description_ptr_arr(id));
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
    for(ErrType type = 0; type < ERR_TYPE__AMOUNT; type++)
    {
      errBufferInfoResponce = err_writer__set_task__all_errors(type);
      is_this_structs_same(errBufferInfoResponce, err_writer__get_status());
      assert(strcmp(errBufferInfoResponce.arr_ptr, "") == 0); 
      assert(errBufferInfoResponce.state       == ERR_WRITER__CUR_TASK__WRITE_ALL_ERRORS);
      assert(errBufferInfoResponce.type_filter == type);
      
      errBufferInfoResponce = err_writer__set_task__err_unhandled_errors(type);
      is_this_structs_same(errBufferInfoResponce, err_writer__get_status());
      assert(strcmp(errBufferInfoResponce.arr_ptr, "") == 0); 
      assert(errBufferInfoResponce.state       == ERR_WRITER__CUR_TASK__WRITE_err_unhandled_errors);
      assert(errBufferInfoResponce.type_filter == type);

      printf("  PASS: return, type %d\n", type);      
    }

  // Проверка записи ALL ERR
    for(ErrType type = 0; type < ERR_TYPE__AMOUNT; type++)
    {
      errBufferInfoResponce = err_writer__set_task__all_errors(type);
      errBufferInfoResponce = err_writer__write_next_part();
      if(is_there_any_error_with_cur_type(type) == true)
      {
        assert(strcmp(errBufferInfoResponce.arr_ptr, ERRORS_HEADER_MESSAGE) == 0);        
        for(int id = 0; id < ERR_ID__AMOUNT; id++)
        {
          errBufferInfoResponce = err_writer__write_next_part();
          if(get_error_type(id) == type)
          {
            snprintf(test_arr, sizeof(test_arr), "%d/%d/%s\n", id, get_error_counter(id), get_err_description_ptr_arr(id));
            assert(strcmp(errBufferInfoResponce.arr_ptr, test_arr) == 0);
          }
          else
          {
            assert(strcmp(errBufferInfoResponce.arr_ptr, "") == 0);
          }
        }   
      }

      else
      {
        assert(strcmp(errBufferInfoResponce.arr_ptr, "") == 0);
        assert(get_error_counter(ERR_ID__NO_ERR_WITH_THIS_TYPE) > 0);
        assert(errBufferInfoResponce.state == ERR_WRITER__CUR_TASK__COMPLETE);
      }

      printf("  PASS: write all err: type %d\n", type);
    }

  printf("j_errors_writer_test.c: ВСЕ ТЕСТЫ УСПЕШНО ПРОЙДЕНЫ!\n\n");

  // Сброс ошибок
    for(int id = 0; id < ERR_ID__AMOUNT; id++)
    {
      reset_error_counter(id);
      reset_unhandled_error_flag(id);
    }
}