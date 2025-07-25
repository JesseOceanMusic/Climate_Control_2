#include <stdio.h>
#include <assert.h>
#include <string.h>                                                             // Для strcmp
#include "c_users.h"

int main()
{
  // correct input
    assert(users__get_user_rights(USERS__NAMES__GUEST) != USERS__RIGHTS__NONE);
  return 0;
}