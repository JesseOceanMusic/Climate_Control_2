#include <stdio.h>
#include <assert.h>
#include <string.h>                                                             // Для strcmp

#include "c_time_wrapper.h"

int main()
{
  time_t test_unix = time_wrapper__get_UNIX();
  int test_year    = time_wrapper__get_YEAR();
  int test_month   = time_wrapper__get_MONTH();
  int test_day     = time_wrapper__get_DAY();
  int test_hour    = time_wrapper__get_HOUR();
  int test_min     = time_wrapper__get_MIN();
  int test_sec     = time_wrapper__get_SEC();

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

  printf("== WITHOUT 0 ==\n");
    printf("   UTC: %ju\n", time_wrapper__get_UNIX());
    printf("   Date: %d-%d-%d\n", time_wrapper__get_YEAR(), time_wrapper__get_MONTH(), time_wrapper__get_DAY());
    printf("   Time: %d:%d:%d\n", time_wrapper__get_HOUR(), time_wrapper__get_MIN(), time_wrapper__get_SEC());

  printf("== WITH 0 ==\n");
    printf("   UTC: %ju\n", test_unix);
    printf("   Date:");
    printf("%d-", test_year);

    if(test_month < 10) { printf("%d", 0); }
    printf("%d-", test_month);

    if(test_day < 10) { printf("%d", 0); }
    printf("%d\n", test_day);

    printf("   Time:");
    if(test_hour < 10) { printf("%d", 0); }
    printf("%d:", test_hour);

    if(test_min < 10) { printf("%d", 0); }
    printf("%d:", test_min);

    if(test_sec < 10) { printf("%d", 0); }
    printf("%d\n", test_sec);

  return 0;
}