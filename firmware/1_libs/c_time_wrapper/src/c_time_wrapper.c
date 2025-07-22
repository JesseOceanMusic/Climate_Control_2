#include <stdio.h>                                                              // нужен для NULL
#include "c_time_wrapper.h"
#include "c_errors_public.h"

  static char time_wrapper__DATE__YYYY_arr[5];                                  // [Y][Y][Y][Y][\0]
  static char time_wrapper__DATE__MM_arr[3];                                    // [M][M][\0]
  static char time_wrapper__DATE__DD_arr[3];                                    // [D][D][\0]

  static char time_wrapper__TIME__HH_arr[3];                                    // [H][H][\0]
  static char time_wrapper__TIME__MM_arr[3];                                    // [M][M][\0]
  static char time_wrapper__TIME__SS_arr[3];                                    // [S][S][\0]

  static char time_wrapper__DATE__YYYY_MM_DD_arr[11];                           // [Y][Y][Y][Y][-][M][M][-][D][D][\0] 
  static char time_wrapper__TIME__HH_MM_SS_arr[9];                              // [H][H][:][M][M][:][S][S][\0]


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

  static void time_wrapper__set_arr__target(char* arr_ptr, size_t arr_size, int value)
  {
    for(size_t i = (arr_size - 1); i > 0; i--) // проходимся по массиву с конца
    {
      arr_ptr[i - 1] = (value % 10) + '0';     // начинаем с предпоследнего индекса и заканчивает нулевым
      value /= 10;
    }
    arr_ptr[arr_size - 1] = '\0';              // ставим нуль терминатор в конце
  }

  static void time_wrapper__update_arr_DATE__YYYY_MM_DD()
  {
    time_wrapper__DATE__YYYY_MM_DD_arr[0]  = time_wrapper__DATE__YYYY_arr[0];
    time_wrapper__DATE__YYYY_MM_DD_arr[1]  = time_wrapper__DATE__YYYY_arr[1];
    time_wrapper__DATE__YYYY_MM_DD_arr[2]  = time_wrapper__DATE__YYYY_arr[2];
    time_wrapper__DATE__YYYY_MM_DD_arr[3]  = time_wrapper__DATE__YYYY_arr[3];
    time_wrapper__DATE__YYYY_MM_DD_arr[4]  = '-';
    time_wrapper__DATE__YYYY_MM_DD_arr[5]  = time_wrapper__DATE__MM_arr[0];
    time_wrapper__DATE__YYYY_MM_DD_arr[6]  = time_wrapper__DATE__MM_arr[1];
    time_wrapper__DATE__YYYY_MM_DD_arr[7]  = '-';
    time_wrapper__DATE__YYYY_MM_DD_arr[8]  = time_wrapper__TIME__SS_arr[0];
    time_wrapper__DATE__YYYY_MM_DD_arr[9]  = time_wrapper__TIME__SS_arr[1];
    time_wrapper__DATE__YYYY_MM_DD_arr[10] = '\0';
  }

  static void time_wrapper__update_arr_TIME__HH_MM_SS()
  {
    time_wrapper__TIME__HH_MM_SS_arr[0] = time_wrapper__TIME__HH_arr[0];
    time_wrapper__TIME__HH_MM_SS_arr[1] = time_wrapper__TIME__HH_arr[1];
    time_wrapper__TIME__HH_MM_SS_arr[2] = ':';
    time_wrapper__TIME__HH_MM_SS_arr[3] = time_wrapper__TIME__MM_arr[0];
    time_wrapper__TIME__HH_MM_SS_arr[4] = time_wrapper__TIME__MM_arr[1];
    time_wrapper__TIME__HH_MM_SS_arr[5] = ':';
    time_wrapper__TIME__HH_MM_SS_arr[6] = time_wrapper__TIME__SS_arr[0];
    time_wrapper__TIME__HH_MM_SS_arr[7] = time_wrapper__TIME__SS_arr[1];
    time_wrapper__TIME__HH_MM_SS_arr[8] = '\0';
  }

  static void time_wrapper__reset_arr()
  {
    time_wrapper__set_arr__target(time_wrapper__DATE__YYYY_arr, sizeof(time_wrapper__DATE__YYYY_arr), 0);
    time_wrapper__set_arr__target(time_wrapper__DATE__MM_arr,   sizeof(time_wrapper__DATE__MM_arr),   0);
    time_wrapper__set_arr__target(time_wrapper__DATE__DD_arr,   sizeof(time_wrapper__DATE__DD_arr),   0);
    time_wrapper__set_arr__target(time_wrapper__TIME__HH_arr,   sizeof(time_wrapper__TIME__HH_arr),   0);
    time_wrapper__set_arr__target(time_wrapper__TIME__MM_arr,   sizeof(time_wrapper__TIME__MM_arr),   0);
    time_wrapper__set_arr__target(time_wrapper__TIME__SS_arr,   sizeof(time_wrapper__TIME__SS_arr),   0);
  }

  static void time_wrapper__update_arr()
  {
    if(time_wrapper__update() == false)
    {
      time_wrapper__reset_arr();
    }
    else
    {
      time_wrapper__set_arr__target(time_wrapper__DATE__YYYY_arr, sizeof(time_wrapper__DATE__YYYY_arr), time_wrapper__get_val__YEAR());
      time_wrapper__set_arr__target(time_wrapper__DATE__MM_arr,   sizeof(time_wrapper__DATE__MM_arr),   time_wrapper__get_val__MONTH());
      time_wrapper__set_arr__target(time_wrapper__DATE__DD_arr,   sizeof(time_wrapper__DATE__DD_arr),   time_wrapper__get_val__DAY());
      time_wrapper__set_arr__target(time_wrapper__TIME__HH_arr,   sizeof(time_wrapper__TIME__HH_arr),   time_wrapper__get_val__HOUR());
      time_wrapper__set_arr__target(time_wrapper__TIME__MM_arr,   sizeof(time_wrapper__TIME__MM_arr),   time_wrapper__get_val__MIN());
      time_wrapper__set_arr__target(time_wrapper__TIME__SS_arr,   sizeof(time_wrapper__TIME__SS_arr),   time_wrapper__get_val__SEC());
    }
    time_wrapper__update_arr_DATE__YYYY_MM_DD();
    time_wrapper__update_arr_TIME__HH_MM_SS();
  }

  static int time_wrapper__get_val__YEAR_raw()  { return currentTime->tm_year + 1900; }
  static int time_wrapper__get_val__MONTH_raw() { return currentTime->tm_mon + 1; }
  static int time_wrapper__get_val__DAY_raw()   { return currentTime->tm_mday; }
  static int time_wrapper__get_val__HOUR_raw()  { return currentTime->tm_hour; }
  static int time_wrapper__get_val__MIN_raw()   { return currentTime->tm_min; }
  static int time_wrapper__get_val__SEC_raw()   { return currentTime->tm_sec; }


  time_t time_wrapper__get_UNIX()    { time_wrapper__update(); return current_time_UNIX; }
  int time_wrapper__get_val__YEAR()  { time_wrapper__update(); return time_wrapper__get_val__YEAR_raw(); }
  int time_wrapper__get_val__MONTH() { time_wrapper__update(); return time_wrapper__get_val__MONTH_raw(); }
  int time_wrapper__get_val__DAY()   { time_wrapper__update(); return time_wrapper__get_val__DAY_raw(); }
  int time_wrapper__get_val__HOUR()  { time_wrapper__update(); return time_wrapper__get_val__HOUR_raw(); }
  int time_wrapper__get_val__MIN()   { time_wrapper__update(); return time_wrapper__get_val__MIN_raw(); }
  int time_wrapper__get_val__SEC()   { time_wrapper__update(); return time_wrapper__get_val__SEC_raw(); }

  const char* time_wrapper__get_ptr__YEAR()           { time_wrapper__update_arr(); return time_wrapper__DATE__YYYY_arr; }       // [Y][Y][Y][Y][\0]
  const char* time_wrapper__get_ptr__MONTH()          { time_wrapper__update_arr(); return time_wrapper__DATE__MM_arr; }         // [M][M][\0]
  const char* time_wrapper__get_ptr__DAY()            { time_wrapper__update_arr(); return time_wrapper__DATE__DD_arr; }         // [D][D][\0]

  const char* time_wrapper__get_ptr__HOUR()           { time_wrapper__update_arr(); return time_wrapper__TIME__HH_arr; }         // [H][H][\0]
  const char* time_wrapper__get_ptr__MIN()            { time_wrapper__update_arr(); return time_wrapper__TIME__MM_arr; }         // [M][M][\0]
  const char* time_wrapper__get_ptr__SEC()            { time_wrapper__update_arr(); return time_wrapper__TIME__SS_arr; }         // [S][S][\0]

  const char* time_wrapper__get_ptr__YEAR_MONTH_DAY() { time_wrapper__update_arr(); return time_wrapper__DATE__YYYY_MM_DD_arr; } // [Y][Y][Y][Y][-][M][M][-][D][D][\0]
  const char* time_wrapper__get_ptr__HOUR_MIN_SEC()   { time_wrapper__update_arr(); return time_wrapper__TIME__HH_MM_SS_arr; }   // [H][H][:][M][M][:][S][S][\0]


