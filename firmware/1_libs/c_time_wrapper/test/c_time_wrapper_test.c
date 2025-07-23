#include <stdio.h>
#include <assert.h>
#include <string.h>                                                             // Для strcmp

#include "c_time_wrapper.h"

int main()
{
  // тесты значений
    assert(time_wrapper__get_UNIX()  >  1735765200);                            // 2 января 2025
    assert(time_wrapper__get_UNIX()  <  4891438800);                            // 2 января 2125

    assert(time_wrapper__get_val__YEAR()  >= 2025);
    assert(time_wrapper__get_val__YEAR()  < 2125);

    assert(time_wrapper__get_val__MONTH() >= 0);
    assert(time_wrapper__get_val__MONTH() <= 12);

    assert(time_wrapper__get_val__DAY()   >= 0);
    assert(time_wrapper__get_val__DAY()   <= 31);

    assert(time_wrapper__get_val__HOUR()  >= 0);  
    assert(time_wrapper__get_val__HOUR()  <= 23);

    assert(time_wrapper__get_val__MIN()   >= 0);
    assert(time_wrapper__get_val__MIN()   <= 59);

    assert(time_wrapper__get_val__SEC()   >= 0);
    assert(time_wrapper__get_val__SEC()   <= 60);                               // может быть 60 (для високосных секунд)

    printf("  PASS: values are correct!.\n");

  // NULL 
    assert(time_wrapper__get_ptr__YEAR() != NULL);
    assert(time_wrapper__get_ptr__MONTH() != NULL);
    assert(time_wrapper__get_ptr__DAY() != NULL);

    assert(time_wrapper__get_ptr__HOUR() != NULL);
    assert(time_wrapper__get_ptr__MIN() != NULL);
    assert(time_wrapper__get_ptr__SEC() != NULL);

    assert(time_wrapper__get_ptr__YEAR_MONTH_DAY() != NULL);
    assert(time_wrapper__get_ptr__HOUR_MIN_SEC() != NULL);

    printf("  PASS: > > NULL.\n");

  // вывод текущей даты и времени
    printf("   UTC: %ju\n", time_wrapper__get_UNIX());
    printf("   Date: %s\n", time_wrapper__get_ptr__YEAR_MONTH_DAY());
    printf("   Time: %s\n", time_wrapper__get_ptr__HOUR_MIN_SEC());
  return 0;
}