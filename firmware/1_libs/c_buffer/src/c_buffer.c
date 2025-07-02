#include "c_buffer.h"

//#define BUFFER_FULL_CLEAR_WITH_MEMSET
#ifdef BUFFER_FULL_CLEAR_WITH_MEMSET
  #include <string.h>
#endif

static void buf_update_size_left(struct BufHandle *struct_ptr)
{
  struct_ptr->size_left = struct_ptr->arr_size_ - struct_ptr->offset_ - 1;
  // size_left: сколько осталось байт для записи ('\0' уже вычтен)
  // arr_size_: размер массива в байтах целиком
  // offset_: указывает на индекс, в котором находится '\0'
  // в пустом массиве из 2 байт:
            // offset_    = 0
            // size_left = 1
            // arr_size_  = 2
}

bool buf_init(struct BufHandle *struct_ptr, char* arr_ptr, unsigned int arr_size_)
{
  if(struct_ptr == NULL || arr_ptr == NULL)
  {
    err_raise_error(ERR_ID__BUF_RECEIVED_NULL);
    return false;
  }

  struct_ptr->arr_ptr = arr_ptr;
  struct_ptr->arr_size_ = arr_size_;
  struct_ptr->offset_ = 0;
  buf_update_size_left(struct_ptr);
  struct_ptr->is_empty = true;
  return true;
}

bool buf_write_char(struct BufHandle *struct_ptr, const char *data_char)          // основная функция ЗАПИСИ в буфер CHAR
{
  if(struct_ptr == NULL || data_char == NULL)
  {
    err_raise_error(ERR_ID__BUF_RECEIVED_NULL);
    return false;
  }

  unsigned int needed_size = 1 + snprintf(NULL, 0, data_char);

  if(struct_ptr->offset_ + needed_size <= struct_ptr->arr_size_)                  // 1. Проверяем, есть ли место для записи
  {
    struct_ptr->offset_ += snprintf(struct_ptr->arr_ptr + struct_ptr->offset_,    // 2. Выполняем запись. snprintf вернет количество символов, 
                                   struct_ptr->arr_size_ - struct_ptr->offset_,   // которое БЫЛО БЫ записано, если бы буфера было достаточно.
                                   "%s", data_char);                            // Оно также запишет столько, сколько влезет, и поставит нуль-терминатор.
    
    buf_update_size_left(struct_ptr);
    struct_ptr->is_empty = false;
    return true;
  }
  return false;
}

bool buf_write_int(struct BufHandle *struct_ptr, int data_int)                    // функция записи INT в буфер
{
  //здесь на NULL проверять не надо -> проверим в buf_write_char()
  char data_char[BUF_INT_MAX_SYMBOLS_LENGTH];
  snprintf(data_char, sizeof(data_char), "%d", data_int);
  return buf_write_char(struct_ptr, data_char);
}

bool buf_write_float(struct BufHandle *struct_ptr, float data_float, unsigned short precision) // функция записи FLOAT в буфер
{
  //здесь на NULL проверять не надо -> проверим в buf_write_char()
  if(precision > BUF_FLOAT_MAX_PRECISION)
  {
    err_raise_error(ERR_ID__BUF_BAD_FLOAT_PRECISION);
    return false;
  }

  char data_char[BUF_FLOAT_MAX_SYMBOLS_LENGTH];
  snprintf(data_char, sizeof(data_char), "%.*f", precision, data_float);
                                          // `.*` означает, что точность будет взята из следующего аргумента функции (precision)
  return buf_write_char(struct_ptr, data_char);
}

bool buf_clear(struct BufHandle *struct_ptr)                                      // функция ОТЧИСТКИ буфера
{
  if(struct_ptr == NULL)
  {
    err_raise_error(ERR_ID__BUF_RECEIVED_NULL);
    return false;
  }

  struct_ptr->offset_ = 0;  
  struct_ptr->is_empty = true;
  buf_update_size_left(struct_ptr);

  #ifdef BUFFER_FULL_CLEAR_WITH_MEMSET
    memset(struct_ptr->arr_ptr , 0, struct_ptr->arr_size_);                      // БЕЗОПАСНЕЕ // заполняет весь массив нулями
  #endif

  struct_ptr->arr_ptr[0] = '\0';                                                // БЫСТРЕЕ // только первый элемент массива = нуль-терминатор

  return true;
}