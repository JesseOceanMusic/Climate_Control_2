#include <stdio.h>
#include "c_buffer.h"

/// === ИНИЦИАЛИЗАЦИЯ БУФЕРА c_buffer === ///                               // !!!ТОЛЬКО ДЛЯ ПРИМЕРА!!! РАБОТЫ ИНТЕРФЕЙСА
                                                                                // буфер создаётся на самом высоком уровне иерархии
  static char char_buffer_medium_arr[512];                                      // создаём массив (буфер)       
  struct BufInfo bufInfo =                                          // инициализируем структуру для массива (буфера)
  {
    .arr_ptr      = char_buffer_medium_arr,                                     // указатель на массив (буфер)
    .arr_size     = sizeof(char_buffer_medium_arr),                             // размер массива (буфера)
    .offset       = 0,                                                          // offset(смещение) равно нулю
  };

/// === ПРИМЕР ИСПОЛЬЗОВАНИЯ c_buffer === ///
  void write_test(struct BufInfo *struct_ptr)
  {
    float temp = 24.8;
    int co2 = 999;

    buf_write_char(struct_ptr, "CO2 is: ");
    buf_write_int(struct_ptr, co2);
    buf_write_char(struct_ptr, "\nTemperature is: ");
    buf_write_float(struct_ptr, temp, 2);
  }

  int main()
  {
    printf("\nзапись в буфер...\n");
    write_test(&bufInfo);

    printf("\nчтение параметров...\n");
    printf("bufInfo.offset: %d\n", bufInfo.offset);
    printf("buf_is_it_empty: %d\n",         buf_is_it_empty(&bufInfo));
    printf("buf_size_left: %d\n",        buf_size_left(&bufInfo));

    printf("\nчтение буфера...\n");
    printf("char_buffer_medium_arr:\n%s\n", bufInfo.arr_ptr);
    
    printf("\nотчистка буфера...\n");
    buf_clear(&bufInfo);

    return 0;
  }