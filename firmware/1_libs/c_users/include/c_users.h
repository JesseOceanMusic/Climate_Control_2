#pragma once
  #include <stdbool.h>

  #define USER_TOKEN__INVALID "1234567890"

  typedef enum
  {
    USER_RIGHTS__NONE = 0,                                                      // ДОЛЖЕН БЫТЬ ПЕРВЫМ! // для возврата ошибки
    USER_RIGHTS__USER,
    USER_RIGHTS__ROOT,
    USER_RIGHTS__AMOUNT,
  } UserRights;

  typedef enum                                                                  // перечисление кодов ошибок
  {
    #define USERS_LIST(user_id, user_rights, tg_token) user_id,                 // X-MACROS
    #include "c_users_list.def"                                                 // в папке "/include"
    #undef  USERS_LIST
    USER_ID__AMOUNT,                                                            // ДОЛЖЕН БЫТЬ ПОСЛЕДНИМ! // Количество Пользователей
  }UserId;


  UserId     users__get_user_id    (char* tg_token);                            // вернёт USER_ID__DOES_NOT_EXIST, если пользователя с таким токеном не существует
  UserRights users__get_user_rights(UserId user_id);                            // вернёт USER_RIGHTS__NONE, если такого пользователя не существует