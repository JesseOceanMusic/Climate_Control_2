#include <stdio.h>
#include <assert.h>
#include <string.h>                                                             // Для strcmp
#include "c_users.h"

int main()
{
  // correct input
    assert(users__get_user_rights(USER_ID__TG__ME) != 0);
  return 0;
}