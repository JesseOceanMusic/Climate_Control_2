#include "c_buffer.h"

/// === ПРИМЕР ИСПОЛЬЗОВАНИЯ c_buffer === ///
  int main()
  {
    printf("\nbuffer content:%s", buf_get_arr_ptr(BUF_ID__TG_MSG));
    printf("\nis buffer empty? %d", buf_get_is_empty(BUF_ID__TG_MSG));

    buf_write_int(BUF_ID__TG_MSG, 123);
      printf("\n\nwriting int 123 to buffer...");
      printf("\nbuffer content: %s", buf_get_arr_ptr(BUF_ID__TG_MSG));
      printf("\nbuffer size left: %d", buf_get_size_left(BUF_ID__TG_MSG));
      printf("\nis buffer empty? %d", buf_get_is_empty(BUF_ID__TG_MSG));

    buf_write_char(BUF_ID__TG_MSG, "hello");
      printf("\n\nwriting string hello to buffer...");
      printf("\nbuffer content: %s", buf_get_arr_ptr(BUF_ID__TG_MSG));
      printf("\nbuffer size left: %d", buf_get_size_left(BUF_ID__TG_MSG));
      printf("\nis buffer empty? %d", buf_get_is_empty(BUF_ID__TG_MSG));

    buf_write_float(BUF_ID__TG_MSG, 1.234, 3);
      printf("\n\nwriting float 1.234 to buffer...");
      printf("\nbuffer content: %s", buf_get_arr_ptr(BUF_ID__TG_MSG));
      printf("\nbuffer size left: %d", buf_get_size_left(BUF_ID__TG_MSG));
      printf("\nis buffer empty? %d", buf_get_is_empty(BUF_ID__TG_MSG));
    
    buf_clear(BUF_ID__TG_MSG);
      printf("\n\nclear buffer...");
      printf("\nbuffer content: %s", buf_get_arr_ptr(BUF_ID__TG_MSG));
      printf("\nbuffer size left: %d", buf_get_size_left(BUF_ID__TG_MSG));
      printf("\nis buffer empty? %d", buf_get_is_empty(BUF_ID__TG_MSG));
    return 0;
  }