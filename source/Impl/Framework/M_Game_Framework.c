/**************************************************************************
 * This is the Framework of the program.
 *
 * It initializes all data and  combines all the
 * following modules to one single program :
 *
 * M_Game_Framework.c          // Framework
 *     S_FPS_Counter.c         // Service "FPS-Counter"
 *     S_Quit.c                // Service "Quit" - it quit's the program in a clean way
 *     M_Game_Main_Menu.c      // Module Main Menu
 *     M_Game_Main.c           // Module of the Core Game
 *     M_Game_Hall_of_Fame.c   // Module for the Highscore-List
 *     S_Game_Objects.c        // Service "Game-Objects" which delivers functions for
 *                             // manipulation of game objects (structures)
 *     S_SDL3_Rendering.c      // Service "SDL3" - a wrapper-module for some functions
 *                             // needing SDL3-API
 *     S_Shape_Manager.c       // Service "Shape-Manager" which organizes all of
 *                             // the Pieces shapes used in the game
 *     S_Texture_Manager.c     // Service "Texture-Manager" which organizes all the
 *                             // the textures used in the game
 *
 ******************************************************************************/

#include "M_Game_Framework.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "D_Common.h"
#include "M_Game_Hall_of_Fame.h"
#include "M_Game_Main.h"
#include "M_Game_Main_Menu.h"
#include "S_Game_Objects.h"
#include "S_Quit.h"
#include "S_SDL3_Rendering.h"
#include "S_Shape_Manager.h"
#include "S_Texture_Manager.h"
#include "S_FPS_Counter.h"

#include <libgen.h>

unsigned short mg_exit_flag; // wether the user wants to quit the game
unsigned short mg_flag_fps; // wether the fps counter is turned on
int mg_main_game_state;  // indicates the status of the game

/***************************************************/
/* BOOTING                                         */
/***************************************************/

static int mg_boot(void);  // boots the SDL-Layer
static int mg_init(void);  // initializes the game
static int mg_load_textures(void); /* add all needed Textures
                                      to a tree of MyGameTextures */

static void mg_quit(void); // quit's the game

/**************************************************************************
 *
 * Load the Textures for the game.
 *
 **************************************************************************/
static int mg_load_textures() {
	// FONTS :

	mt_load_texture(FILE_FONT_DATA_01, HOOK_FONT_DATA_01, NULL);

	if (mt_search_texture(HOOK_FONT_DATA_01) == NULL) {
		printf("could not load fonts!\n");
		quit_game_with_log_error("data management error.\n", 1);
	}

	// MAIN MENU :

	mt_load_texture(FILE_MMENU_SCREEN_MASK_OFF, HOOK_MMENU_SCREEN_MASK_OFF,
	NULL);
	mt_load_texture(FILE_MMENU_SCREEN_MASK_ON, HOOK_MMENU_SCREEN_MASK_ON, NULL);

	if ((mt_search_texture(HOOK_MMENU_SCREEN_MASK_OFF) == NULL)
			|| (mt_search_texture(HOOK_MMENU_SCREEN_MASK_ON) == NULL)) {
		printf("could not load textures for menu!\n");
		quit_game_with_log_error("data management error.\n", 1);
	}

	// HALL OF FAME :

	mt_load_texture(FILE_HOF_SCREEN_MASK, HOOK_HOF_SCREEN_MASK, NULL);

	if (mt_search_texture(HOOK_HOF_SCREEN_MASK) == NULL) {
		printf("could not load textures for hall of fame!\n");
		quit_game_with_log_error("data management error.\n", 1);
	}

	// INGAME :

	mt_load_texture(FILE_INGAME_SCREEN_MASK, HOOK_INGAME_SCREEN_MASK, NULL);
	mt_load_texture(FILE_CELL_BLOCK_MASK, HOOK_CELL_BLOCK_MASK, NULL);
	mt_load_texture(FILE_CELL_FREE_MASK, HOOK_CELL_FREE_MASK, NULL);

	if ((mt_search_texture(HOOK_INGAME_SCREEN_MASK) == NULL)
			|| (mt_search_texture(HOOK_CELL_BLOCK_MASK) == NULL)
			|| (mt_search_texture(HOOK_CELL_FREE_MASK) == NULL)) {
		printf("could not load textures for core game!\n");
		quit_game_with_log_error("data management error.\n", 1);
	}

	// ALPHA INGAME

	mt_load_texture(FILE_CELL_BLOCK_MASK_ALPHA, HOOK_CELL_BLOCK_MASK_ALPHA,
			NULL);

	// FONTS
	// LETTERS (font0xx.png)
	for (int i=65;i<=90;i++) {
        char letter_image_id[255];
        char *letter_basename;
        sprintf(letter_image_id, LETTERS_ID_FONTS_ID_FORMAT_STR, i);
        letter_basename=basename(letter_image_id);
        printf("letter_image_id: '%s'\n", letter_image_id);
        printf("letter_basename: '%s'\n", letter_basename);

    	mt_load_texture(letter_image_id, letter_basename,
    			NULL);
	}

	// NUMBERS (num0xx.png)

	for (int i=48;i<=57;i++) {
        char num_image_id[255];
        char *num_basename;
        sprintf(num_image_id, NUMBER_FONTS_ID_FORMAT_STR, i);
        num_basename=basename(num_image_id);
        printf("letter_image_id: '%s'\n", num_image_id);
        printf("letter_basename: '%s'\n", num_basename);

    	mt_load_texture(num_image_id, num_basename,
    			NULL);
	}

	return TRUE;
}

