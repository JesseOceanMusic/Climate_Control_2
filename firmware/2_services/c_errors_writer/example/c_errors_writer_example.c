/*
 #include "c_errors_writer.h"
  #include <windows.h>                                                          // Для Sleep() // !!!ТОЛЬКО ДЛЯ ПРИМЕРА!!!

  int main()
  {
    while(1)
    {
      Sleep(2000);

      raise_error(ERR_ID__UNDEFINED);

      printf("\n\nНЕОБРАБОТАННЫЕ ОШИБКИ:\n\n");
      bool state = true;
      struct ErrWriterInfo ErrWriterInfo;
      while(state == true)
      {
        write_err_unhandled_errors_to_buffer(&ErrWriterInfo);
        //printf("%d", ErrWriterInfo.is_errors_writer_written_anything);
        printf("%s", ErrWriterInfo.arr_ptr);

        state = ErrWriterInfo.is_errors_writer_written_anything;
      }

      Sleep(2000);
      printf("\n\nВСЕ ОШИБКИ:\n\n");
      state = true;
      while(state == true)
      {
        write_all_errors_to_buffer(&ErrWriterInfo);
        //printf("%d", ErrWriterInfo.is_errors_writer_written_anything);
        printf("%s", ErrWriterInfo.arr_ptr);
        
        state = ErrWriterInfo.is_errors_writer_written_anything;
      }      
    }
    return 0;
  }
    */