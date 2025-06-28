#include "c_errors_writer.h"

static char char_buffer_for_errors_arr[BUFFER_FOR_ERRORS_SIZE];               // создаём массив (буфер)       
static struct BufInfo bufInfo =                                // инициализируем структуру для массива (буфера)
{
  .arr_ptr  = char_buffer_for_errors_arr,                                     // указатель на массив (буфер)
  .arr_size = sizeof(char_buffer_for_errors_arr),                             // размер массива (буфера)
  .offset   = 0,                                                              // offset(смещение) равно нулю
};

static struct ErrWriterInfo errWriterInfo =
{
  .arr_ptr     = char_buffer_for_errors_arr,
  .msg_length  = 0,
  .state       = ERR_WRITER__CUR_TASK__NONE,
  .type_filter = ERR_TYPE__ANY_TYPE,
};

static bool  err_writer_init_flag = false;
static ErrId err_writer_cur_err_id = 0;

  bool err_writer(struct BufInfo *struct_ptr, ErrId errId, ErrType errType_filter, bool only_unhandled)
  {
    struct ErrInfo errInfo = err_get_info(errId);
    if(errInfo.was_id_correct == false)
    {
      return false;
    }

    if(errInfo.type == errType_filter || errType_filter == ERR_TYPE__ANY_TYPE)
    {
      if(only_unhandled == false || errInfo.unhandled == true)
      {
        //print
      }
    }
  }

static void write_error_to_buffer (ErrId id, bool print_counter_for_unhandled_errors)
{
  struct ErrInfo errInfo = err_get_info(id);

  buf_write_int(&bufInfo, (int)errInfo.id);                                   // ID
  buf_write_char(&bufInfo, "/");
  
  if(print_counter_for_unhandled_errors == false)
  {
    buf_write_int(&bufInfo, (int)errInfo.counter_cur);                        // counter (счётчик за всё время работы программы)
  }
  else
  {
    unsigned short unhandled_errors_calc;
    unhandled_errors_calc = errInfo.counter_cur - errInfo.counter_last;
    buf_write_int(&bufInfo, (int)unhandled_errors_calc);                      // counter (счетчик только необработанных ошибок)
  }

  buf_write_char(&bufInfo, "/");
  buf_write_char(&bufInfo, errInfo.description_ptr);                          // description
  buf_write_char(&bufInfo, "\n");
}

static bool is_buffer_has_enough_space(ErrId id)
{
  struct ErrInfo errInfo = err_get_info(id);
  unsigned int needed_size = 1 + snprintf(NULL, 0, "%d/%d/%s\n", 
                                (int)id, 
                                (int)errInfo.counter_cur,                     // cur всегда будет больше, чем разница, так что можем считать для "худшего" случая 
                                errInfo.description_ptr);
                                
  if (buf_size_left(&bufInfo) < needed_size)
  {
    return false; // МЕСТО КОНЧИЛОСЬ
  }
  return true;
}

static bool is_there_any_error_with_cur_type()
{
  struct ErrInfo errInfo;

  for(ErrId id = 0; id < ERR_ID__AMOUNT; id++)
  {
    errInfo = err_get_info(id);

    if(errInfo.type == errWriterInfo.type_filter)
    {
      return true;
    }
  }
  err_raise_error(ERR_ID__ERR_WR_UNLISTED_ERROR_TYPE);
  return false;
}

