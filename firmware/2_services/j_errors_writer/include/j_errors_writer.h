#pragma once
  #include "j_buffer.h"
  #include "j_errors.h"
  #include <stdbool.h>
  #include <string.h>                                                           // для strlen()

  #define BUFFER_FOR_ERRORS_SIZE 128
  
  typedef enum
  {
    ERR_WRITER__CUR_TASK__NONE,
    ERR_WRITER__CUR_TASK__WRITE_ALL_ERRORS,
    ERR_WRITER__CUR_TASK__WRITE_UNHANDLED_ERRORS,
    ERR_WRITER__CUR_TASK__COMPLETE,
    ERR_WRITER__CUR_TASK__ERROR,
  } ErrWriterState;

  typedef struct
  {
    char *arr_ptr;
    ErrWriterState state;
    ErrorsTypes type_filter;
  } ErrBufferInfo;

  ErrBufferInfo err_writer__write_next_part();  
  ErrBufferInfo err_writer__set_task__all_errors(ErrorsTypes handle_this_type);
  ErrBufferInfo err_writer__set_task__unhandled_errors(ErrorsTypes handle_this_type);
  ErrBufferInfo err_writer__get_status();