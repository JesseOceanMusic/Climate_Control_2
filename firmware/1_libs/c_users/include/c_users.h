#pragma once
  #include <stdbool.h>

  #define USERS__TG_ID_INVALID "1234567890"
  #define USERS__USER_ID_MAX_LENGTH 16

  typedef enum
  {
    USER_RIGHTS__NONE = 0,                                                      // ДОЛЖЕН БЫТЬ ПЕРВЫМ! // для возврата ошибки
    USER_RIGHTS__USER,
    USER_RIGHTS__ROOT,
    USER_RIGHTS__AMOUNT,
  } UserRights;

  typedef enum                                                                  // перечисление кодов ошибок
  {
    #define USERS_LIST(user_name, user_rights, user_tg_id) user_name,                 // X-MACROS
    #include "c_users_list.def"                                                 // в папке "/include"
    #undef  USERS_LIST
    USER_ID__AMOUNT,                                                            // ДОЛЖЕН БЫТЬ ПОСЛЕДНИМ! // Количество Пользователей
  }UserName;


  UserName   users__get_user_name(const char* user_tg_id);                      // вернёт USER_ID__DOES_NOT_EXIST, если пользователя с таким токеном не существует
  UserRights users__get_user_rights(UserName user_name);                        // вернёт USER_RIGHTS__NONE, если такого пользователя не существует
  void       users__guest_reset_id();                                           // сброс айди гостевого пользователя
  bool       users__guest_set_id(const char* user_tg_id);                       // добавить айди (добавить гостевого пользователя)
