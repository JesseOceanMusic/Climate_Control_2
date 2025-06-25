#include "j_errors_writer.h"

  static char char_buffer_for_errors_arr[BUFFER_FOR_ERRORS_SIZE];               // создаём массив (буфер)       
  static struct CharBuffer charBufferForErrors =                                // инициализируем структуру для массива (буфера)
  {
    .arr_ptr  = char_buffer_for_errors_arr,                                     // указатель на массив (буфер)
    .arr_size = sizeof(char_buffer_for_errors_arr),                             // размер массива (буфера)
    .offset   = 0,                                                              // offset(смещение) равно нулю
  };

  static ErrBufferInfo errWriterErrBufferInfo =
  {
    .arr_ptr     = char_buffer_for_errors_arr,
    .state       = ERR_WRITER__CUR_TASK__NONE,
    .type_filter = ERR_TYPE__ALERT,
  };

  static short write_errors_id_itterator = -1;   // -1 = заголовок

  static void write_error_to_buffer (int id)
  {
    buffer_write_int(&charBufferForErrors, id);                                 // ID
    buffer_write_char(&charBufferForErrors, "/");
    buffer_write_int(&charBufferForErrors, get_error_counter(id));              // counter
    buffer_write_char(&charBufferForErrors, "/");
    buffer_write_char(&charBufferForErrors, get_err_description_ptr_arr(id));     // description
    buffer_write_char(&charBufferForErrors, "\n");
  }

  static bool is_buffer_has_enough_space(int id)
  {
    unsigned int needed_size = 1 + snprintf(NULL, 0, "%d/%d/%s\n", 
                                  id, 
                                  get_error_counter(id), 
                                  get_err_description_ptr_arr(id));
                                  
    if (buffer_size_left(&charBufferForErrors) < needed_size)
    {
      return false; // МЕСТО КОНЧИЛОСЬ
    }
    return true;
  }

  static bool is_there_any_error_with_cur_type()
  {
    for(int id = 0; id < ERR_ID__AMOUNT; id++)
    {
      if(get_error_type(id) == errWriterErrBufferInfo.type_filter)
      {
        return true;
      }
    }
    raise_error(ERR_ID__NO_ERR_WITH_THIS_TYPE);
    return false;
  }

  static void fill_err_buffer()
  {
    // if UNHANDLED ERRORS - Проверяем есть ли ошибки для записи нужного типа
      if(errWriterErrBufferInfo.state == ERR_WRITER__CUR_TASK__WRITE_err_unhandled_errors)
      {
        if(has_err_unhandled_errors(errWriterErrBufferInfo.type_filter) == false)
        {
          errWriterErrBufferInfo.state = ERR_WRITER__CUR_TASK__COMPLETE;
          return; // Ошибок с таким фильром не было
        }
      }

    // ALL ERRORS - Проверяем есть ли вообще ошибки с таким типом
      if(errWriterErrBufferInfo.state == ERR_WRITER__CUR_TASK__WRITE_ALL_ERRORS)
      {
        if(is_there_any_error_with_cur_type() == false)
        {
          errWriterErrBufferInfo.state = ERR_WRITER__CUR_TASK__COMPLETE;
          return; // Ошибок с таким фильром не было              
        }
      }      
    // Добавляем заголовок.
      if(write_errors_id_itterator == -1) 
      {
        if (buffer_write_char(&charBufferForErrors, ERRORS_HEADER_MESSAGE) == false)
        {
          errWriterErrBufferInfo.state = ERR_WRITER__CUR_TASK__ERROR;
        }
        write_errors_id_itterator = 0;
        return; // записали заголовок
      }

    // Проверяем есть ли место для записи одной ошибки.
      if(write_errors_id_itterator == 0)
      {
        if (is_buffer_has_enough_space(write_errors_id_itterator) == false)
        {
          errWriterErrBufferInfo.state = ERR_WRITER__CUR_TASK__ERROR;
          return;    // МЕСТО КОНЧИЛОСЬ, НЕЛЬЗЯ ЗАПИСАТЬ ДАЖЕ ОДНУ ОШИБКУ - ВСЁ ПЛОХО!
        }
      }
    
    // Фильтр ошибок
      if(get_error_type(write_errors_id_itterator) == errWriterErrBufferInfo.type_filter)
      {
        // ALL ERRORS
        if (errWriterErrBufferInfo.state == ERR_WRITER__CUR_TASK__WRITE_ALL_ERRORS)
        {
          write_error_to_buffer(write_errors_id_itterator);
        }        
      
        // UNHANDLED ERRORS
        if (errWriterErrBufferInfo.state == ERR_WRITER__CUR_TASK__WRITE_err_unhandled_errors)
        {
          if(get_unhandled_error_flag(write_errors_id_itterator) == true)
          {
            write_error_to_buffer(write_errors_id_itterator);
            reset_unhandled_error_flag(write_errors_id_itterator);          // считаем, что обработали ошибку
          }
        }
      }

    // Все ошибки были записаны
      if(write_errors_id_itterator == ERR_ID__AMOUNT - 1)
      {
        errWriterErrBufferInfo.state = ERR_WRITER__CUR_TASK__COMPLETE;
      }
    write_errors_id_itterator++;
  }

  ErrBufferInfo err_writer__write_next_part()
  {    
    if(errWriterErrBufferInfo.state == ERR_WRITER__CUR_TASK__WRITE_ALL_ERRORS ||
       errWriterErrBufferInfo.state == ERR_WRITER__CUR_TASK__WRITE_err_unhandled_errors)
    {
        // Отчищаем буфер
          buffer_clear(&charBufferForErrors);
        // Запускаем итерацию FSM
          fill_err_buffer();
    }
    return errWriterErrBufferInfo;
  }

  ErrBufferInfo err_writer__set_task__all_errors(ErrType handle_this_type)
  {
    errWriterErrBufferInfo.type_filter = handle_this_type;
    errWriterErrBufferInfo.state = ERR_WRITER__CUR_TASK__WRITE_ALL_ERRORS;
    write_errors_id_itterator = -1;                                              // Сбрасываем итератор
    return errWriterErrBufferInfo;
  }

  ErrBufferInfo err_writer__set_task__err_unhandled_errors(ErrType handle_this_type)
  {
    errWriterErrBufferInfo.type_filter = handle_this_type;
    errWriterErrBufferInfo.state = ERR_WRITER__CUR_TASK__WRITE_err_unhandled_errors;
    write_errors_id_itterator = -1;                                              // Сбрасываем итератор
    return errWriterErrBufferInfo;
  }

  ErrBufferInfo err_writer__get_status()
  {
    return errWriterErrBufferInfo;
  }