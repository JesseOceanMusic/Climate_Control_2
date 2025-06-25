#include "j_errors_test.h"
//#include "j_buffer_test.h"
//#include "j_errors_writer_test.h"

int main()
{
  printf("run_all_tests.c: ЗАПУСК ТЕСТИРОВАНИЯ\n\n");

  // 0_core
    j_errors_test();
  
  // 1_libs
    //j_buffer_test();

  // 2_services
    //j_errors_writer_test();

  printf("run_all_tests: ВСЕ ТЕСТЫ УСПЕШНО ПРОЙДЕНЫ!\n\n");
}