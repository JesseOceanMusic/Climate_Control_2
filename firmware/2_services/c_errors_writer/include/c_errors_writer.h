#pragma once
  #include <stdbool.h>
  #include <string.h>                                                           // для strlen()
  #include "c_buffer.h"
  #include "c_errors.h"

  #define BUFFER_FOR_ERRORS_SIZE 128  // не должно быть больше unsigned short. если нужно больше, то менять arr_size в ErrWriterInfo()
  


  /*
    1.         manager -> c_errors.h has_unhandled_errors();
    2. if true manager -> c_errors_writer.h запиши ошибки
                          1. куда записать?
                          2. что записать?
  */

  bool err_writer(struct BufInfo *struct_ptr, ErrId errId, ErrType errType_filter, bool only_unhandled);


  typedef enum 
  {
    ERR_WRITER__CUR_TASK__NONE,
    ERR_WRITER__CUR_TASK__WRITE_ALL_ERRORS,
    ERR_WRITER__CUR_TASK__WRITE_UNHANDLED_ERRORS,
    ERR_WRITER__CUR_TASK__COMPLETE,
    ERR_WRITER__CUR_TASK__ERROR,
  } ErrWriterState;

  struct ErrWriterInfo
  {
    char *arr_ptr;
    unsigned short msg_length;
    ErrWriterState state;
    ErrType type_filter;
  };

  struct ErrWriterInfo err_writer__get_info();
  struct ErrWriterInfo err_writer__set_task__all_errors      (ErrType handle_this_type);
  struct ErrWriterInfo err_writer__set_task__unhandled_errors(ErrType handle_this_type);
  struct ErrWriterInfo err_writer__write_next_part();


  // тесты
     _Static_assert(BUFFER_FOR_ERRORS_SIZE < ERR_USHORT_MAX, "BUFFER_FOR_ERRORS_SIZE: value exceed unsigned short");