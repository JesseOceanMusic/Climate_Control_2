#include "jesse_buffer.h"

//#define BUFFER_FULL_CLEAR_WITH_MEMSET
#ifdef BUFFER_FULL_CLEAR_WITH_MEMSET
  #include <string.h>
#endif


BufferStatus buffer_write_char(struct CharBuffer *struct_ptr, const char *data) // основная функция ЗАПИСИ в буфер CHAR
{
  if(struct_ptr == NULL) { return BUFFER_ERROR_NULL; }
  if(data == NULL) { return BUFFER_ERROR_NULL; }

  if(struct_ptr->offset < struct_ptr->arr_size - 1)                             // 1. Проверяем, есть ли место для записи (минимум 1 байт для нуль-терминатора)
  {
    struct_ptr->offset += snprintf(struct_ptr->arr_ptr + struct_ptr->offset,    // 2. Выполняем запись. snprintf вернет количество символов, 
                                   struct_ptr->arr_size - struct_ptr->offset,   // которое БЫЛО БЫ записано, если бы буфера было достаточно.
                                   "%s", data);                                 // Оно также запишет столько, сколько влезет, и поставит нуль-терминатор.
  }

  if (struct_ptr->offset >= struct_ptr->arr_size - 1)                           // 3. Проверяем, не переполнился ли offset после попытки записи.
  {
    struct_ptr->offset = struct_ptr->arr_size - 1;                              // устанавливаем offset на последний элемент массива

    static const char ARRAY_IS_FULL[] = "\n\n...ERROR: BUFFER IS OVERFILLED";
    if(struct_ptr->arr_size > sizeof(ARRAY_IS_FULL))                            // 4. Проверяем, достаточно ли места в буффере, чтобы поместилось сообщение.
    {
      struct_ptr->offset -= sizeof(ARRAY_IS_FULL);                              // сдвигаем offset на размер сообщения об ошибке (sizeof(ARRAY_IS_FULL) включает нуль-терминатор)
      struct_ptr->offset += snprintf(struct_ptr->arr_ptr + struct_ptr->offset,
                                    struct_ptr->arr_size - struct_ptr->offset,
                                    "%s", ARRAY_IS_FULL);
    }
    return BUFFER_ERROR_OVERFILL;
  }
  return BUFFER_SUCCESS;
}

BufferStatus buffer_write_int(struct CharBuffer *struct_ptr, int var)           // функция записи INT в буфер
{
  char var_char[32];
  snprintf(var_char, sizeof(var_char), "%d", var);
  return buffer_write_char(struct_ptr, var_char);
}

BufferStatus buffer_write_float(struct CharBuffer *struct_ptr, float var)       // функция записи FLOAT в буфер
{
  char var_char[32];
  snprintf(var_char, sizeof(var_char), "%.2f", var);
  return buffer_write_char(struct_ptr, var_char);
}

BufferStatus buffer_clear(struct CharBuffer *struct_ptr)                        // функция ОТЧИСТКИ буфера
{
  if(struct_ptr == NULL) { return BUFFER_ERROR_NULL; }
  struct_ptr->offset = 0;
  struct_ptr->arr_ptr[0] = '\0';                                                // БЫСТРЕЕ // только первый элемент массива = нуль-терминатор

  #ifdef BUFFER_FULL_CLEAR_WITH_MEMSET
    memset(struct_ptr->arr_ptr , 0, struct_ptr->arr_size);                      // БЕЗОПАСНЕЕ // заполняет весь массив нулями
  #endif

  return BUFFER_SUCCESS;
}