#pragma once
  #include <time.h>

  // === геттеры по значению ===
    time_t time_wrapper__get_UNIX();                                            // количество секунд с 1 января 1970 года
    int    time_wrapper__get_val__YEAR();
    int    time_wrapper__get_val__MONTH();
    int    time_wrapper__get_val__DAY();
    int    time_wrapper__get_val__HOUR();
    int    time_wrapper__get_val__MIN();
    int    time_wrapper__get_val__SEC();


  // === геттеры указателя на массив из char ===
    const char* time_wrapper__get_ptr__YEAR();                                  // [Y][Y][Y][Y][\0]
    const char* time_wrapper__get_ptr__MONTH();                                 // [M][M][\0]
    const char* time_wrapper__get_ptr__DAY();                                   // [D][D][\0]
    const char* time_wrapper__get_ptr__HOUR();                                  // [H][H][\0]
    const char* time_wrapper__get_ptr__MIN();                                   // [M][M][\0]
    const char* time_wrapper__get_ptr__SEC();                                   // [S][S][\0]

    const char* time_wrapper__get_ptr__YEAR_MONTH_DAY();                        // [Y][Y][Y][Y][-][M][M][-][D][D][\0]
    const char* time_wrapper__get_ptr__HOUR_MIN_SEC();                          // [H][H][:][M][M][:][S][S][\0]

// !!! Это библиотека только для форматирование времени, которое она запрашивает из системы.
// !!! Временная зона и синхронизация времени должна происходить отдельно, через NTP сервер.

// В чем разница геттеров по значению и по указателю на char?
// ...get_val__DAY() --> вернёт int со значением    "7".  если сделать printf - вывод "7".
// ...get_ptr__DAY() --> вернёт указатель на строку "07". если сделать printf - вывод "07".