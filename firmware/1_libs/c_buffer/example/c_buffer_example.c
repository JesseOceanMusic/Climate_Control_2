#include "c_buffer.h"

/// === ПРИМЕР ИСПОЛЬЗОВАНИЯ c_buffer === ///
  int main()
  {
    printf("\nbuffer content:%s", buf__get_arr_ptr(BUF__ID__TG_MSG));
    printf("\nis buffer empty? %d", buf__get_is_empty(BUF__ID__TG_MSG));

    buf__write_int(BUF__ID__TG_MSG, 123);
      printf("\n\nwriting int 123 to buffer...");
      printf("\nbuffer content: %s", buf__get_arr_ptr(BUF__ID__TG_MSG));
      printf("\nbuffer size left: %d", buf__get_size_left(BUF__ID__TG_MSG));
      printf("\nis buffer empty? %d", buf__get_is_empty(BUF__ID__TG_MSG));

    buf__write_char(BUF__ID__TG_MSG, "hello");
      printf("\n\nwriting string hello to buffer...");
      printf("\nbuffer content: %s", buf__get_arr_ptr(BUF__ID__TG_MSG));
      printf("\nbuffer size left: %d", buf__get_size_left(BUF__ID__TG_MSG));
      printf("\nis buffer empty? %d", buf__get_is_empty(BUF__ID__TG_MSG));

    buf__write_float(BUF__ID__TG_MSG, 1.234, 3);
      printf("\n\nwriting float 1.234 to buffer...");
      printf("\nbuffer content: %s", buf__get_arr_ptr(BUF__ID__TG_MSG));
      printf("\nbuffer size left: %d", buf__get_size_left(BUF__ID__TG_MSG));
      printf("\nis buffer empty? %d", buf__get_is_empty(BUF__ID__TG_MSG));
    
    buf__clear(BUF__ID__TG_MSG);
      printf("\n\nclear buffer...");
      printf("\nbuffer content: %s", buf__get_arr_ptr(BUF__ID__TG_MSG));
      printf("\nbuffer size left: %d", buf__get_size_left(BUF__ID__TG_MSG));
      printf("\nis buffer empty? %d", buf__get_is_empty(BUF__ID__TG_MSG));
    return 0;
  }