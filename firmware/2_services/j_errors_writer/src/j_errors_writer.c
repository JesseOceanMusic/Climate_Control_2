#include "j_errors_writer.h"

/// === ИНИЦИАЛИЗАЦИЯ БУФЕРА === ///
                                                                                // буфер создаётся на самом высоком уровне иерархии
  static char char_buffer_for_errors_arr[81];                                   // создаём массив (буфер)       
  struct CharBuffer charBufferForErrors =                                       // инициализируем структуру для массива (буфера)
  {
    .arr_ptr      = char_buffer_for_errors_arr,                                 // указатель на массив (буфер)
    .arr_size     = sizeof(char_buffer_for_errors_arr),                         // размер массива (буфера)
    .offset       = 0,                                                          // offset(смещение) равно нулю
  };

/// ===

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

  void write_unhandled_errors_to_buffer(struct ErrorsBufferInfo *errorsBufferInfo)
  {
    // 1. Добавляем указатель в структуру
      errorsBufferInfo->arr_ptr = char_buffer_for_errors_arr;
      errorsBufferInfo->is_errors_writer_written_anything = false;

    // 2. Отчищаем буфер
      buffer_clear(&charBufferForErrors);

    // 2. Проверяем есть ли ошибки для записи
      if(has_unhandled_errors() == false)
      {   
        return;  // Нет ошибок
      }

    // 4-a Записываем в буфер необработанные ошибки
      
      for (unsigned short id = 0; id < J_ERRORS_AMOUNT; id++)
      {
        // Добавляем заголовок, если это "первое сообщение".
          if(id == 0)
          {
            buffer_write_char(&charBufferForErrors, ERRORS_HEADER_MESSAGE);
          }

        if (unhandled_errors[id] == true)
        {
          if (is_buffer_has_enough_space(id) == false)
          {
            return; // МЕСТО КОНЧИЛОСЬ
          }
          write_error_to_buffer(id);
          errorsBufferInfo->is_errors_writer_written_anything = true;
          unhandled_errors[id] = false;                                           // считаем, что обработали ошибку
        }
     }
  }

  static unsigned short write_all_errors_itterator_id = 0;

  void write_all_errors_to_buffer(struct ErrorsBufferInfo *errorsBufferInfo)
  {
    // 1. Добавляем указатель в структуру
      errorsBufferInfo->arr_ptr = char_buffer_for_errors_arr;
      errorsBufferInfo->is_errors_writer_written_anything = false;

    // 2. Отчищаем буфер
      buffer_clear(&charBufferForErrors);

    // 2. если в прошлый раз отправили все ошибки сбрасываем стейт и возвращаем false
      if (write_all_errors_itterator_id == J_ERRORS_AMOUNT)
      {
        write_all_errors_itterator_id = 0;
        return;
      }

    // 4 Записываем в буфер все ошибки
      for (int id = write_all_errors_itterator_id ; id < J_ERRORS_AMOUNT; id++)
      {
        // Добавляем заголовок, если это "первое сообщение".
          if(id == 0)
          {
            buffer_write_char(&charBufferForErrors, ERRORS_HEADER_MESSAGE);
          }
        
        if (is_buffer_has_enough_space(id) == false)
        {
          return; // МЕСТО КОНЧИЛОСЬ
        }
        write_error_to_buffer(id);
        write_all_errors_itterator_id = id + 1;
        errorsBufferInfo->is_errors_writer_written_anything = true;
      }
  }