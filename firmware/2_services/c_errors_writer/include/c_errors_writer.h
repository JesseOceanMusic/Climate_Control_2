#pragma once
  #include <stdbool.h>
  #include <string.h>                                                           // для strlen()
  #include "c_buffer.h"
  #include "c_errors.h"
  
  static const char ERR_HEADER_MESSAGE[] = "ID/counter/description:\n";         // Название столбцов для вывода ошибок

  bool err_writer(struct BufInfo *struct_ptr, ErrId errId, ErrType errType_filter, bool only_unhandled);