#include <stdio.h>                                                              // нужен для NULL
#include "c_time_wrapper.h"
#include "c_errors_public.h"

  static time_t current_time_UNIX;                                              // переменная для хранения текущего времени в UTC
  static struct tm *currentTime;                                                // создаём структуру для хранения даты и времени
  
  static bool time_wrapper__update()
  {
    current_time_UNIX = time(NULL);                                             // "обновляем" время
    currentTime = localtime(&current_time_UNIX);                                // заполняем структуру
    if(currentTime == NULL)                    // LCOV_EXCL_START               // крайне редкая ситуация
    { 
      err_raise_error(ERR_ID__TIME_WRAPPED_NULL);
      return false;
    }                                          // LCOV_EXCL_STOP
    return true;
  }

  time_t time_wrapper__get_UNIX()
  {
    time_wrapper__update();
    return current_time_UNIX;
  }

  int time_wrapper__get_YEAR()
  {
    if(time_wrapper__update() == false) { return 0; }   // LCOV_EXCL_LINE
    return (currentTime->tm_year + 1900);
  }

  int time_wrapper__get_MONTH()
  {
    if(time_wrapper__update() == false) { return 0; }   // LCOV_EXCL_LINE
    return (currentTime->tm_mon + 1);
  }

  int time_wrapper__get_DAY()
  {
    if(time_wrapper__update() == false) { return 0; }   // LCOV_EXCL_LINE
    return (currentTime->tm_mday);
  }

  int time_wrapper__get_HOUR()
  {
    if(time_wrapper__update() == false) { return 0; }   // LCOV_EXCL_LINE
    return (currentTime->tm_hour);
  }

  int time_wrapper__get_MIN()
  {
    if(time_wrapper__update() == false) { return 0; }   // LCOV_EXCL_LINE
    return (currentTime->tm_min);
  }

  int time_wrapper__get_SEC()
  {
    if(time_wrapper__update() == false) { return 0; }   // LCOV_EXCL_LINE
    return (currentTime->tm_sec);
  }

  /*
    tm_sec: секунды от 0 до 60
    tm_min: минуты от 0 до 59
    tm_hour: часы от 0 до 23
    tm_mday: день месяца от 1 до 31
    tm_mon: месяц от 0 до 11
    tm_year: год, начиная с 1900
    tm_wday: день недели от 0 до 6 (воскресенье имеет номер 0)
    tm_yday: количество дней года, прошедших с 1 января, от 0 до 365
    tm_isdst: если больше 0, то установлен переход на летнее время. Если равно 0, то переход на летнее время не действует.
  */