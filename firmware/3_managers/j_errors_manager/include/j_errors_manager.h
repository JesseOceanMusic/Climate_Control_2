#pragma once
  #include "j_buffer.h"
  #include "j_errors.h"
  #include <stdbool.h>

  /*
  ФУНКЦИИ
  ? 1. Хранит в себе массив для ошибок и управляет им
  2. Записывает лог всех ошибок в буффер и отправляет ежесуточно/по команде
  3. Ticker, который проверяет били ли ошибки в лупе и отправляет алерты если были
  ??? - возвращает true, если есть ошибки или сама отправляет в телегу?
  
  */
  void print_errors_log_to_buffer (struct CharBuffer *struct_ptr);
  bool tick_ERRORS_handler (struct CharBuffer *struct_ptr);