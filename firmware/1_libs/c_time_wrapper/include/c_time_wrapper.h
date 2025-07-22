#pragma once
  #include <time.h>
  #include <stdbool.h>
  #include "c_time_wrapper.h"


  time_t      time_wrapper__get_UNIX();

  int         time_wrapper__get_val__YEAR();
  int         time_wrapper__get_val__MONTH();
  int         time_wrapper__get_val__DAY();
  int         time_wrapper__get_val__HOUR();
  int         time_wrapper__get_val__MIN();
  int         time_wrapper__get_val__SEC();

  const char* time_wrapper__get_ptr__YEAR();                                    // [Y][Y][Y][Y][\0]
  const char* time_wrapper__get_ptr__MONTH();                                   // [M][M][\0]
  const char* time_wrapper__get_ptr__DAY();                                     // [D][D][\0]
  const char* time_wrapper__get_ptr__HOUR();                                    // [H][H][\0]
  const char* time_wrapper__get_ptr__MIN();                                     // [M][M][\0]
  const char* time_wrapper__get_ptr__SEC();                                     // [S][S][\0]

  const char* time_wrapper__get_ptr__YEAR_MONTH_DAY();                          // [Y][Y][Y][Y][-][M][M][-][D][D][\0]
  const char* time_wrapper__get_ptr__HOUR_MIN_SEC();                            // [H][H][:][M][M][:][S][S][\0]

// Это библиотека только для форматирование времени, которое она запрашивает из системы.
// Временная зона и синхронизация времени должна происходить отдельно, через NTP сервер.