#ifndef COMMON_H
#define COMMON_H 1

/* Project name and version information */
#define WORKINGNAME "Green Ricky"
#define VERSION "0.8.0"

/* Main programmers name and gitHub information */
#define AUTHOR "oldschool-noob"
#define AUTHOR_GITHUB_HANDLE "ahnoob-deb"
/* TODO: Add email address here */
#define AUTHOR_EMAIL "TODO"
#define GITHUB_REPO "TODO"

/* boolean values - C89 does not know stdbool.h */
#define TRUE 1
#define FALSE 0

/* DEBUG LEVELS
 0 - No debug output
 1 - Basic debug output (initialization, main events)
 2 - Verbose debug output (detailed function calls, variable states)
 3 - Very verbose (all possible debug info, for deep troubleshooting)
 */
#define DEBUG_VERY_VERBOSE 3
#define DEBUG_VERBOSE 2
#define DEBUG_NORMAL 1
#define DEBUG_NONE 0

/* Chosen Debug Level */
#define DEBUG_LEVEL DEBUG_VERBOSE

/* Dimensions of the Cetris-Window */
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define TARGET_FPS 60


#define FILE_HOF "../data/hof/hof.txt"

#define BLOCK_SIZE_PX 20

#define DRAW_START_X 291
#define DRAW_START_Y 163

#define NEXT_PIECE_X 85
#define NEXT_PIECE_Y 250

/* where is no bricket... > FREE = bricket */
#define FREE 0
/* for now, used for marking imploding lines in core game map */
#define IMPLODING -1

/* color of core game map, (0-5) */
#define MAP_COLOR 0

/* Dimensions, in blocks, of the game map */
#define MAP_WIDTH 10
#define MAP_HEIGHT 20

#define SCORE_PER_PARK 1
#define SCORE_PER_LINE 10
#define SCORE_FAC_FOUR_LINES 4

/* Dimensions of the game Pieces */
/* width , in blocks, of a Piece */
#define PIECE_WIDTH 7
/* height , in blocks, of a Piece */
#define PIECE_HEIGHT 7

#define SPAWN_X 4
#define SPAWN_Y 0

/* Collision Bit masks */
/* a collision below, bit 2 is set */
#define COLL_BELOW 4
/* collision left, bit 0 is set. */
#define COLL_LEFT 1
/* collision right, bit 1 is set */
#define COLL_RIGHT 2
/* collision left+right+below, bit 0,1,2 are set (=7)*/
#define COLL_ALL 7

/* Directions for shape matrix */
#define COUNT_DIRECTIONS 4

/* The number of different shapes of the Pieces */
#define COUNT_SHAPES 7

/* 5 MyGameColors are defined for the Pieces (for now). */
#define COUNT_PIECE_COLORS 8

/* 5 MyGameColors are defined for the map (for now). */
#define COUNT_MAP_COLORS 6

/* what is one second? */
#define SEC_UNIT 1000
/* The time span, where you can do a last move when
 hitting the ground.
 */
#define LAST_MOVE_WAIT 5000

/* Lines implode after this time */
#define IMPLODING_TIME 1200

#define LIMIT_FPS TRUE

/* where the playground starts
 #define GAMEAREA_STARTX 65
 #define GAMEAREA_STARTY 10 */

/* Exit key is ESCAPE key (27 in ASCII) */
#define K_EXIT K_ESCAPE

/* Piece falling with 1.5 Pixels per Second */
#define PIECE_FALL_PER_SECOND 3.0f

/* The initial move-factor of a falling peace, following the factor when down-key is
 pressed. */
#define NORM_MOVE_FACTOR 1
#define MAX_MOVE_FACTOR 8

/* Delay in milliseconds for waiting after Game Over */
#define GAME_OVER_DELAY 3000

#define MAX_FULL_LINES 4
#define NO_LINE -1

/* Events */
#define EV_INSTANT_QUIT -1
#define EV_NO_EVENT -25

/* Main Game States */
#define ST_BREAK 0
#define ST_MAIN_MENU 1
#define ST_CORE_GAME 2
#define ST_HALL_OF_FAME 3
#define ST_EXIT 4

#define HOF_COLOR 0x30d556
#define HOF_CSCORE_COLOR 0xe37a56


#define START_SCORE 333

#endif
