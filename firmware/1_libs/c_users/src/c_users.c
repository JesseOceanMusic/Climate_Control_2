#include <stdio.h>                                                              // Для NULL
#include <string.h>                                                             // для strcmp
#include "c_users.h"
#include "c_errors_public.h"

static char users__tg_id_arr[USERS__NAMES__AMOUNT][USERS__TG_ID__MAX_LENGTH] =  // массив с айди пользователей (для гостевого пользователя пришлось сделать не const)
{
  #define USERS_LIST(user_name, user_rights, user_tg_id) [user_name] = user_tg_id,  // X-MACROS
  #include "c_users_list.def"                                                   // в папке "/include"
  #undef  USERS_LIST
};

static const UsersRights users__rights_arr[USERS__NAMES__AMOUNT] =              // массив с правами пользователей
{
  #define USERS_LIST(user_name, user_rights, user_tg_id) [user_name] = user_rights,   // X-MACROS
  #include "c_users_list.def"                                                   // в папке "/include"
  #undef  USERS_LIST
};

static int users__cur_state[USERS__NAMES__AMOUNT] = {USERS__STATE__DEFAULT};    // массив для хранения текущего состояния пользователя (обрабатываемой команды)

static bool users__check_user_name(UsersNames user_name)
{
  if(user_name > USERS__NAMES__ERROR && user_name < USERS__NAMES__AMOUNT)
  {
    return true;
  }
  err__raise_error(ERR__ID__USERS__WRONG_NAME);
  return false;
}

// узнать имя (индекс) пользователя по айди... нужна для обработки входящих запросов
  UsersNames users__get_user_name(const char* const user_tg_id)
  {
    if(user_tg_id != NULL && strcmp(user_tg_id, USERS__TG_ID__ERROR) != 0 && strcmp(user_tg_id, USERS__TG_ID__DEFAULT) != 0)
    {
      for(UsersNames id = USERS__NAMES__ERROR + 1; id < USERS__NAMES__AMOUNT; id++)
      {
        if(strcmp(user_tg_id, users__tg_id_arr[id]) == 0)
        {
          return id;
        }
      }
    }
    err__raise_error(ERR__ID__USERS__WRONG_TG_ID);
    return USERS__NAMES__ERROR;
  }

// функции работы с пользователями
  UsersRights users__get_rights(UsersNames user_name)
  {
    if(users__check_user_name(user_name) == true)
    {
      return users__rights_arr[user_name];
    }
    return USERS__RIGHTS__ERROR;
  }

  int users__get_cur_state  (UsersNames user_name)
  {
    if(users__check_user_name(user_name) == true)
    {
      return users__cur_state[user_name];
    }
    return USERS__STATE__BAD;
  }

  bool users__set_cur_state  (UsersNames user_name, int user_state)
  {
    if(users__check_user_name(user_name) == true)
    {
      if(user_state != USERS__STATE__BAD)
      {
        users__cur_state[user_name] = user_state;
        return true;
      }
      err__raise_error(ERR__ID__USERS__WRONG_STATE);
    }
    return false;
  }

void users__guest_reset_id()
{
  snprintf(users__tg_id_arr[USERS__NAMES__GUEST], sizeof(users__tg_id_arr[USERS__NAMES__GUEST]), "%s", USERS__TG_ID__ERROR);
}

bool users__guest_set_id(const char* const user_tg_id)
{
  if(user_tg_id != NULL && strlen(user_tg_id) < sizeof(users__tg_id_arr[USERS__NAMES__GUEST]))
  {
    snprintf(users__tg_id_arr[USERS__NAMES__GUEST], sizeof(users__tg_id_arr[USERS__NAMES__GUEST]), "%s", user_tg_id);
    return true;
  }
  err__raise_error(ERR__ID__USERS__WRONG_TG_ID);
  return false;
}

const char* users__get_tg_id(UsersNames user_name)
{
  if(users__check_user_name(user_name) == true)
  {
    return users__tg_id_arr[user_name];
  }
  return USERS__TG_ID__ERROR;
}

// тесты
  _Static_assert(sizeof(users__cur_state)/sizeof(int)                 == USERS__NAMES__AMOUNT,     "Mismatch between USERS__NAMES__AMOUNT and users__rights_arr");
  _Static_assert(sizeof(users__rights_arr)/sizeof(UsersRights)        == USERS__NAMES__AMOUNT,     "Mismatch between USERS__NAMES__AMOUNT and users__rights_arr");
  _Static_assert(sizeof(users__tg_id_arr)/sizeof(users__tg_id_arr[0]) == USERS__NAMES__AMOUNT,     "Mismatch between USERS__NAMES__AMOUNT and users__tg_id_arr row count");
  _Static_assert(sizeof(users__tg_id_arr[0])                          == USERS__TG_ID__MAX_LENGTH, "Mismatch between USERS__TG_ID__MAX_LENGTH and users__tg_id_arr column size");

  _Static_assert(USERS__RIGHTS__ERROR     == 0,                     "USERS__RIGHTS__ERROR must be 0");
  _Static_assert(USERS__RIGHTS__AMOUNT   >  USERS__RIGHTS__ERROR,   "USERS__RIGHTS__AMOUNT must be defined correctly");

  _Static_assert(USERS__NAMES__ERROR   == 0,                     "USERS__NAMES__ERROR must be 0");
  _Static_assert(USERS__NAMES__AMOUNT    >  USERS__NAMES__ERROR, "USERS__NAMES__AMOUNT must be defined correctly");

  _Static_assert(USERS__TG_ID__MAX_LENGTH      >= 16,                      "USERS__TG_ID__MAX_LENGTH is too small");
  _Static_assert(sizeof(USERS__TG_ID__ERROR) < USERS__TG_ID__MAX_LENGTH, "USERS__TG_ID__ERROR is too long");