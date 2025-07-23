#include "stdio.h"                                                              // для NULL. не оверхед, потому что она и так уже есть в буфере для snsprintf.
#include "c_errors_internal.h"
#include "c_errors_writer.h"

const char* ERR_HEADER_MESSAGE = "id/total counter/unhandled counter/description:\n";

static bool errwr__write_error_to_buffer(BufId bufId, ErrId errId)
{
  struct ErrInfo errInfo;
  if(err_get_info(&errInfo, errId) == false) { return false; }

  if(errInfo.description_ptr == NULL)                                           // защита от дурака, который мог добавить description 'NULL' в c_errors_list.def
  {                                                                             
    err_raise_error(ERR_ID__ERR_DESCRIPTION_IS_NULL);
    return false;
  }

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

bool errwr__write_one_err(BufId bufId, ErrId errId, ErrType errType, bool only_unhandled)
{
  if(buf_is_id_correct  (bufId)   == false) { return false; }
  if(err_is_id_correct  (errId)   == false) { return false; }
  if(err_is_type_correct(errType) == false) { return false; }

  struct ErrInfo errInfo;
  if(err_get_info(&errInfo, errId) == false) { return false; }

  if(errInfo.type == errType || errType == ERR_TYPE__ANY_TYPE)                                        // проверяем нужно ли выводить эту ошибку
  {
    if(only_unhandled == false || errInfo.unhandled == true)                                          // проверяем нужно ли выводить эту ошибку
    {
      if(buf_clear(BUF_ID__ERR_TEMP)                                      == false) { return false; } // отчищаем временный буфер
      if(errwr__write_error_to_buffer(BUF_ID__ERR_TEMP, errId)            == false) { return false; } // записываем в временный буфер
      if(buf_will_this_data_fit(bufId, buf_get_arr_ptr(BUF_ID__ERR_TEMP)) == false) { return false; } // проверяем влезет ли в предоставленный буфер сообщение целиком
      if(buf_write_char(bufId, buf_get_arr_ptr(BUF_ID__ERR_TEMP))         == false) { return false; } // записываем
      if(only_unhandled == true)                                              
      {
        if(err_reset_counter_and_flag(errId)                              == false) { return false; } // обрабатываем
      }
    }
  }
  return true;
}