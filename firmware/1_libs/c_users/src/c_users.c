#include <stdio.h>                                                              // Для NULL
#include <string.h>                                                             // для strcmp
#include "c_users.h"
#include "c_errors_public.h"

static const UserRights users__user_rights_arr[USER_ID__AMOUNT] =               // массив с правами пользователей
{
  #define USERS_LIST(user_name, user_rights, user_tg_id) [user_name] = user_rights,   // X-MACROS
  #include "c_users_list.def"                                                   // в папке "/include"
  #undef  USERS_LIST
};

static char users__tg_user_id_arr[USER_ID__AMOUNT][USERS__USER_ID_MAX_LENGTH] =     // массив с айди пользователей (для гостевого пользователя пришлось сделать не const)
{
  #define USERS_LIST(user_name, user_rights, user_tg_id) [user_name] = user_tg_id,  // X-MACROS
  #include "c_users_list.def"                                                   // в папке "/include"
  #undef  USERS_LIST
};

  UserName users__get_user_name(const char* user_tg_id)
  {
    if(user_tg_id != NULL)
    {
      for(UserName id = USER_NAME__INVALID + 1; id < USER_ID__AMOUNT; id++)
      {
        if (strcmp(user_tg_id, users__tg_user_id_arr[id]) == 0)
        {
          return id;
        }
      }
    }
    err_raise_error(ERR_ID__USERS_WRONG_TOKEN);
    return USER_NAME__INVALID;
  }

  UserRights users__get_user_rights(UserName user_name)
  {
    if(user_name > USER_NAME__INVALID && user_name < USER_ID__AMOUNT)
    {
      return users__user_rights_arr[user_name];
    }
    err_raise_error(ERR_ID__USERS_WRONG_USER_ID);
    return USER_RIGHTS__NONE;
  }

  void users__guest_reset_id()
  {
    snprintf(users__tg_user_id_arr[USER_NAME__GUEST], sizeof(users__tg_user_id_arr[USER_NAME__GUEST]), "%s", USERS__TG_ID_INVALID);
  }

  bool users__guest_set_id(const char* user_tg_id)
  {
    if(0) //добавить проверку длины
    {
      snprintf(users__tg_user_id_arr[USER_NAME__GUEST], sizeof(users__tg_user_id_arr[USER_NAME__GUEST]), "%s", user_tg_id);
      return true;
    }
    return false;
  }

// тесты
  _Static_assert(sizeof(users__user_rights_arr)/sizeof(UserRights) == USER_ID__AMOUNT, "Mismatch between USER_ID__AMOUNT and users__user_rights_arr");

  _Static_assert(USER_RIGHTS__NONE   == 0,                 "USER_RIGHTS__NONE must be 0");
  _Static_assert(USER_RIGHTS__AMOUNT >  USER_RIGHTS__NONE, "USER_RIGHTS__AMOUNT must be defined correctly");
  _Static_assert(USER_ID__AMOUNT     >  USER_NAME__INVALID,  "USER_ID__AMOUNT must be defined correctly");

  _Static_assert(USERS__USER_ID_MAX_LENGTH >= 16,            "USERS__USER_ID_MAX_LENGTH is too small");