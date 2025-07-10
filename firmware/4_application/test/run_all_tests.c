  #include "stdio.h"

  // 0_core
    #include "c_errors_test.h"
  // 1_libs
    #include "c_buffer_test.h"
  // 2_services
    #include "c_errors_writer_test.h"

int main()
{
  printf("run_all_tests.c: init.\n\n");

  // 0_core
    printf("c_errors_test.c: init.\n");
    c_errors_test();
    printf("c_errors_test: done.\n\n");
  
  // 1_libs
    printf("c_buffer_test.c: init.\n");
    c_buffer_test();
    printf("c_buffer_test: done.\n\n");

  // 2_services
    printf("c_errors_writer_test.c: init.\n");
    c_errors_writer_test();
    printf("c_errors_writer_test: done.\n\n");

  printf("run_all_tests: done.\n\n");
}