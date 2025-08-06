#include "stdio.h"                                                              // для NULL. не оверхед, потому что она и так уже есть в буфере для snsprintf.
#include "c_errors_internal.h"
#include "c_errors_writer.h"

const char* const ERR_HEADER_MESSAGE = "id/total counter/unhandled counter/description:\n";

static bool errwr__write_error_to_buffer(BufId bufId, ErrId errId)
{
  struct ErrInfo errInfo;
  if(err__get_info(&errInfo, errId) == false) { return false; }                                                                           // LCOV_EXCL_BR_LINE // в текущей реализации буффера из-за предварительных проверок эта ветка недостижима. но с целью консистентности и независимости c_errors_writer - эта проверка остаётся! defensive programming

  if(errInfo.description_ptr == NULL)                                           // защита от description 'NULL' в c_errors_list.def      // LCOV_EXCL_START // эта ветка недостижима, тк c_buffer_test уже проверяет инициализацию c_buffer_list.def, но защита здесь необходима! defensive programming
  {                                                                             
    err__raise_error(ERR__ID__ERR__DESCRIPTION_IS_NULL);
    return false;
  }                                                                                                                                      // LCOV_EXCL_STOP // эта ветка недостижима, тк c_buffer_test уже проверяет инициализацию c_buffer_list.def, но защита здесь необходима! defensive programming

  if(buf__write_int (bufId, (int)errInfo.id)                == false) { return false; }   // ID                                           // LCOV_EXCL_BR_LINE // в текущей реализации буффера из-за предварительных проверок эта ветка недостижима. но с целью консистентности и независимости c_errors_writer - эта проверка остаётся! defensive programming
  if(buf__write_char(bufId, "/")                            == false) { return false; }   // /                                            // LCOV_EXCL_BR_LINE // в текущей реализации буффера из-за предварительных проверок эта ветка недостижима. но с целью консистентности и независимости c_errors_writer - эта проверка остаётся! defensive programming
  if(buf__write_int (bufId, (int)errInfo.counter_total)     == false) { return false; }   // total counter                                // LCOV_EXCL_BR_LINE // в текущей реализации буффера из-за предварительных проверок эта ветка недостижима. но с целью консистентности и независимости c_errors_writer - эта проверка остаётся! defensive programming
  if(buf__write_char(bufId, "/")                            == false) { return false; }   // /                                            // LCOV_EXCL_BR_LINE // в текущей реализации буффера из-за предварительных проверок эта ветка недостижима. но с целью консистентности и независимости c_errors_writer - эта проверка остаётся! defensive programming
  if(buf__write_int (bufId, (int)errInfo.counter_unhandled) == false) { return false; }   // unhandled counter                            // LCOV_EXCL_BR_LINE // в текущей реализации буффера из-за предварительных проверок эта ветка недостижима. но с целью консистентности и независимости c_errors_writer - эта проверка остаётся! defensive programming
  if(buf__write_char(bufId, "/")                            == false) { return false; }   // /                                            // LCOV_EXCL_BR_LINE // в текущей реализации буффера из-за предварительных проверок эта ветка недостижима. но с целью консистентности и независимости c_errors_writer - эта проверка остаётся! defensive programming
  if(buf__write_char(bufId, errInfo.description_ptr)        == false) { return false; }   // description                                  // LCOV_EXCL_BR_LINE // в текущей реализации буффера из-за предварительных проверок эта ветка недостижима. но с целью консистентности и независимости c_errors_writer - эта проверка остаётся! defensive programming
  if(buf__write_char(bufId, "\n")                           == false) { return false; }   // \n                                           // LCOV_EXCL_BR_LINE // в текущей реализации буффера из-за предварительных проверок эта ветка недостижима. но с целью консистентности и независимости c_errors_writer - эта проверка остаётся! defensive programming

  return true;
}

bool errwr__write_one_err(BufId bufId, ErrId errId, ErrType errType, bool only_unhandled)
{
  if(buf__is_id_correct  (bufId)   == false) { return false; }
  if(err__is_id_correct  (errId)   == false) { return false; }
  if(err__is_type_correct(errType) == false) { return false; }

  struct ErrInfo errInfo;
  if(err__get_info(&errInfo, errId) == false) { return false; }                                                                           // LCOV_EXCL_BR_LINE // в текущей реализации буффера из-за предварительных проверок эта ветка недостижима. но с целью консистентности и независимости c_errors_writer - эта проверка остаётся! defensive programming

  if(errInfo.type == errType || errType == ERR__TYPE__ANY_TYPE)                                        // проверяем нужно ли выводить эту ошибку
  {
    if(only_unhandled == false || errInfo.unhandled == true)                                          // проверяем нужно ли выводить эту ошибку
    {
      if(buf__clear(BUF__ID__ERR_TEMP)                                       == false) { return false; } // отчищаем временный буфер        // LCOV_EXCL_BR_LINE // в текущей реализации буффера из-за предварительных проверок эта ветка недостижима. но с целью консистентности и независимости c_errors_writer - эта проверка остаётся! defensive programming
      if(errwr__write_error_to_buffer(BUF__ID__ERR_TEMP, errId)              == false) { return false; } // записываем в временный буфер    // LCOV_EXCL_BR_LINE // в текущей реализации буффера из-за предварительных проверок эта ветка недостижима. но с целью консистентности и независимости c_errors_writer - эта проверка остаётся! defensive programming
      if(buf__will_this_data_fit(bufId, buf__get_arr_ptr(BUF__ID__ERR_TEMP)) == false) { return false; } // проверяем влезет ли в предоставленный буфер сообщение целиком
      if(buf__write_char(bufId, buf__get_arr_ptr(BUF__ID__ERR_TEMP))         == false) { return false; } // записываем                      // LCOV_EXCL_BR_LINE // в текущей реализации буффера из-за предварительных проверок эта ветка недостижима. но с целью консистентности и независимости c_errors_writer - эта проверка остаётся! defensive programming
      if(only_unhandled == true)                                              
      {
        if(err__reset_counter_and_flag(errId)                              == false) { return false; } // обрабатываем                    // LCOV_EXCL_BR_LINE // в текущей реализации буффера из-за предварительных проверок эта ветка недостижима. но с целью консистентности и независимости c_errors_writer - эта проверка остаётся! defensive programming
      }
    }
  }
  return true;
}