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

unsigned short mg_exit_flag;
unsigned short mg_flag_fps;
int mg_main_game_state;

/***************************************************/
/* BOOTING                                         */
/***************************************************/

static int mg_boot(void);
static int mg_init(void);
/* add all MyGameTextures to a tree of MyGameTextures */
static int mg_load_textures(void);
static int mg_load_texture(char *p_filename, char *p_tex_hook_id,
		MyGameTexture_t **p_hook);

static void mg_quit(void);

static int mg_load_texture(char *p_filename, char *p_tex_hook_id,
		MyGameTexture_t **p_hook) {
	SDL_Surface *surface = NULL;
	char *bmp_path = NULL;

	/* Textures are pixel data that we upload to the video hardware for fast drawing. Lots of 2D
	 engines refer to these as "sprites." We'll do a static MyGameTexture (upload once, draw many
	 times) with data from a bitmap file. */

	/* SDL_Surface is pixel data the CPU can access. SDL_Texture is pixel data the GPU can access.
	 Load a .bmp into a surface, move it to a MyGameTexture from there. */
	SDL_asprintf(&bmp_path, "%s%s", SDL_GetBasePath(), p_filename); /* allocate a string of the full file path */

	surface = IMG_Load(bmp_path);
	if (!surface) {
		printf("Couldn't load image: %s\n", SDL_GetError());
		SDL_Log("Couldn't load image: %s", SDL_GetError());
		quit_game_with_log_error("data management error.\n", 1);
	}
	SDL_free(bmp_path); /* done with this, the file is loaded. */

	if (SDL_SetSurfaceColorKey(surface, true,
			SDL_MapSurfaceRGB(surface, 0xFF, 0xFF, 0xFF)) == false) {
		printf("Unable to color key! SDL error: %s\n", SDL_GetError());
		SDL_Log("Unable to color key! SDL error: %s", SDL_GetError());
		quit_game_with_log_error("data management error.\n", 1);
	}

	SDL_Texture *tmp_texture = SDL_CreateTextureFromSurface(sdla_get_renderer(),
			surface);
	if (tmp_texture == NULL) {
		printf("Couldn't convert surface to texture : %s\n", SDL_GetError());
		SDL_Log("Couldn't convert surface to texture : %s", SDL_GetError());
		quit_game_with_log_error("data management error.\n", 1);
	}

	mt_add_texture(p_tex_hook_id, tmp_texture, tmp_texture->w, tmp_texture->h);

	/* SDL_DestroySurface(surface); *//* done with this, the MyGameTexture has a copy of the pixels now. */

	printf("hooking texture %s ... \n", p_tex_hook_id);
	if (p_hook != NULL) {
		*p_hook = mt_search_texture(p_tex_hook_id);

		if (*p_hook == NULL) {
			printf("could not find %s in the tree! exit.\n", p_tex_hook_id);
			quit_game_with_log_error("data management error.\n", 1);
		}
		printf("adress hook[%s] : [%p]\n", p_tex_hook_id, *p_hook);
	}

	return TRUE;
}

/* Load the MyGameTextures for the core game. */
static int mg_load_textures() {
	/* FONTS : */

	mg_load_texture(FILE_FONT_DATA_01, HOOK_FONT_DATA_01, NULL);

	if (mt_search_texture(HOOK_FONT_DATA_01) == NULL) {
		printf("could not load fonts!\n");
		quit_game_with_log_error("data management error.\n", 1);
	}

	/* MAIN MENU : */

	mg_load_texture(FILE_MMENU_SCREEN_MASK_OFF, HOOK_MMENU_SCREEN_MASK_OFF,
	NULL);
	mg_load_texture(FILE_MMENU_SCREEN_MASK_ON, HOOK_MMENU_SCREEN_MASK_ON, NULL);

	if ((mt_search_texture(HOOK_MMENU_SCREEN_MASK_OFF) == NULL)
			|| (mt_search_texture(HOOK_MMENU_SCREEN_MASK_ON) == NULL)) {
		printf("could not load textures for menu!\n");
		quit_game_with_log_error("data management error.\n", 1);
	}

	/* HALL OF FAME : */

	mg_load_texture(FILE_HOF_SCREEN_MASK, HOOK_HOF_SCREEN_MASK, NULL);

	if (mt_search_texture(HOOK_HOF_SCREEN_MASK) == NULL) {
		printf("could not load textures for hall of fame!\n");
		quit_game_with_log_error("data management error.\n", 1);
	}

	/* INGAME : */

	mg_load_texture(FILE_INGAME_SCREEN_MASK, HOOK_INGAME_SCREEN_MASK, NULL);
	mg_load_texture(FILE_CELL_BLOCK_MASK, HOOK_CELL_BLOCK_MASK, NULL);
	mg_load_texture(FILE_CELL_FREE_MASK, HOOK_CELL_FREE_MASK, NULL);

	if ((mt_search_texture(HOOK_INGAME_SCREEN_MASK) == NULL)
			|| (mt_search_texture(HOOK_CELL_BLOCK_MASK) == NULL)
			|| (mt_search_texture(HOOK_CELL_FREE_MASK) == NULL)) {
		printf("could not load textures for core game!\n");
		quit_game_with_log_error("data management error.\n", 1);
	}

	/* ALPHA INGAME */

	mg_load_texture(FILE_CELL_BLOCK_MASK_ALPHA, HOOK_CELL_BLOCK_MASK_ALPHA,
			NULL);

	return TRUE;
}

void mg_run() {

	if (mg_boot() == TRUE) {
		printf("mg_run() : booting complete...\n");

	} else {
		printf("ERROR!\n");
		return;
	}

	/* new random seed */
	srand(time(NULL));

	printf("game running now...\n");

	unsigned int score = 0;

	mg_main_game_state = ST_MAIN_MENU;

	while (mg_main_game_state != ST_EXIT) {

		score = START_SCORE;

		if (mg_main_game_state == ST_MAIN_MENU) {
			// Main loop of Main Menu
			mm_run();
			mg_main_game_state = mm_get_game_state();
		}
		if (mg_main_game_state == ST_CORE_GAME) {
			// Main loop of the core game
			score = cg_run();
			printf("YOUR SCORE : %u\n", score);
			mg_main_game_state = cg_get_game_state();
			printf("exited core game loop. game state is : %d\n",
					mg_main_game_state);
		}
		if (mg_main_game_state == ST_HALL_OF_FAME) {
			// Main loop of the Hall of Fame
			hof_run(score);
			mg_main_game_state = hof_get_game_state();
		}
	}

	mg_quit();
}

int mg_boot() {
	printf("mg_boot() : booting MultiMediaLayer...");
	if (sdla_boot_mmAPI_SDL() == FALSE)
		return FALSE;
	else
		printf("SDL3 ok.\n");
	if (mg_init() == FALSE)
		return FALSE;

	mt_init();

	//mg_start = SDL_GetPerformanceCounter();

	/* load the Textures. */
	mg_load_textures();

	gfx_init_hooks();

	printf("ok.\n");

	printf("mg_boot() : booting game manager complete.\n");

	return TRUE;
}

static int mg_init() {

	printf("init game manager...\n");

	mg_exit_flag = FALSE;
	mg_flag_fps = FALSE;
	mg_main_game_state = ST_MAIN_MENU;

	/* new random seed */
	srand(time(NULL));

	return TRUE;
}

static void mg_quit() {
	prepare_quit_game();
}

/*Uint64 mg_get_freq() {
 return mg_freq;
 }*/
