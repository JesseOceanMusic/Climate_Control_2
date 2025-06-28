#include "c_errors_writer_test.h"

struct ErrWriterInfo errWriterInfo;

static bool are_those_structs_same(struct ErrWriterInfo str1, struct ErrWriterInfo str2)
{
  if(str1.arr_ptr     == str2.arr_ptr ||
     str1.msg_length  == str2.msg_length ||
     str1.state       == str2.state ||
     str1.type_filter == str2.type_filter)
  {
    return true;
  }
  return false;
}

static bool is_there_any_error_with_cur_type(ErrType type)
{
  struct ErrInfo errInfo;
  for(ErrId id = 0; id < ERR_ID__AMOUNT; id++)
  {
    errInfo = err_get_info(id);
    if(errInfo.type == type)
    {
      return true;
    }
  }
  return false;
}

void c_errors_writer_test()
{
  char test_arr[BUFFER_FOR_ERRORS_SIZE * 2];  // больше чем буфер
  struct ErrInfo errInfo;

  printf("c_errors_writer_test.c: ЗАПУСК ТЕСТИРОВАНИЯ\n");  

  // проверка размера массива для ошибок
    unsigned int max_msg_size = 1 + snprintf(NULL, 0, "%s", ERR_HEADER_MESSAGE); 
    
    for(ErrId id = 0; id < ERR_ID__AMOUNT; id++)
    {
      errInfo = err_get_info(id); 
      unsigned int msg_size = 1 + snprintf(NULL, 0, "%d/%d/%s\n", 
                                (int)id, 
                                (int)errInfo.counter_cur,                       // cur всегда будет больше, чем разница, так что можем считать для "худшего" случая 
                                errInfo.description_ptr);
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
      errWriterInfo = err_writer__set_task__all_errors(type);
      are_those_structs_same(errWriterInfo, err_writer__get_info());
      assert(strcmp(errWriterInfo.arr_ptr, "") == 0); 
      assert(errWriterInfo.state       == ERR_WRITER__CUR_TASK__WRITE_ALL_ERRORS);
      assert(errWriterInfo.type_filter == type);
      
      errWriterInfo = err_writer__set_task__err_unhandled_errors(type);
      are_those_structs_same(errWriterInfo, err_writer__get_info());
      assert(strcmp(errWriterInfo.arr_ptr, "") == 0); 
      assert(errWriterInfo.state       == ERR_WRITER__CUR_TASK__WRITE_UNHANDLED_ERRORS);
      assert(errWriterInfo.type_filter == type);

      printf("  PASS: return, type %d\n", type);      
    }

  // Проверка записи ALL ERR
    for(ErrType type = 0; type < ERR_TYPE__AMOUNT; type++)
    {
      errWriterInfo = err_writer__set_task__all_errors(type);
      errWriterInfo = err_writer__write_next_part();
      if(is_there_any_error_with_cur_type(type) == true)
      {
        assert(strcmp(errWriterInfo.arr_ptr, ERR_HEADER_MESSAGE) == 0);        
        for(int id = 0; id < ERR_ID__AMOUNT; id++)
        {
          errWriterInfo = err_writer__write_next_part();
          if(get_error_type(id) == type)
          {
            snprintf(test_arr, sizeof(test_arr), "%d/%d/%s\n", id, get_error_counter(id), get_err_description_ptr_arr(id));
            assert(strcmp(errWriterInfo.arr_ptr, test_arr) == 0);
          }
          else
          {
            assert(strcmp(errWriterInfo.arr_ptr, "") == 0);
          }
        }   
      }

      else
      {
        assert(strcmp(errWriterInfo.arr_ptr, "") == 0);
        assert(get_error_counter(ERR_ID__ERR_WR_UNLISTED_ERROR_TYPE) > 0);
        assert(errWriterInfo.state == ERR_WRITER__CUR_TASK__COMPLETE);
      }

      printf("  PASS: write all err: type %d\n", type);
    }

  printf("c_errors_writer_test.c: ВСЕ ТЕСТЫ УСПЕШНО ПРОЙДЕНЫ!\n\n");

  // Сброс ошибок
    for(int id = 0; id < ERR_ID__AMOUNT; id++)
    {
      reset_error_counter(id);
      reset_unhandled_error_flag(id);
    }
}