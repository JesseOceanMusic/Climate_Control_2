/// === ИНТЕРФЕЙС === ///
  #include "j_errors_manager.h"
  
  static const char ERROR_HEADER_MESSAGE[] = "ERRORS:\nID/counter/description:\n";

  static void check_write_result_for_errors(BufferStatus result)                // да, это дублирование функции из интерфейса буффера
  {                                                                             // исключает взаимодействие модулей одного уровня иерархии
    if (result == BUFFER_ERROR_NULL)
    {
      raise_error(ERROR_ID_BUFFER_NULL);
    }

    if (result == BUFFER_ERROR_OVERFILL)
    {
      raise_error(ERROR_ID_BUFFER_OVERFILLED);
    }    
  }

  static void write_error_to_buffer (struct CharBuffer *struct_ptr, int id)
  {
    BufferStatus bufferResult;
    bufferResult = buffer_write_int(struct_ptr, id);                            // ID
    check_write_result_for_errors(bufferResult);

    bufferResult = buffer_write_char(struct_ptr, "/");
    check_write_result_for_errors(bufferResult);

    bufferResult = buffer_write_int(struct_ptr, error_counters_arr[id]);        // counter
    check_write_result_for_errors(bufferResult);
    
    bufferResult = buffer_write_char(struct_ptr, "/");
    check_write_result_for_errors(bufferResult);
    
    bufferResult = buffer_write_char(struct_ptr, error_description_ptr[id]);    // description
    check_write_result_for_errors(bufferResult);
    
    bufferResult = buffer_write_char(struct_ptr, "\n");
    check_write_result_for_errors(bufferResult);
  }

  static void write_error_header_message_to_buffer (struct CharBuffer *struct_ptr)
  {
    BufferStatus bufferResult;
    bufferResult = buffer_write_char(struct_ptr, ERROR_HEADER_MESSAGE);         // добавляем заголовок
    check_write_result_for_errors(bufferResult);
  }

  void print_all_errors_to_buffer (struct CharBuffer *struct_ptr)               // запись ВСЕХ ошибок за время работы программы в буфер  
  {
    write_error_header_message_to_buffer(struct_ptr);                           // добавляем заголовок

    for (int id = 0; id < J_ERRORS_AMOUNT; id++)
    {
      int size_left = struct_ptr->arr_size - struct_ptr->offset;

      if (error_counters_arr[id] > 0 && error_description_ptr[id] != NULL)
      {
        write_error_to_buffer(struct_ptr, id);
      }
    }
  }

  void print_unhandled_errors_to_buffer (struct CharBuffer *struct_ptr)
  {
    write_error_header_message_to_buffer(struct_ptr);                           // добавляем заголовок
    
    for (int id = 0; id < J_ERRORS_AMOUNT; id++)
    {
      if (unhandled_errors[id] == true && error_description_ptr[id] != NULL)    // если ошибка необработана
      {
        int size_left = struct_ptr->arr_size - struct_ptr->offset;
        int needed_size = sizeof(error_description_ptr[id]) + 16;               // описание + 16 символов на /0, код ошибки, /, количество. с запасом.
        if (size_left < needed_size)
        {
          return; // МЕСТО КОНЧИЛОСЬ
        }
        write_error_to_buffer(struct_ptr, id);
        unhandled_errors[id] = false;                                           // считаем, что обработали ошибку
      }
    }
  }

  bool tick_errors_writer (struct CharBuffer *struct_ptr)                       // тикер, должен постоянно вызываться в loop
  {
    if(struct_ptr->offset == 0)                                                 // 1. БУФЕР ПУСТОЙ? (если не пустой, значит тот, кто их выводит халтурит или не успел)
    {
      if (has_unhandled_errors() == true)                                       // 2. ЕСТЬ ОШИБКИ -> ЗАПИШЕМ В БУФЕР, СКОЛЬКО ВЛЕЗЕТ
      {
        write_error_header_message_to_buffer(struct_ptr);                       // добавляем заголовок

      }
    }

    BufferStatus bufferResult;
    if (has_unhandled_errors() == true)
    {
      bufferResult = buffer_write_char(struct_ptr, ERROR_HEADER_MESSAGE);       // добавляем заголовок
      check_write_result_for_errors(bufferResult);

      for (int id = 0; id < J_ERRORS_AMOUNT; id++)
      {
        if (unhandled_errors[id] == true && error_description_ptr[id] != NULL)
        {
          write_error_to_buffer(struct_ptr, id);
          unhandled_errors[id] = false;                                         // считаем, что обработали ошибку
        }
      }
      return true;
    }
    return false;
  }