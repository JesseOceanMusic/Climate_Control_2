#include <stddef.h>                                                             // Для NULL
#include <string.h>                                                             // для strcmp
#include "c_users.h"
#include "c_errors_public.h"

static const UserRights users__tg_user_rights_arr[USER_ID__AMOUNT] =            // массив с правами пользователей
{
  #define USERS_LIST(user_id, user_rights, tg_token) [user_id] = user_rights,   // X-MACROS
  #include "c_users_list.def"                                                   // в папке "/include"
  #undef  USERS_LIST
};

static const char* users__tg_tokens_arr[USER_ID__AMOUNT] =                      // массив с токенами пользователей
{
  #define USERS_LIST(user_id, user_rights, tg_token) [user_id] = tg_token,      // X-MACROS
  #include "c_users_list.def"                                                   // в папке "/include"
  #undef  USERS_LIST
};

  UserId users__get_user_id(char* tg_token)
  {
    if(tg_token != NULL)
    {
      for(UserId id = USER_ID__INVALID + 1; id < USER_ID__AMOUNT; id++)
      {
        if (strcmp(tg_token, users__tg_tokens_arr[id]) == 0)
        {
          return id;
        }
      }
    }
    err_raise_error(ERR_ID__USERS_WRONG_TOKEN);
    return USER_ID__INVALID;
  }

  UserRights users__get_user_rights(UserId user_id)
  {
    if(user_id > USER_ID__INVALID && user_id < USER_ID__AMOUNT)
    {
      return users__tg_user_rights_arr[user_id];
    }
    err_raise_error(ERR_ID__USERS_WRONG_USER_ID);
    return USER_RIGHTS__NONE;
  }


// тесты
  _Static_assert(sizeof(users__tg_user_rights_arr)/sizeof(UserRights) == USER_ID__AMOUNT, "Mismatch between USER_ID__AMOUNT and users__tg_user_rights_arr");
  _Static_assert(sizeof(users__tg_tokens_arr)/sizeof(const char*)     == USER_ID__AMOUNT, "Mismatch between USER_ID__AMOUNT and users__tg_tokens_arr");

  _Static_assert(USER_RIGHTS__NONE   == 0,                 "USER_RIGHTS__NONE must be 0");
  _Static_assert(USER_RIGHTS__AMOUNT >  USER_RIGHTS__NONE, "USER_RIGHTS__AMOUNT must be defined correctly");
  _Static_assert(USER_ID__AMOUNT     >  USER_ID__INVALID,  "USER_ID__AMOUNT must be defined correctly");