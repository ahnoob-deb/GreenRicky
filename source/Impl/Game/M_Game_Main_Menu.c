#include "M_Game_Main_Menu.h"

#include <stdio.h>

#include "D_Common.h"
#include "D_fonts.h"
#include "S_SDL3_Rendering.h"
#include "S_FPS_Counter.h"

/* the state of the main menu */
static int mm_game_state;
/* turn on/off the fps counter */
static int mm_flag_fps;
/* the currently chosen menu-entry */
static int mm_menu_option;

/* the main loop of the main menu */
static void mm_main_loop(void);
/* keyboard-event-handler */
static void mm_dispatch_keyboard_events(void);
/* core-render-function of the main menu */
static void mm_render(void);
/* switch the menu-entry chosen and check its bounds. */
static void mm_switch_menu(int inc_factor);
/* initialize the main menu */
static void mm_init(void);

/**************************************************************************
 *
 * Initialize the main menu.
 *
 * Just set some flags.
 *
 **************************************************************************/
static void mm_init() {
	mm_game_state = ST_MAIN_MENU;
	mm_flag_fps = FALSE;
	mm_menu_option = MM_OPT_NEW_GAME;
}

/**************************************************************************
 *
 * Initialize the main-menu and then start the main loop.
 *
 **************************************************************************/
void mm_run() {
	mm_init();
	mm_main_loop();
}

/**************************************************************************
 *
 * Get the current state of the main menu.
 *
 **************************************************************************/
int mm_get_game_state() {
	return mm_game_state;
}

/**************************************************************************
 *
 * switch the menu-entry chosen and check its bounds.
 *
 **************************************************************************/
static void mm_switch_menu(int inc_factor) {
	mm_menu_option += 1 * inc_factor;
	if (mm_menu_option < 0)
		mm_menu_option = MM_COUNT_OPTIONS - 1;
	if (mm_menu_option > MM_COUNT_OPTIONS - 1)
		mm_menu_option = 0;

	switch (mm_menu_option) {
	case MM_OPT_NEW_GAME:
		printf("option 'new game' chosen\n");
		break;

	case MM_OPT_OPTIONS:
		printf("option 'options' chosen\n");
		break;

	case MM_OPT_HALL_OF_FAME:
		printf("option 'hall of fame' chosen\n");
		break;

	case MM_OPT_EXIT:
		printf("option 'exit' chosen\n");
		break;

	default:
		break;
	}
}

/**************************************************************************
 *
 * The main loop of the main menu. One frame per loop.
 *
 **************************************************************************/
static void mm_main_loop() {
	printf("entering main menu...\n");

	int limit_fps_flag = LIMIT_FPS; // wether the fps should be limited...

	while (mm_game_state == ST_MAIN_MENU) {

		cou_inc_frame(); // count the frames...
		cou_calc_fps();  // calculate the current frames per second
		if (limit_fps_flag) {
			cou_limit_fps(); // limit the fps if activated
		}

		mm_dispatch_keyboard_events(); // handle keyboard-events

		mm_render(); // render the current game state
	}
}
/**************************************************************************
 *
 * Keyboard-event-handler.
 *
 * Processes the keyboard inputs.
 * Will be called every frame.
 *
 **************************************************************************/
static void mm_dispatch_keyboard_events() {

	SDL_Event event;

	if (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT) {
			mm_game_state = ST_EXIT;  // for example, if the x in the right corner of the window is pressed.
		} else if (event.type == SDL_EVENT_KEY_DOWN) {

			printf("MAIN-MENU-DISPATCH-EVENT ::: %d\n", event.key.key);
			// up and down in the main menu...
			if (event.key.key == SDLK_DOWN) {
				mm_switch_menu(MM_DOWN);
				return;
			}
			if (event.key.key == SDLK_UP) {
				mm_switch_menu(MM_UP);
				return;
			}
			// choose a menu entry...
			if (event.key.key == SDLK_RETURN) {
				if (mm_menu_option == MM_OPT_NEW_GAME)
					mm_game_state = ST_CORE_GAME;
				else if (mm_menu_option == MM_OPT_OPTIONS)
					mm_game_state = ST_MAIN_MENU;
				else if (mm_menu_option == MM_OPT_HALL_OF_FAME)
					mm_game_state = ST_HALL_OF_FAME;
				else if (mm_menu_option == MM_OPT_EXIT)
					mm_game_state = ST_EXIT;
				return;
			}
			// turn on / off the fps counter
			if (event.key.key == SDLK_F10) {
				mm_flag_fps = !mm_flag_fps;
				return;
			}
			// user wants to exit the main menu
			if (event.key.key == SDLK_ESCAPE) {
				mm_game_state = ST_EXIT;
				return;
			}
		}
	}
}
/**************************************************************************
 *
 * Core-render-function of the main menu.
 * Will be called every frame.
 *
 **************************************************************************/
static void mm_render() {

	sdla_clear_buffer();

	// render the ingame background image
	sdla_render_texture(mt_search_texture(HOOK_MMENU_SCREEN_MASK_OFF), 0.0f,
			0.0f);

	int startx = 610;
	int starty = 386;

	int transparency = GLOBAL_FONT_TRANSPARENCY;

	// render the menu
	sdla_printf_tex3(630, 97, 3, transparency, HEADLINE1);
	sdla_printf_tex3(630, 97 + YSPACEING, 3, transparency,  HEADLINE2);

	sdla_printf_tex3(startx, starty, 2, transparency, "NEW GAME");
	sdla_printf_tex3(startx, starty + 2 * YSPACEING, 2, transparency, "OPTIONS");
	sdla_printf_tex3(startx - 25, starty + 4 * YSPACEING, 2, transparency, "HALL OF FAME");
	sdla_printf_tex3(startx + 20, starty + 6 * YSPACEING, 2, transparency, "EXIT");

	// emphasize, which option is chosen by choosing another degree of transparency of the font.
	switch (mm_menu_option) {
	case MM_OPT_NEW_GAME:
		sdla_printf_tex3(startx, starty, 3, ALPHA_SOLID,
				"NEW GAME");
		break;

	case MM_OPT_OPTIONS:
		sdla_printf_tex3(startx, starty + 2 * YSPACEING, 3, ALPHA_SOLID,
				"OPTIONS");
		break;

	case MM_OPT_HALL_OF_FAME:
		sdla_printf_tex3(startx - 25, starty + 4 * YSPACEING, 3, ALPHA_SOLID,
				"HALL OF FAME");
		break;

	case MM_OPT_EXIT:
		sdla_printf_tex3(startx + 20, starty + 6 * YSPACEING, 3, ALPHA_SOLID,
				"EXIT");
		break;

	default:
		break;
	}

	// if activated, show fps-counter
	if (mm_flag_fps) {
		cou_print_fps();
	}


	sdla_present_buffer();
}
