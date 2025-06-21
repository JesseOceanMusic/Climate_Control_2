/// === ИНТЕРФЕЙС === ///
  #include "jesse_errors_api.h"
  
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

  void print_errors_log_to_buffer (struct CharBuffer *struct_ptr)               // запись ВСЕХ ошибок за время работы программы в буфер  
  {
    BufferStatus bufferResult;
    bufferResult = buffer_clear(struct_ptr);                                    // освобождаем буфер принудительно
    check_write_result_for_errors(bufferResult);

    bufferResult = buffer_write_char(struct_ptr, ERROR_HEADER_MESSAGE);         // добавляем заголовок
    check_write_result_for_errors(bufferResult);

    for (int id = 0; id < ERRORS_AMOUNT; id++)
    {
      if (error_counters_arr[id] > 0 && error_description_ptr[id] != NULL)
      {
        write_error_to_buffer(struct_ptr, id);
      }
    }
  }

  bool tick_ERRORS_handler (struct CharBuffer *struct_ptr)                      // тикер, должен постоянно вызываться в loop
                                                                                // eсли были ошибки В ЦИКЛЕ - запишет в буфер
  {
    BufferStatus bufferResult;
    if (has_unhandled_errors() == true)
    {
      bufferResult = buffer_clear(struct_ptr);                                  // освобождаем буфер принудительно
      check_write_result_for_errors(bufferResult);
      bufferResult = buffer_write_char(struct_ptr, ERROR_HEADER_MESSAGE);       // добавляем заголовок
      check_write_result_for_errors(bufferResult);

      for (int id = 0; id < ERRORS_AMOUNT; id++)
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