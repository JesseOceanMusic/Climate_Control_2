#include "jesse_buffer_api.h"

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
  // buffer_write_char_and_error_check()                                        // запись char/string по указателю
  // buffer_write_int_and_error_check()                                         // запись int по указателю
  // buffer_write_float_and_error_check()                                       // запись float по указателю
  // buffer_clear()                                                             // очистить буфер
  
  void write_test_with_interface(struct CharBuffer *struct_ptr)
  {
    float temp = 24.8;
    int co2 = 999;

    buffer_write_char_and_error_check(struct_ptr, "CO2 is: ");
    buffer_write_int_and_error_check(struct_ptr, co2);
    buffer_write_char_and_error_check(struct_ptr, "\nTemperature is: ");
    buffer_write_float_and_error_check(struct_ptr, temp);
  }

  #include <windows.h>                                                          // Для Sleep() // !!!ТОЛЬКО ДЛЯ ПРИМЕРА!!!

  int main()
  {
    for(int i = 0; i < 20; i++)
    {
      write_test_with_interface(&charBufferMedium);
      printf("CharBuffer.offset is: %d\n",     charBufferMedium.offset);
      printf("char_buffer_medium_arr is:\n%s\n\n", charBufferMedium.arr_ptr);
      buffer_clear(&charBufferMedium);                                          // после использования данных - "отчищаем буфер".
      Sleep(3000);
    }
    return 0;
  }