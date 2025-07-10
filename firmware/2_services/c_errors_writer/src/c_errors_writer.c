#include "c_errors_internal.h"
#include "stdio.h"
#include <string.h>                                                             // для strlen()
#include "c_errors_writer.h"


static bool is_buffer_has_enough_space(BufId bufId, ErrId errId)
{
  struct ErrInfo errInfo;
  if(err_get_info(&errInfo, errId) == false) { return false; }

  if(errInfo.description_ptr == NULL)                                           // защита от дурака, который мог добавить description == NULL в c_errors_list.def
  {                                                                             
    err_raise_error(ERR_ID__ERR_DESCRIPTION_IS_NULL);
    return false;
  }

  unsigned int needed_size = snprintf(NULL, 0, "%d/%d/%s\n", 
                                (int)errId, 
                                (int)errInfo.counter_total,                     // counter_total всегда будет >= counter_unhandled считать для "худшего" случая 
                                errInfo.description_ptr);
                                
  if(buf_get_size_left(bufId) < needed_size) { return false; }                  // МЕСТО КОНЧИЛОСЬ

  return true;
}

static bool write_error_to_buffer(BufId bufId, ErrId errId)
{
  struct ErrInfo errInfo;
  if(err_get_info(&errInfo, errId) == false) { return false; }

  if(buf_write_int (bufId, (int)errInfo.id)                == false) { return false; }   // ID
  if(buf_write_char(bufId, "/")                            == false) { return false; }   // /
  if(buf_write_int (bufId, (int)errInfo.counter_total)     == false) { return false; }   // total counter
  if(buf_write_char(bufId, "/")                            == false) { return false; }   // /
  if(buf_write_int (bufId, (int)errInfo.counter_unhandled) == false) { return false; }   // unhandled counter
  if(buf_write_char(bufId, "/")                            == false) { return false; }   // /
  if(buf_write_char(bufId, errInfo.description_ptr)        == false) { return false; }   // description
  if(buf_write_char(bufId, "\n")                           == false) { return false; }   // \n

  return true;
}

bool err_writer__write_one_err(BufId bufId, ErrId errId, ErrType errType, bool only_unhandled)
{
  struct ErrInfo errInfo;
  if(err_is_id_correct(errId)      == false) { return false; }
  if(err_is_type_correct(errType)  == false) { return false; }
  if(err_get_info(&errInfo, errId) == false) { return false; }


  if(errInfo.type == errType || errType == ERR_TYPE__ANY_TYPE)
  {
    if(only_unhandled == false || errInfo.unhandled == true)                    // проверяем быстро флаги
    {
      if(is_buffer_has_enough_space(bufId, errId)  == false) { return false; }  // проверяем хватит ли места

      if(only_unhandled == true)                                              
      {
        if(err_reset_counter_and_flag(errId) == false) { return false; }        // обрабатываем
      }
      if(write_error_to_buffer(bufId, errId) == false) { return false; }        // записываем
    }
  }
  return true;
}