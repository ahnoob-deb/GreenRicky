#ifndef M_GAME_MAIN_MENU_H
#define M_GAME_MAIN_MENU_H 1

#define EV_NEW_GAME 1
#define EV_OPTIONS 2
#define EV_HALL_OF_FAME 3

#define STR_HEADLINE "GREEN RICKY"
#define STR_NEW_GAME "NEW GAME"
#define STR_OPTIONS "OPTIONS"
#define STR_HALL_OF_FAME "HALL OF FAME"

#define MM_DOWN 1
#define MM_UP -1

#define MM_COUNT_OPTIONS     4

#define MM_OPT_NEW_GAME      0
#define MM_OPT_OPTIONS       1
#define MM_OPT_HALL_OF_FAME  2
#define MM_OPT_EXIT          3

#define OPTION_ACTIVATED 10
#define OPTION_DEACTIVATED 1

void mm_run(void);
int mm_get_game_state(void);

#endif
