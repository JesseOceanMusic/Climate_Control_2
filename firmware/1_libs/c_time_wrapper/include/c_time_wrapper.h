#pragma once
  #include <time.h>
  #include <stdbool.h>
  #include "c_time_wrapper.h"

  time_t time_wrapper__get_UNIX();
  int time_wrapper__get_YEAR();
  int time_wrapper__get_MONTH();
  int time_wrapper__get_DAY();
  int time_wrapper__get_HOUR();
  int time_wrapper__get_MIN();
  int time_wrapper__get_SEC();
  
// Это библиотека только для форматирование времени, которое она запрашивает из системы.
// Временная зона и синхронизация времени должна происходить отдельно, через NTP сервер.