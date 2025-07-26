#include <stdio.h>
#include <assert.h>
#include <string.h>                                                             // Для strcmp
#include "c_users.h"
#include "c_errors_public.h"

#define USERS__STATE__VALID 99
#define USERS__TG_ID__VALID     "112233445566"
#define USERS__TG_ID__NOT_EXIST "123123123123"
#define USERS__NAMES__NOT_EXIST -1
#define USERS__TG_ID__IVALID_LENGTH "1234:1234:1234:1234:1234:1234:1234:1234"

int main()
{
  // правильность инициализации констант
    assert(strcmp(USERS__TG_ID__TEST_USER, USERS__TG_ID__INVALID) != 0);        // проверяем, что они все разные...
    assert(strcmp(USERS__TG_ID__TEST_USER, USERS__TG_ID__VALID)   != 0);        // проверяем, что они все разные...
    assert(strcmp(USERS__TG_ID__NOT_EXIST, USERS__TG_ID__VALID)   != 0);        // проверяем, что они все разные...

    assert(sizeof(USERS__TG_ID__IVALID_LENGTH) > USERS__TG_ID__MAX_LENGTH);     // проверяем, что слишком длинный айди действительно длиннее максимально допустимого

    for(UsersNames userName = USERS__NAMES__INVALID + 1; userName < USERS__NAMES__AMOUNT; userName++)
    {
      assert(users__get_cur_state(userName) == USERS__STATE__DEFAULT);
    }

  // correct input
    assert(users__get_user_name(USERS__TG_ID__TEST_USER) != USERS__NAMES__INVALID);
    const UsersNames userNameValid = users__get_user_name(USERS__TG_ID__TEST_USER);
    
    assert(users__get_user_rights(userNameValid) != USERS__RIGHTS__NONE);
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
      assert(users__get_user_name(USERS__TG_ID__INVALID)         == USERS__NAMES__INVALID); // + 1  
      assert(users__get_user_name(USERS__TG_ID__NOT_EXIST)       == USERS__NAMES__INVALID); // + 2
      assert(users__get_user_name(USERS__TG_ID__DEFAULT)         == USERS__NAMES__INVALID); // + 3
      assert(users__get_user_name(NULL)                          == USERS__NAMES__INVALID); // + 4
      assert(err__get_total_counter(ERR__ID__USERS__WRONG_TG_ID) == 4);                     // == 4
      err__reset_all();                                                                     // reset

    // invalid user_name
      assert(users__get_user_rights(USERS__NAMES__NOT_EXIST)     == USERS__RIGHTS__NONE);  // + 1
      assert(users__get_user_rights(USERS__NAMES__INVALID)       == USERS__RIGHTS__NONE);  // + 2
      assert(err__get_total_counter(ERR__ID__USERS__WRONG_NAME)  == 2);                    // == 2
      err__reset_all();                                                                    // reset

    // invalid user_name
      assert(users__get_cur_state(USERS__NAMES__NOT_EXIST)       == USERS__STATE__BAD);    // + 1
      assert(users__get_cur_state(USERS__NAMES__INVALID)         == USERS__STATE__BAD);    // + 2
      assert(err__get_total_counter(ERR__ID__USERS__WRONG_NAME)  == 2);                    // == 2
      err__reset_all();                                                                    // reset

    // invalid user_name + valid state
      assert(users__set_cur_state(USERS__NAMES__NOT_EXIST, USERS__STATE__VALID) == false); // + 1
      assert(users__set_cur_state(USERS__NAMES__INVALID, USERS__STATE__VALID)   == false); // + 2
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
    for(UsersNames userName = USERS__NAMES__INVALID + 1; userName < USERS__NAMES__AMOUNT; userName++)
    {
      assert(users__get_cur_state(userName) == USERS__STATE__DEFAULT);
    }
  return 0;
}