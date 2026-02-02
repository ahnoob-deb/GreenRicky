#ifndef M_GAME_MAIN_H
#define M_GAME_MAIN_H 1

#define POI_IS_FREE 0
#define POI_IS_BLOCKED 1

#define MAX_LEN_EXTRA_TEXT 20
#define FOUR_LINES_GRATS "SUPERB"
#define TEXT_GAME_OVER "GAME OVER"
#define TEXT_BREAK "BREAK"

/* game statistics */
typedef struct
{
    unsigned int score;
    unsigned int count_pieces_landed;
    unsigned int rows_destroyed;
    unsigned int level;
} GameStats_t;


/* booting the core game at startup,
 * this means, loading data etc.
 */
unsigned int cg_run(void);
int cg_get_game_state(void);

#endif