// === тесты === 

  _Static_assert(sizeof(time_wrapper__DATE__YYYY_arr)       == 5,  "wrong buffer size for time_wrapper__DATE__YYYY_arr");
  _Static_assert(sizeof(time_wrapper__DATE__MM_arr)         == 3,  "wrong buffer size for time_wrapper__DATE__MM_arr");
  _Static_assert(sizeof(time_wrapper__DATE__DD_arr)         == 3,  "wrong buffer size for time_wrapper__DATE__DD_arr");
  _Static_assert(sizeof(time_wrapper__TIME__HH_arr)         == 3,  "wrong buffer size for time_wrapper__TIME__HH_arr");
  _Static_assert(sizeof(time_wrapper__TIME__MM_arr)         == 3,  "wrong buffer size for time_wrapper__TIME__MM_arr");
  _Static_assert(sizeof(time_wrapper__TIME__SS_arr)         == 3,  "wrong buffer size for time_wrapper__TIME__SS_arr");
  _Static_assert(sizeof(time_wrapper__DATE__YYYY_MM_DD_arr) == 11, "wrong buffer size for time_wrapper__DATE__YYYY_MM_DD_arr");
  _Static_assert(sizeof(time_wrapper__TIME__HH_MM_SS_arr)   == 9,  "wrong buffer size for time_wrapper__TIME__HH_MM_SS_arr");


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