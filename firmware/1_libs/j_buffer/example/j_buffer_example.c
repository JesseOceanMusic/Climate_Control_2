#include <stdio.h>
#include "j_buffer.h"

/// === ИНИЦИАЛИЗАЦИЯ БУФЕРА j_buffer === ///                               // !!!ТОЛЬКО ДЛЯ ПРИМЕРА!!! РАБОТЫ ИНТЕРФЕЙСА
                                                                                // буфер создаётся на самом высоком уровне иерархии
  static char char_buffer_medium_arr[512];                                      // создаём массив (буфер)       
  struct CharBuffer charBufferMedium =                                          // инициализируем структуру для массива (буфера)
  {
    .arr_ptr      = char_buffer_medium_arr,                                     // указатель на массив (буфер)
    .arr_size     = sizeof(char_buffer_medium_arr),                             // размер массива (буфера)
    .offset       = 0,                                                          // offset(смещение) равно нулю
  };

/// === ПРИМЕР ИСПОЛЬЗОВАНИЯ j_buffer === ///
  void write_test(struct CharBuffer *struct_ptr)
  {
    float temp = 24.8;
    int co2 = 999;

    buffer_write_char(struct_ptr, "CO2 is: ");
    buffer_write_int(struct_ptr, co2);
    buffer_write_char(struct_ptr, "\nTemperature is: ");
    buffer_write_float(struct_ptr, temp);
  }

  int main()
  {
    printf("\nзапись в буфер...\n");
    write_test(&charBufferMedium);

    printf("\nчтение параметров...\n");
    printf("charBufferMedium.offset: %d\n", charBufferMedium.offset);
    printf("is_buffer_empty: %d\n",         is_buffer_empty(&charBufferMedium));
    printf("buffer_size_left: %d\n",        buffer_size_left(&charBufferMedium));

    printf("\nчтение буфера...\n");
    printf("char_buffer_medium_arr:\n%s\n", charBufferMedium.arr_ptr);
    
    printf("\nотчистка буфера...\n");
    buffer_clear(&charBufferMedium);

    return 0;
  }