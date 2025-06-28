  // 0_core
    #include "c_errors_test.h"
  // 1_libs
    #include "c_buffer_test.h"
  // 2_services
    //#include "c_errors_writer_test.h"

int main()
{
  printf("run_all_tests.c: init.\n\n");

  // 0_core
    c_errors_test();
  
  // 1_libs
    c_buffer_test();
 
  // 2_services
    //c_errors_writer_test();

  printf("run_all_tests: done.\n\n");
}