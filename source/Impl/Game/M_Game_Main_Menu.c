#include "M_Game_Main_Menu.h"

#include <stdio.h>

#include "D_Common.h"
#include "D_fonts.h"
#include "S_SDL3_Rendering.h"
#include "S_FPS_Counter.h"

static int mm_game_state;
static int mm_exit_flag;
static int mm_flag_fps;

static int mm_menu_option;

static void mm_main_loop(void);
static void mm_dispatch_keyboard_events(void);
static void mm_render(void);
static void mm_switch_menu(int inc_factor);

static void mm_init(void);

static void mm_init() {
	mm_game_state = ST_MAIN_MENU;
	mm_exit_flag = FALSE;
	mm_flag_fps = FALSE;
	mm_menu_option = MM_OPT_NEW_GAME;
}

void mm_run() {
	mm_init();
	mm_main_loop();
}

int mm_get_game_state() {
	return mm_game_state;
}

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

static void mm_main_loop() {
	printf("entering main menu...\n");

	mm_init();

	int limit_fps_flag = LIMIT_FPS;

	while (mm_game_state == ST_MAIN_MENU) {

		cou_inc_frame();
		cou_calc_fps();
		if (limit_fps_flag) {
			cou_limit_fps();
		}

		mm_dispatch_keyboard_events();

		mm_render();
	}
}

static void mm_dispatch_keyboard_events() {

	SDL_Event event;

	if (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT) {
			mm_game_state = ST_EXIT;
		} else if (event.type == SDL_EVENT_KEY_DOWN) {

			printf("MAIN-MENU-DISPATCH-EVENT ::: %d\n", event.key.key);
			if (event.key.key == SDLK_DOWN) {
				mm_switch_menu(MM_DOWN);
				return;
			}
			if (event.key.key == SDLK_UP) {
				mm_switch_menu(MM_UP);
				return;
			}
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
			if (event.key.key == SDLK_F10) {
				mm_flag_fps = !mm_flag_fps;
				return;
			}
			if (event.key.key == SDLK_ESCAPE) {
				mm_game_state = ST_EXIT;
				return;
			}
		}
	}
}

static void mm_render() {

	sdla_clear_buffer();

	/* render the ingame background image */
	sdla_render_texture(mt_search_texture(HOOK_MMENU_SCREEN_MASK_OFF), 0.0f,
			0.0f);

	int startx = 320;
	int starty = 182;

	sdla_printf_tex2(290, 35, 3, "GREEN");
	sdla_printf_tex2(330, 35 + YSPACEING, 3, "TEA");

	sdla_printf_tex2(startx, starty, 2, "NEW GAME");
	sdla_printf_tex2(startx, starty + 2 * YSPACEING, 2, "OPTIONS");
	sdla_printf_tex2(startx - 25, starty + 4 * YSPACEING, 2, "HALL OF FAME");
	sdla_printf_tex2(startx + 20, starty + 6 * YSPACEING, 2, "EXIT");

	/* render, which option is chosen. */
	switch (mm_menu_option) {
	case MM_OPT_NEW_GAME:
		sdla_printf_tex2(startx, starty, 3, "NEW GAME");
		break;

	case MM_OPT_OPTIONS:
		sdla_printf_tex2(startx, starty + 2 * YSPACEING, 3, "OPTIONS");
		break;

	case MM_OPT_HALL_OF_FAME:
		sdla_printf_tex2(startx - 25, starty + 4 * YSPACEING, 3,
				"HALL OF FAME");
		break;

	case MM_OPT_EXIT:
		sdla_printf_tex2(startx + 20, starty + 6 * YSPACEING, 3, "EXIT");
		break;

	default:
		break;
	}

	/* if activated, show fps-counter */
	if (mm_flag_fps) {
		cou_print_fps();
	}


	sdla_present_buffer();
}