/**************************************************************************
 *
 * Start and run the Framework.
 *
 **************************************************************************/
void mg_run() {

	if (mg_boot()) {
		printf("mg_run() : booting complete...\n");

	} else {
		printf("ERROR!\n");
		mg_quit();
		return;
	}

	// init frame-counter
	cou_init();

	// new random seed
	srand(time(NULL));

	printf("game running now...\n");

	unsigned int score = 0;

	mg_main_game_state = ST_MAIN_MENU;

	while (mg_main_game_state != ST_EXIT) {

		score = START_SCORE;

		if (mg_main_game_state == ST_MAIN_MENU) {
			// start Main Menu
			mm_run();
			mg_main_game_state = mm_get_game_state();
		}
		if (mg_main_game_state == ST_CORE_GAME) {
			// start Core game
			score = cg_run();
			printf("YOUR SCORE : %u\n", score);
			mg_main_game_state = cg_get_game_state();
			printf("exited core game loop. game state is : %d\n",
					mg_main_game_state);
		}
		if (mg_main_game_state == ST_HALL_OF_FAME) {
			// start Hall of Fame
			hof_run(score);
			mg_main_game_state = hof_get_game_state();
		}
	}

	mg_quit();
}

/**************************************************************************
 *
 * Boots the SDL-API
 *
 **************************************************************************/
int mg_boot() {
	printf("mg_boot() : booting MultiMediaLayer...");
	if (sdla_boot_mmAPI_SDL() == FALSE)
		return FALSE;
	else
		printf("SDL3 ok.\n");
	if (mg_init() == FALSE)
		return FALSE;

	mt_init();

	// load the Textures.
	mg_load_textures();

	// remember the textures, so you don't have to search them again
	// by using the texture manager
	gfx_init_hooks();

	printf("ok.\n");

	printf("mg_boot() : booting game manager complete.\n");

	return TRUE;
}
/**************************************************************************
 *
 * Initializes the Framework
 *
 **************************************************************************/
static int mg_init() {

	printf("init game manager...\n");

	mg_exit_flag = FALSE;
	mg_flag_fps = FALSE;
	mg_main_game_state = ST_MAIN_MENU;

	// new random seed
	srand(time(NULL));

	return TRUE;
}

/**************************************************************************
 *
 * Quit's the game in a clean way.
 *
 **************************************************************************/
static void mg_quit() {
	prepare_quit_game();
}
