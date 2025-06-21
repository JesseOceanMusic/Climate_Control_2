#include "jesse_buffer.h"

/// === ИНИЦИАЛИЗАЦИЯ БУФЕРА === ///                                            // !!!ТОЛЬКО ДЛЯ ПРИМЕРА!!! РАБОТЫ ИНТЕРФЕЙСА
                                                                                // буфер создаётся на самом высоком уровне иерархии
  static char char_buffer_medium_arr[512];                                      // создаём массив (буфер)       
  struct CharBuffer charBufferMedium =                                          // инициализируем структуру для массива (буфера)
  {
    .arr_ptr      = char_buffer_medium_arr,                                     // указатель на массив (буфер)
    .arr_size     = sizeof(char_buffer_medium_arr),                             // размер массива (буфера)
    .offset       = 0,                                                          // offset(смещение) равно нулю
  };

/// === ПРИМЕР ИСПОЛЬЗОВАНИЯ БЕЗ ИНТЕРФЕЙСА jesse_buffer_api === ///            // Неудобный вариант, работы напрямую с библиотекой jesse_buffer
  void write_test_without_interface(struct CharBuffer *struct_ptr)
  {
    float temp = 24.8;
    int co2 = 999;
    BufferStatus result;

    result = buffer_write_char(struct_ptr, "CO2 is: ");
    result = buffer_write_int(struct_ptr, co2);
    result = buffer_write_char(struct_ptr, "\nTemperature is: ");
    result = buffer_write_float(struct_ptr, temp);

    if (result == BUFFER_ERROR_NULL)
    {
    // raise_error(ERROR_ID_BUFFER_NULL);
    }

    if (result == BUFFER_ERROR_OVERFILL)
    {
    // raise_error(ERROR_ID_BUFFER_OVERFILLED);
    }
  }

  int main()
  {
    write_test_without_interface(&charBufferMedium);                            
    printf("\nWITHOUT INTERFACE\n");
    printf("CharBuffer.offset is: %d\n",     charBufferMedium.offset);
    printf("char_buffer_medium_arr is:\n%s\n", charBufferMedium.arr_ptr);
    BufferStatus result = buffer_clear(&charBufferMedium);                      // после использования данных - "отчищаем буфер".

    if (result == BUFFER_ERROR_NULL)
    {
    //raise_error(ERROR_ID_BUFFER_NULL);
    }

    if (result == BUFFER_ERROR_OVERFILL)
    {
    //raise_error(ERROR_ID_BUFFER_OVERFILLED);
    }
    return 0;
  }