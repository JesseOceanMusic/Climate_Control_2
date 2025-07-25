#include <stdio.h>                                                              // нужен для NULL
#include "c_buffer.h"
#include "c_errors_public.h"

#ifdef BUF__FULL_CLEAR_WITH_MEMSET
  #include <string.h>
#endif

const char* const BUF__BAD_ID = "BUF__BAD_ID";

// создаём и инициализируем буферы из списка
  #define BUFFER_LIST(buffer_id, buffer_name, buffer_size) static char buffer_name[buffer_size] = {0}; // X-MACROS
  #include "c_buffer_list.def"
  #undef  BUFFER_LIST

// Структура для хранения информации о буфере
struct BufInfoPrivate
{
    char*              ptr;
    const unsigned int size;
    unsigned int       offset;
};

// Создаём массив структур с информацией о буферах
static struct BufInfoPrivate bufInfoPrivate[BUF__ID__AMOUNT] = {
    #define BUFFER_LIST(buffer_id, buffer_name, buffer_size) [buffer_id] = {buffer_name, buffer_size, 0},
    #include "c_buffer_list.def"
    #undef BUFFER_LIST
};

// хелперы
  bool buf__is_id_correct(BufId bufId)
  {
    if(bufId >= 0 && bufId < BUF__ID__AMOUNT) { return true; }
    err__raise_error(ERR__ID__BUF__RECEIVED_WRONG_ID);
    return false;
  }

  bool buf__will_this_data_fit(BufId bufId, const char* const data_char_ptr)
  {
    if(data_char_ptr == NULL)
    {                                                                             
      err__raise_error(ERR__ID__BUF__RECEIVED_NULL);
      return false;
    }

    unsigned int needed_size = snprintf(NULL, 0, "%s", data_char_ptr);
                                  
    if(buf__get_size_left(bufId) < needed_size) { return false; }                // МЕСТО КОНЧИЛОСЬ

    return true;
  }

// операции
  bool buf__write_char(BufId bufId, const char* const data_char_ptr)             // основная функция ЗАПИСИ в буфер CHAR
  {
    if(buf__is_id_correct(bufId)                     == false) { return false; }
    if(buf__will_this_data_fit(bufId, data_char_ptr) == false)                   // хватит ли места для записи? там же проверим на NULL
    {
      err__raise_error(ERR__ID__BUF__OVERFILLED);
      return false;
    }

    bufInfoPrivate[bufId].offset += snprintf(bufInfoPrivate[bufId].ptr + bufInfoPrivate[bufId].offset, // 2. Выполняем запись. snprintf вернет количество символов, 
                                    bufInfoPrivate[bufId].size - bufInfoPrivate[bufId].offset,         // которое БЫЛО БЫ записано, если бы буфера было достаточно.
                                    "%s", data_char_ptr);                                              // Оно также запишет столько, сколько влезет, и поставит нуль-терминатор.
    return true;
  }

  bool buf__write_int(BufId bufId, const int   data_int)                         // функция записи INT в буфер
  {
    //здесь на NULL проверять не надо -> проверим в buf__write_char()
    char data_char_ptr[BUF__INT_MAX_SYMBOLS_LENGTH];
    snprintf(data_char_ptr, sizeof(data_char_ptr), "%d", data_int);
    return buf__write_char(bufId, data_char_ptr);
  }

  bool buf__write_float(BufId bufId, const float data_float, const unsigned short precision) // функция записи FLOAT в буфер
  {
    //здесь на NULL проверять не надо -> проверим в buf__write_char()
    if(precision > BUF__FLOAT_MAX_PRECISION)
    {
      err__raise_error(ERR__ID__BUF__WRONG_FLOAT_PRECISION);
      return false;
    }

    char data_char_ptr[BUF__FLOAT_MAX_SYMBOLS_LENGTH];
    snprintf(data_char_ptr, sizeof(data_char_ptr), "%.*f", precision, data_float);
                                                 // `.*` означает, что точность будет взята из следующего аргумента функции (precision)
    return buf__write_char(bufId, data_char_ptr);
  }

  bool buf__clear(BufId bufId)                                                   // функция очистки буфера
  {
    if(buf__is_id_correct(bufId) == false) { return false; }

    bufInfoPrivate[bufId].offset = 0;

    #ifdef BUF__FULL_CLEAR_WITH_MEMSET
      memset(bufInfoPrivate[bufId].ptr , 0, bufInfoPrivate[bufId].size);        // БЕЗОПАСНЕЕ // заполняет весь массив нулями
    #endif

    bufInfoPrivate[bufId].ptr[0] = '\0';                                        // БЫСТРЕЕ // только первый элемент массива = нуль-терминатор
    return true;
  }


// геттеры
  const char* buf__get_arr_ptr  (BufId bufId)
  {
    if(buf__is_id_correct(bufId) == false) { return BUF__BAD_ID; }
    return bufInfoPrivate[bufId].ptr;
  }

  unsigned int buf__get_size_left(BufId bufId)
  {
    if(buf__is_id_correct(bufId) == false) { return 0; }

    return (bufInfoPrivate[bufId].size - bufInfoPrivate[bufId].offset - 1);
    // size_left: сколько осталось байт для записи ('\0' уже вычтен)
    // arr_size_: размер массива в байтах целиком
    // offset_: указывает на индекс, в котором находится '\0'
    // в пустом массиве из 2 байт:
              // offset_    = 0
              // size_left = 1
              // arr_size_  = 2
  }

  bool buf__get_is_empty (BufId bufId)
  {
    if(buf__is_id_correct(bufId) == false) { return false; }
    if(bufInfoPrivate[bufId].offset == 0) { return true;  }
    return false;
  }
