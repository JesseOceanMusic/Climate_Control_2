#pragma once
  #include <stdbool.h>

  #define USERS__TG_ID__INVALID "111222333444"
  #define USERS__TG_ID__DEFAULT "0"
  #define USERS__TG_ID__MAX_LENGTH 16
  #define USERS__STATE__BAD -1
  #define USERS__STATE__DEFAULT 0

  #ifdef ENABLE_UNIT_TESTING_API
    #define USERS__TG_ID__TEST_USER "999888777666"
  #endif

  typedef enum
  {
    USERS__RIGHTS__NONE = 0,                                                    // ДОЛЖЕН БЫТЬ ПЕРВЫМ! // для возврата ошибки
    USERS__RIGHTS__USER,
    USERS__RIGHTS__ROOT,
    USERS__RIGHTS__AMOUNT,
  } UsersRights;

  typedef enum                                                                  // перечисление кодов ошибок
  {
    #define USERS_LIST(user_name, user_rights, user_tg_id) user_name,           // X-MACROS
    #include "c_users_list.def"                                                 // в папке "/include"
    #undef  USERS_LIST
    USERS__NAMES__AMOUNT,                                                       // ДОЛЖЕН БЫТЬ ПОСЛЕДНИМ! // Количество Пользователей
  }UsersNames;

  // узнать имя (индекс) пользователя по айди... нужна для обработки входящих запросов
    UsersNames  users__get_user_name(const char* const user_tg_id);             // вернёт USERS__NAMES__INVALID, если пользователя с таким токеном не существует

  // функции работы с пользователями
    UsersRights users__get_user_rights(UsersNames user_name);                   // узнать права пользователя // вернёт USERS__RIGHTS__NONE, если такого пользователя не существует
    int         users__get_cur_state  (UsersNames user_name);                   // получить текущее состояние пользователя // вернет -1, если такого пользователя существует
    bool        users__set_cur_state  (UsersNames user_name, int user_state);   // установить текущее состояние пользователя // вернет false, если такого пользователя существует \\ -1 ставить нельзя! зарезервирован под ошибку! 

  // настройки гостевого пользователя
    void users__guest_reset_id();                                               // сброс айди гостевого пользователя
    bool users__guest_set_id(const char* const user_tg_id);                     // добавить айди (добавить гостевого пользователя)