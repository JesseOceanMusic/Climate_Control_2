#include "j_errors_writer.h"

  static char char_buffer_for_errors_arr[BUFFER_FOR_ERRORS_SIZE];               // создаём массив (буфер)       
  struct CharBuffer charBufferForErrors =                                       // инициализируем структуру для массива (буфера)
  {
    .arr_ptr      = char_buffer_for_errors_arr,                                 // указатель на массив (буфер)
    .arr_size     = sizeof(char_buffer_for_errors_arr),                         // размер массива (буфера)
    .offset       = 0,                                                          // offset(смещение) равно нулю
  };

  static char error_writer_state = ERRORS_WRITER__CUR_TASK__NONE;
  static unsigned short write_errors_id_itterator = -1;
  static ErrorsTypes errors_type_filter;
  static short fsm_msg_generator_state = 0;

  static void write_error_to_buffer (int id)
  {
    buffer_write_int(&charBufferForErrors, id);                                 // ID
    buffer_write_char(&charBufferForErrors, "/");
    buffer_write_int(&charBufferForErrors, get_error_counter(id));              // counter
    buffer_write_char(&charBufferForErrors, "/");
    buffer_write_char(&charBufferForErrors, get_error_description_ptr(id));     // description
    buffer_write_char(&charBufferForErrors, "\n");
  }

  static bool is_buffer_has_enough_space(int id)
  {
    unsigned int needed_size = 1 + snprintf(NULL, 0, "%d/%d/%s\n", 
                                  id, 
                                  get_error_counter(id), 
                                  get_error_description_ptr(id));
                                  
    if (buffer_size_left(&charBufferForErrors) < needed_size)
    {
      return false; // МЕСТО КОНЧИЛОСЬ
    }
    return true;
  }

  static void write_errors_to_buffer_ini(struct ErrorsBufferInfo *errorsBufferInfo)
  {
    // Проверяем указатель
      if(errorsBufferInfo == NULL)
      {
        raise_error(ERR_ID__NULL_IN_ERRORS_WRITER);
        error_writer_state = ERRORS_WRITER__CUR_TASK__ERROR; // ОШИБКА
        return;
      }

    // Добавляем указатель в структуру
      errorsBufferInfo->arr_ptr = char_buffer_for_errors_arr;

    // Отчищаем буфер
      buffer_clear(&charBufferForErrors);
  }

  static void fsm_msg_generator(struct ErrorsBufferInfo *errorsBufferInfo)
  {
    switch(fsm_msg_generator_state)
    {
      case 0:
        // Проверяем указатель на NULL, отчищаем буфер, добавляем указатель в структуру
          write_errors_to_buffer_ini(errorsBufferInfo);
          fsm_msg_generator_state = 1;
          break;
      
      case 1:
        // Проверяем есть ли ошибки для записи
          if(error_writer_state == ERRORS_WRITER__CUR_TASK__WRITE_UNHANDLED_ERRORS)
          {
            if(has_unhandled_errors() == false)
            { 
              // Нет ошибок
              error_writer_state = ERRORS_WRITER__CUR_TASK__COMPLETE;
            }
          }
          fsm_msg_generator_state = 2;
          break;

      case 2:
        // Добавляем заголовок.
          if(write_errors_id_itterator == 0) 
          {
            if (buffer_write_char(&charBufferForErrors, ERRORS_HEADER_MESSAGE) == false)
            {
              error_writer_state = ERRORS_WRITER__CUR_TASK__ERROR;
            }
            write_errors_id_itterator = 0;
          }
          fsm_msg_generator_state = 3;
          break;

      case 3:
        // Проверяем есть ли место для записи одной ошибки.
          if(charBufferForErrors.offset == 0)
          {
            if (is_buffer_has_enough_space(write_errors_id_itterator) == false)
            {
              error_writer_state = ERRORS_WRITER__CUR_TASK__ERROR;
              break;    // МЕСТО КОНЧИЛОСЬ, НЕЛЬЗЯ ЗАПИСАТЬ ДАЖЕ ОДНУ ОШИБКУ - ВСЁ ПЛОХО!
            }
          }
        
        // Фильтр ошибок
          if(get_error_type(write_errors_id_itterator) == errors_type_filter)
          {
          // ALL ERRORS
            if (error_writer_state == ERRORS_WRITER__CUR_TASK__WRITE_ALL_ERRORS)
            {
              write_error_to_buffer(write_errors_id_itterator);
            }        
          
          // UNHANDLED ERRORS
            if (error_writer_state == ERRORS_WRITER__CUR_TASK__WRITE_UNHANDLED_ERRORS)
            {
              if(get_unhandled_error_flag(write_errors_id_itterator) == true)
              {
                write_error_to_buffer(write_errors_id_itterator);
                reset_unhandled_error_flag(write_errors_id_itterator);                // считаем, что обработали ошибку
              }
            }
          }
          write_errors_id_itterator++;

        // Все ошибки были записаны
          if(write_errors_id_itterator == J_ERRORS_AMOUNT - 1)
          {
            error_writer_state = ERRORS_WRITER__CUR_TASK__COMPLETE;
          }
          break;
    }
  }

  void errors_writer__write_next_part(struct ErrorsBufferInfo *errorsBufferInfo)
  {    
    if(error_writer_state == ERRORS_WRITER__CUR_TASK__WRITE_ALL_ERRORS ||
       error_writer_state == ERRORS_WRITER__CUR_TASK__WRITE_UNHANDLED_ERRORS)
    {
      fsm_msg_generator_state = 0;
      fsm_msg_generator(errorsBufferInfo);
    }
  }

  void errors_writer__set_task__all_errors(ErrorsTypes handle_this_type)
  {
    errors_type_filter = handle_this_type;
    error_writer_state = ERRORS_WRITER__CUR_TASK__WRITE_ALL_ERRORS;
    write_errors_id_itterator = 0;                                              // Сбрасываем итератор
  }

  void errors_writer__set_task__unhandled_errors(ErrorsTypes handle_this_type)
  {
    errors_type_filter = handle_this_type;
    error_writer_state = ERRORS_WRITER__CUR_TASK__WRITE_UNHANDLED_ERRORS;
    write_errors_id_itterator = 0;                                              // Сбрасываем итератор
  }