#include "j_buffer.h"

//#define BUFFER_FULL_CLEAR_WITH_MEMSET
#ifdef BUFFER_FULL_CLEAR_WITH_MEMSET
  #include <string.h>
#endif

const char* ARRAY_IS_FULL_FOOTER_MSG = "\n\n...ERROR: BUFFER IS OVERFILLED";

bool buffer_write_char(struct CharBuffer *struct_ptr, const char *data)         // основная функция ЗАПИСИ в буфер CHAR
{
  if(struct_ptr == NULL || data == NULL)
  {
    raise_error(ERROR_ID_BUFFER_NULL);
    return false;
  }

  if(struct_ptr->offset < struct_ptr->arr_size - 1)                             // 1. Проверяем, есть ли место для записи (минимум 1 байт для нуль-терминатора)
  {
    struct_ptr->offset += snprintf(struct_ptr->arr_ptr + struct_ptr->offset,    // 2. Выполняем запись. snprintf вернет количество символов, 
                                   struct_ptr->arr_size - struct_ptr->offset,   // которое БЫЛО БЫ записано, если бы буфера было достаточно.
                                   "%s", data);                                 // Оно также запишет столько, сколько влезет, и поставит нуль-терминатор.
  }

  if (struct_ptr->offset >= struct_ptr->arr_size)                               // 3. Проверяем, не переполнился ли offset после попытки записи.
  {
    struct_ptr->offset = struct_ptr->arr_size - 1;                              // устанавливаем offset на последний элемент массива
    
    unsigned int needed_size = 1 + snprintf(NULL, 0, "%s", ARRAY_IS_FULL_FOOTER_MSG);
    if(struct_ptr->arr_size >= needed_size)                                     // 4. Проверяем, достаточно ли места в буффере, чтобы поместилось сообщение.
    {
      struct_ptr->offset -= needed_size - 1;                                    // сдвигаем offset на размер сообщения об ошибке (sizeof(ARRAY_IS_FULL_FOOTER_MSG) включает нуль-терминатор)
      struct_ptr->offset += snprintf(struct_ptr->arr_ptr + struct_ptr->offset,
                                    struct_ptr->arr_size - struct_ptr->offset,
                                    "%s", ARRAY_IS_FULL_FOOTER_MSG);
    }
    raise_error(ERROR_ID_BUFFER_OVERFILLED);
    return false;
  }
  return true;
}

bool buffer_write_int(struct CharBuffer *struct_ptr, int var)                   // функция записи INT в буфер
{
  //здесь на NULL проверять не надо -> проверим в buffer_write_char()
  char var_char[32];
  snprintf(var_char, sizeof(var_char), "%d", var);
  return buffer_write_char(struct_ptr, var_char);
}

bool buffer_write_float(struct CharBuffer *struct_ptr, float var)               // функция записи FLOAT в буфер
{
  //здесь на NULL проверять не надо -> проверим в buffer_write_char()
  char var_char[32];
  snprintf(var_char, sizeof(var_char), "%.2f", var);
  return buffer_write_char(struct_ptr, var_char);
}

bool buffer_clear(struct CharBuffer *struct_ptr)                                // функция ОТЧИСТКИ буфера
{
  if(struct_ptr == NULL)
  {
    raise_error(ERROR_ID_BUFFER_NULL);
    return false;
  }

  struct_ptr->offset = 0;
  struct_ptr->arr_ptr[0] = '\0';                                                // БЫСТРЕЕ // только первый элемент массива = нуль-терминатор

  #ifdef BUFFER_FULL_CLEAR_WITH_MEMSET
    memset(struct_ptr->arr_ptr , 0, struct_ptr->arr_size);                      // БЕЗОПАСНЕЕ // заполняет весь массив нулями
  #endif

  return true;
}

bool is_buffer_empty(struct CharBuffer *struct_ptr)
{
  if(struct_ptr == NULL)
  {
    raise_error(ERROR_ID_BUFFER_NULL);
    return false;
  }

  if(struct_ptr->offset == 0)
  {
    return true;
  }
  return false;
}

unsigned int buffer_size_left(struct CharBuffer *struct_ptr)
{
  if(struct_ptr == NULL)
  {
    raise_error(ERROR_ID_BUFFER_NULL);
    return 0;
  }

  unsigned int size_left = struct_ptr->arr_size - struct_ptr->offset;
  return size_left;
}