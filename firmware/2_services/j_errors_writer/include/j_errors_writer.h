#pragma once
  #include "j_buffer.h"
  #include "j_errors.h"
  #include <stdbool.h>
  #include <string.h>                                                           // для strlen()

  #define BUFFER_FOR_ERRORS_SIZE 128
  
  enum
  {
    ERRORS_WRITER__CUR_TASK__NONE,
    ERRORS_WRITER__CUR_TASK__WRITE_ALL_ERRORS,
    ERRORS_WRITER__CUR_TASK__WRITE_UNHANDLED_ERRORS,
    ERRORS_WRITER__CUR_TASK__COMPLETE,
    ERRORS_WRITER__CUR_TASK__ERROR,
  };

  struct ErrorsBufferInfo
  {
    char *arr_ptr;
    char state;
  };

  void errors_writer__write_next_part(struct ErrorsBufferInfo *errorsBufferInfo);  
  void errors_writer__set_task__all_errors(ErrorsTypes handle_this_type);
  void errors_writer__set_task__unhandled_errors(ErrorsTypes handle_this_type);
