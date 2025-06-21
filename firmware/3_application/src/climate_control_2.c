#include "climate_control_2.h"

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
  // buffer_clear()    


int main()
{
  return 0;
}