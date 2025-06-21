/// ПРИМЕР НЕ РАБОТАЕТ, ТАК КАК ПЕРЕИМЕНОВАН ФАЙЛ "errors_list.def" ///



/// === ИНИЦИАЛИЗАЦИЯ === ///
  #include "jesse_errors_api.h"

/// === ИНИЦИАЛИЗАЦИЯ БУФЕРА === ///                                            // !!!ТОЛЬКО ДЛЯ ПРИМЕРА!!! РАБОТЫ ИНТЕРФЕЙСА
                                                                                // буфер создаётся на самом высоком уровне иерархии
  static char char_buffer_medium_arr[512];                                      // создаём массив (буфер)       
  struct CharBuffer charBufferMedium =                                          // инициализируем структуру для массива (буфера)
  {
    .arr_ptr      = char_buffer_medium_arr,                                     // указатель на массив (буфер)
    .arr_size     = sizeof(char_buffer_medium_arr),                             // размер массива (буфера)
    .offset       = 0,                                                          // offset(смещение) равно нулю
  };

/// === ИСПОЛЬЗОВАНИЕ jesse_errors_api === ///
  // скопировать "errors_list_example.def" в include проекта, переименовать в "errors_list.def"
  // добавить коды и описание ошибок в "errors_list.def"

  // tick_errors_writer();                                                     // тикер, который весит в таск/луп и проверят были ли ошибки
  // raise_error(struct CharBuffer *struct_ptr);                                // "поднять" ошибку, принимает указатель на массив буфера
                                                                                // если будут ошибки - запишет в буфер и вернёт true
  // print_all_errors_to_buffer();                                              // запись отчёта обо всех ошибках в буфер
  
  #include <windows.h>                                                          // Для Sleep() // !!!ТОЛЬКО ДЛЯ ПРИМЕРА!!!

  int main()
  {
    while(1)
    {
      Sleep(3000);
      raise_error(ERROR_ID_UNDEFINED);
      raise_error(ERROR_ID_BUFFER_OVERFILLED);
      raise_error(ERROR_ID_BUFFER_OVERFILLED);
      raise_error(ERROR_ID_BUFFER_OVERFILLED);
      
      printf("func - tick_errors_writer:\n");
      tick_errors_writer(&charBufferMedium);                                   // NON-BLOCKING TICKER (должен быть в loop/task)
      printf("char_buffer_medium_arr is:\n%s\n", charBufferMedium.arr_ptr);
      buffer_clear(&charBufferMedium);                                          // после использования данных - "отчищаем буфер".

      Sleep(3000);
      printf("func - print_all_errors_to_buffer:\n");
      print_all_errors_to_buffer(&charBufferMedium);
      printf("char_buffer_medium_arr is:\n%s\n", charBufferMedium.arr_ptr);
      buffer_clear(&charBufferMedium);                                          // после использования данных - "отчищаем буфер".
    }
    return 0;
  }