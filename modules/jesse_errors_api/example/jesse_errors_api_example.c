/// === ИНИЦИАЛИЗАЦИЯ === ///
  //добавить коды и описание ошибок в файл errors_list.def
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

/// === ИСПОЛЬЗОВАНИЕ === ///
  // tick_ERRORS_handler();                                                     // тикер, который весит в таск/луп и проверят были ли ошибки
  // raise_error(ERROR_ID_UNDEFINED);                                           // "поднять" ошибку
  // print_errors_log_to_buffer();                                              // запись отчёта обо всех ошибках в буфер
  
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
      
      printf("func - tick_ERRORS_handler:\n");
      tick_ERRORS_handler(&charBufferMedium);                                   // NON-BLOCKING TICKER (должен быть в loop/task)
      printf("char_buffer_medium_arr is:\n%s\n", charBufferMedium.arr_ptr);
      buffer_clear(&charBufferMedium);                                          // после использования данных - "отчищаем буфер".

      Sleep(3000);
      printf("func - print_errors_log_to_buffer:\n");
      print_errors_log_to_buffer(&charBufferMedium);
      printf("char_buffer_medium_arr is:\n%s\n", charBufferMedium.arr_ptr);
      buffer_clear(&charBufferMedium);                                          // после использования данных - "отчищаем буфер".
    }
    return 0;
  }