static void err_writer_init()
{
  // Проверяем есть ли вообще ошибки с нужным типом
    if(is_there_any_error_with_cur_type() == false)
    {
      // Ошибок с таким типом не существует
      errWriterInfo.state = ERR_WRITER__CUR_TASK__COMPLETE;        
    }
  // UNHANDLED ERRORS - Проверяем есть ли ошибки для записи нужного типа
    else if(errWriterInfo.state == ERR_WRITER__CUR_TASK__WRITE_UNHANDLED_ERRORS &&
            err_has_unhandled_errors(errWriterInfo.type_filter) == false)
    {
      // Нет необработанных ошибок с таким типом
      errWriterInfo.state = ERR_WRITER__CUR_TASK__COMPLETE;
    }
  // Добавляем заголовок.
    else if(buf_write_char(&bufInfo, ERR_HEADER_MESSAGE) == false)
    {
      // не хватило места, чтобы записать заголовок... ошибка добавится в буфере - ERR_ID__BUF_OVERFILLED
      errWriterInfo.state = ERR_WRITER__CUR_TASK__ERROR;
    }
}

static void err_writer_FSM()
{
  // Проверяем есть ли место для записи ошибки.
    if (is_buffer_has_enough_space(err_writer_cur_err_id) == false)
    {
      errWriterInfo.state = ERR_WRITER__CUR_TASK__ERROR;
      return;    // МЕСТО КОНЧИЛОСЬ, НЕЛЬЗЯ ЗАПИСАТЬ ДАЖЕ ОДНУ ОШИБКУ - ВСЁ ПЛОХО!
    }
  // Фильтр ошибок
    struct ErrInfo errInfo = err_get_info(err_writer_cur_err_id);

    if(errInfo.type == errWriterInfo.type_filter)
    {
      // ALL ERRORS
        if (errWriterInfo.state == ERR_WRITER__CUR_TASK__WRITE_ALL_ERRORS)
        {
          write_error_to_buffer(err_writer_cur_err_id, false);
        }
      // UNHANDLED ERRORS
        else if (errWriterInfo.state == ERR_WRITER__CUR_TASK__WRITE_UNHANDLED_ERRORS)
        {
          if(errInfo.unhandled == true)
          {
            write_error_to_buffer(err_writer_cur_err_id, true);
            err_reset_counter_and_flag(err_writer_cur_err_id);               // регистрируем обработку ошибку
          }
        }
      // Проверяем записали ли все ошибки
        if(err_writer_cur_err_id == ERR_ID__AMOUNT - 1)
        {
          errWriterInfo.state = ERR_WRITER__CUR_TASK__COMPLETE;
        }
      // инкремент счетчика ПОСЛЕ проверки
        err_writer_cur_err_id++;
}

struct ErrWriterInfo err_writer__get_info()
{
  return errWriterInfo;
}

struct ErrWriterInfo err_writer__set_task__all_errors(ErrType handle_this_type)
{
  errWriterInfo.type_filter = handle_this_type;
  errWriterInfo.state = ERR_WRITER__CUR_TASK__WRITE_ALL_ERRORS;
  err_writer_cur_err_id = 0;                                              // Сбрасываем итератор
  err_writer_init_flag = true;                                               
  return errWriterInfo;
}

struct ErrWriterInfo err_writer__set_task__unhandled_errors(ErrType handle_this_type)
{
  errWriterInfo.type_filter = handle_this_type;
  errWriterInfo.state = ERR_WRITER__CUR_TASK__WRITE_UNHANDLED_ERRORS;
  err_writer_cur_err_id = 0;                                              // Сбрасываем итератор
  err_writer_init_flag = true;
  return errWriterInfo;
}

struct ErrWriterInfo err_writer__write_next_part()
{    
  if(errWriterInfo.state == ERR_WRITER__CUR_TASK__WRITE_ALL_ERRORS ||
     errWriterInfo.state == ERR_WRITER__CUR_TASK__WRITE_UNHANDLED_ERRORS)
  {
      // Отчищаем буфер
        buf_clear(&bufInfo);
      // делаем проверки и записываем заголовок
        if(err_writer_init_flag == true)
        {
          err_writer_init();
          err_writer_init_flag = false;
        }
      // записываем ошибки по одной
        else
        {
          err_writer_FSM();
        }                             
  }
  errWriterInfo.msg_length = bufInfo.offset;                                  // обновляем количество символов
  return errWriterInfo;
}