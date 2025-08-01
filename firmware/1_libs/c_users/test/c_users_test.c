#include <stdio.h>
#include <assert.h>
#include <string.h>                                                             // Для strcmp
#include "c_users.h"
#include "c_errors_public.h"

#define USERS__STATE__VALID 99
#define USERS__TG_ID__VALID     "112233445566"
#define USERS__NAMES__INVALID -1
#define USERS__TG_ID__IVALID_LENGTH "1234:1234:1234:1234:1234:1234:1234:1234"

int main()
{
  // правильность инициализации констант
    assert(strcmp(USERS__TG_ID__VALID, USERS__TG_ID__ERROR)       != 0);      // проверяем, что они все разные...
    assert(strcmp(USERS__TG_ID__VALID, USERS__TG_ID__TEST_USER)   != 0);      // проверяем, что они все разные...

    assert(strcmp(USERS__TG_ID__ERROR, USERS__TG_ID__TEST_USER)   != 0);      // проверяем, что они все разные...



    assert(sizeof(USERS__TG_ID__IVALID_LENGTH) > USERS__TG_ID__MAX_LENGTH);     // проверяем, что слишком длинный айди действительно длиннее максимально допустимого

    for(UsersNames userName = USERS__NAMES__ERROR + 1; userName < USERS__NAMES__AMOUNT; userName++)
    {
      assert(users__get_cur_state   (userName) == USERS__STATE__DEFAULT);
      assert(users__get_rights      (userName) != USERS__NAMES__ERROR);
      assert(strcmp(users__get_tg_id(userName), USERS__TG_ID__ERROR) != 0);
      assert(strcmp(users__get_tg_id(userName), USERS__TG_ID__VALID) != 0);
    }

  // correct input
    assert(users__get_user_name(USERS__TG_ID__TEST_USER) != USERS__NAMES__ERROR);
    const UsersNames userNameValid = users__get_user_name(USERS__TG_ID__TEST_USER);
    
    assert(users__get_rights(userNameValid) != USERS__RIGHTS__ERROR);
    assert(users__get_cur_state  (userNameValid) != USERS__STATE__BAD);
    assert(users__set_cur_state  (userNameValid, USERS__STATE__VALID) == true);
    assert(users__get_cur_state  (userNameValid) == USERS__STATE__VALID);

    assert(users__set_cur_state(userNameValid, USERS__STATE__DEFAULT) == true);   // сброс

  // guest correct input
    assert(users__guest_set_id (USERS__TG_ID__VALID) == true);
    assert(users__get_user_name(USERS__TG_ID__VALID) == USERS__NAMES__GUEST);
    users__guest_reset_id();

  // incorrect input
    // invalid tg_id
      assert(users__get_user_name(USERS__TG_ID__ERROR)   == USERS__NAMES__ERROR); // + 1  
      assert(users__get_user_name(USERS__TG_ID__VALID)   == USERS__NAMES__ERROR); // + 2  // уже сбросили гостевого пользователя, так что пользователя с таким айди уже нет

      assert(users__get_user_name(USERS__TG_ID__DEFAULT) == USERS__NAMES__ERROR); // + 3
      assert(users__get_user_name(NULL)                  == USERS__NAMES__ERROR); // + 4
      assert(err__get_total_counter(ERR__ID__USERS__WRONG_TG_ID) == 4);             // == 4
      err__reset_all();                                                             // reset

    // invalid user_name
      assert(users__get_rights(USERS__NAMES__INVALID) == USERS__RIGHTS__ERROR); // + 1
      assert(users__get_rights(USERS__NAMES__ERROR)   == USERS__RIGHTS__ERROR);   // + 2
      assert(err__get_total_counter(ERR__ID__USERS__WRONG_NAME)  == 2);           // == 2
      err__reset_all();                                                           // reset

    // invalid user_name
      assert(strcmp(users__get_tg_id(USERS__NAMES__INVALID), USERS__TG_ID__ERROR) == 0);  // + 1
      assert(strcmp(users__get_tg_id(USERS__NAMES__ERROR),   USERS__TG_ID__ERROR)   == 0);  // + 2
      assert(err__get_total_counter(ERR__ID__USERS__WRONG_NAME) == 2);                      // == 2
      err__reset_all();                                                                     // reset


    // invalid user_name
      assert(users__get_cur_state(USERS__NAMES__INVALID)       == USERS__STATE__BAD);    // + 1
      assert(users__get_cur_state(USERS__NAMES__ERROR)         == USERS__STATE__BAD);    // + 2
      assert(err__get_total_counter(ERR__ID__USERS__WRONG_NAME)  == 2);                    // == 2
      err__reset_all();                                                                    // reset

    // invalid user_name + valid state
      assert(users__set_cur_state(USERS__NAMES__INVALID, USERS__STATE__VALID) == false); // + 1
      assert(users__set_cur_state(USERS__NAMES__ERROR, USERS__STATE__VALID)   == false); // + 2
      assert(err__get_total_counter(ERR__ID__USERS__WRONG_NAME) == 2);                     // == 2
      err__reset_all();                                                                    // reset

    // valid user_name + invalid state
      assert(users__set_cur_state(userNameValid, USERS__STATE__BAD) == false);  // + 1
      assert(err__get_total_counter(ERR__ID__USERS__WRONG_STATE) == 1);         // == 1
      err__reset_all();                                                         // reset

    // wrong id
      assert(users__guest_set_id(USERS__TG_ID__IVALID_LENGTH) == false);        // + 1
      assert(users__guest_set_id(NULL) == false);                               // + 2
      assert(err__get_total_counter(ERR__ID__USERS__WRONG_TG_ID) == 2);         // == 2
      err__reset_all();                                                         // reset


    // проверяем, что стейты не поменялись во время теста
    for(UsersNames userName = USERS__NAMES__ERROR + 1; userName < USERS__NAMES__AMOUNT; userName++)
    {
      assert(users__get_cur_state(userName) == USERS__STATE__DEFAULT);
    }
  return 0;
}