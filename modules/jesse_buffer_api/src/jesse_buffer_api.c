#include "jesse_buffer_api.h"

static void check_write_result_for_errors(BufferStatus result)
{
  if (result == BUFFER_ERROR_NULL)
  {
    raise_error(ERROR_ID_BUFFER_NULL);
  }

  if (result == BUFFER_ERROR_OVERFILL)
  {
    raise_error(ERROR_ID_BUFFER_OVERFILLED);
  }    
}

void buffer_write_char_and_error_check(struct CharBuffer *struct_ptr, const char *data)
{
  BufferStatus result = buffer_write_char(struct_ptr, data);
  check_write_result_for_errors(result);
}

void buffer_write_int_and_error_check(struct CharBuffer *struct_ptr, int var)
{
  BufferStatus result = buffer_write_int(struct_ptr, var);
  check_write_result_for_errors(result);
}

void buffer_write_float_and_error_check(struct CharBuffer *struct_ptr, float var)
{
  BufferStatus result = buffer_write_float(struct_ptr, var);
  check_write_result_for_errors(result); 
}