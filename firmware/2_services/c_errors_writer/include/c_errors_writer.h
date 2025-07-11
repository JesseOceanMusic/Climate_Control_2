#pragma once
  #include <stdbool.h>
  #include "c_buffer.h"
  
  static const char ERR_HEADER_MESSAGE[] = "id/total counter/unhandled counter/description:\n";           // Название столбцов для вывода ошибок

  bool errwr__write_one_err(BufId bufId, ErrId errId, ErrType errType_filter, bool only_unhandled);  // записать в буфер(айди), ошибку(айди), если подойдет по фильтру(фильтр), только необработанные или все? (bool)