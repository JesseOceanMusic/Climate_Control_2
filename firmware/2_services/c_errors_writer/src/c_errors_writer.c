#include "c_errors_writer.h"

static bool is_buffer_has_enough_space(struct BufInfo *struct_ptr, ErrId errId)
{
  struct ErrInfo errInfo;
  err_get_info(&errInfo, errId);

  if(errInfo.description_ptr == NULL)                                           // защита от дурака, который мог добавить description == NULL в c_errors_list.def
  {                                                                             
    err_raise_error(ERR_ID__ERR_DESCRIPTION_IS_NULL);
    return false;
  }

  unsigned int needed_size = snprintf(NULL, 0, "%d/%d/%s\n", 
                                (int)errId, 
                                (int)errInfo.counter_total,                     // counter_total всегда будет >= counter_unhandled считать для "худшего" случая 
                                errInfo.description_ptr);
                                
  if (struct_ptr->size_left < needed_size)
  {
    return false; // МЕСТО КОНЧИЛОСЬ
  }
  return true;
}

static void write_error_to_buffer(struct BufInfo *struct_ptr, ErrId errId, bool only_unhandled)
{
  struct ErrInfo errInfo;
  err_get_info(&errInfo, errId);

  buf_write_int(struct_ptr, (int)errInfo.id);                                 // ID
  buf_write_char(struct_ptr, "/");
  
  if(only_unhandled == true)
  {
    buf_write_int(struct_ptr, (int)errInfo.counter_unhandled);
  }
  else
  {
    buf_write_int(struct_ptr, (int)errInfo.counter_total);
  }

  buf_write_char(struct_ptr, "/");
  buf_write_char(struct_ptr, errInfo.description_ptr);                          // description
  buf_write_char(struct_ptr, "\n");
}

bool err_writer(struct BufInfo *struct_ptr, ErrId errId, ErrType errType_filter, bool only_unhandled)
{
  if(struct_ptr == NULL)
  {
    err_raise_error(ERR_ID__ERR_WR_RECEIVED_NULL);
    return false;
  }

  if(err_is_type_correct(errType_filter) == false || err_is_id_correct(errId) == false) // проверка не ради защиты err_get_info(), а чтобы быстрее выйте из функции при неправильных аргументах
  {
    err_raise_error(ERR_ID__ERR_WR_WRONG_ID_OR_TYPE);
    return false;
  }

  struct ErrInfo errInfo;
  if(err_get_info(&errInfo, errId) == false)
  {
    err_raise_error(ERR_ID__ERR_WR_WRONG_ID_OR_TYPE);    
    return false;
  }

  if(errInfo.type == errType_filter || errType_filter == ERR_TYPE__ANY_TYPE)
  {
    if(only_unhandled == false || errInfo.unhandled == true)                    // проверяем быстро флаги
    {
      if(is_buffer_has_enough_space(struct_ptr, errId) == false)                // проверяем хватит ли места
      {
        return false;
      }
      if(only_unhandled == true)                                              
      {
        err_reset_counter_and_flag(errId);                                      // обрабатываем
      }
      write_error_to_buffer(struct_ptr, errId, only_unhandled);                 // записываем
    }
  }
  return true;
}