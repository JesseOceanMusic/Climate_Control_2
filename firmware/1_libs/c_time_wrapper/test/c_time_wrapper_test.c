#include <stdio.h>
#include <assert.h>
#include <string.h>                                                             // Для strcmp

#include "c_time_wrapper.h"

int main()
{
  time_t test_unix = time_wrapper__get_UNIX();
  int test_year    = time_wrapper__get_val__YEAR();
  int test_month   = time_wrapper__get_val__MONTH();
  int test_day     = time_wrapper__get_val__DAY();
  int test_hour    = time_wrapper__get_val__HOUR();
  int test_min     = time_wrapper__get_val__MIN();
  int test_sec     = time_wrapper__get_val__SEC();

  assert(test_unix  >  1735765200);  // 2 января 2025
  assert(test_year  >= 2025);

  assert(test_month >= 0);
  assert(test_month <= 12);

  assert(test_day   >= 0);
  assert(test_day   <= 31);

  assert(test_hour  >= 0);  
  assert(test_hour  <= 23);

  assert(test_min   >= 0);
  assert(test_min   <= 59);

  assert(test_sec   >= 0);
  assert(test_sec   <= 60);  // может быть 60 (для високосных секунд)

    printf("   UTC: %ju\n", time_wrapper__get_UNIX());
    printf("   Date: %s\n", time_wrapper__get_ptr__YEAR_MONTH_DAY());
    printf("   Time: %s\n", time_wrapper__get_ptr__HOUR_MIN_SEC());
  return 0;
}