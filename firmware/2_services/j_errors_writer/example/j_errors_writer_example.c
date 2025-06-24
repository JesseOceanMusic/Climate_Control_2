  #include "j_errors_writer.h"
  #include <windows.h>                                                          // Для Sleep() // !!!ТОЛЬКО ДЛЯ ПРИМЕРА!!!

  int main()
  {
    while(1)
    {
      Sleep(2000);

      raise_error(ERR_ID__UNDEFINED);

      printf("\n\nНЕОБРАБОТАННЫЕ ОШИБКИ:\n\n");
      bool state = true;
      struct ErrorsBufferInfo errorsBufferInfo;
      while(state == true)
      {
        write_unhandled_errors_to_buffer(&errorsBufferInfo);
        //printf("%d", errorsBufferInfo.is_errors_writer_written_anything);
        printf("%s", errorsBufferInfo.arr_ptr);

        state = errorsBufferInfo.is_errors_writer_written_anything;
      }

      Sleep(2000);
      printf("\n\nВСЕ ОШИБКИ:\n\n");
      state = true;
      while(state == true)
      {
        write_all_errors_to_buffer(&errorsBufferInfo);
        //printf("%d", errorsBufferInfo.is_errors_writer_written_anything);
        printf("%s", errorsBufferInfo.arr_ptr);
        
        state = errorsBufferInfo.is_errors_writer_written_anything;
      }      
    }
    return 0;
  }