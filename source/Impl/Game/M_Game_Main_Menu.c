#include "M_Game_Main_Menu.h"

#include <stdio.h>

#include "D_Common.h"
#include "S_SDL3_Rendering.h"

static int mm_game_state;
static int mm_exit_flag;
static int mm_flag_fps;

static int mm_menu_option;

static void mm_main_loop(void);
static void mm_dispatch_events(int p_ev);
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

	while (mm_game_state == ST_MAIN_MENU) {
		mm_render();

		mm_dispatch_events(sdla_process_events());
	}
}

static void mm_dispatch_events(int p_ev) {

	if (p_ev != EV_NO_EVENT) {
		printf("MAIN-MENU-DISPATCH-EVENT ::: %d\n", p_ev);
		if (p_ev == SDLK_DOWN) {
			mm_switch_menu(MM_DOWN);
			return;
		}
		if (p_ev == SDLK_UP) {
			mm_switch_menu(MM_UP);
			return;
		}
		if (p_ev == SDLK_RETURN) {
			if (mm_menu_option == MM_OPT_NEW_GAME)
				mm_game_state = ST_CORE_GAME;
			else if (mm_menu_option == MM_OPT_OPTIONS)
				mm_game_state = ST_MAIN_MENU;
			else if (mm_menu_option == MM_OPT_HALL_OF_FAME)
				mm_game_state = ST_HALL_OF_FAME;
			else if (mm_menu_option == MM_OPT_EXIT)
				mm_game_state=ST_EXIT;
			return;
		}
		if (p_ev == SDLK_F10) {
			mm_flag_fps = !mm_flag_fps;
			return;
		}
		if (p_ev == SDLK_ESCAPE) {
			mm_game_state=ST_EXIT;
			return;
		}
		if (p_ev == EV_INSTANT_QUIT) {
			mm_game_state=ST_EXIT;
			return;
		}
	}
}

static void mm_render() {

	sdla_clear_buffer();

	/* render the ingame background image */
	sdla_render_texture(mt_search_texture(HOOK_MMENU_SCREEN_MASK_OFF), 0.0f, 0.0f);

	/* render, which option is chosen. */
	switch (mm_menu_option) {
	case MM_OPT_NEW_GAME:
		sdla_render_rect_from_tex(mt_search_texture(HOOK_MMENU_SCREEN_MASK_ON),
				304.0f, 182.0f, 304.0f + 165.0f, 182.0f + 40.0f, 304.0f, 182.0f,
				304.0f + 165.0f, 182.0f + 40.0f);
		break;

	case MM_OPT_OPTIONS:
		sdla_render_rect_from_tex(mt_search_texture(HOOK_MMENU_SCREEN_MASK_ON),
				304.0f, 248.0f, 304.0f + 127.0f, 248.0f + 51.0f, 304.0f, 248.0f,
				304.0f + 127.0f, 248.0f + 51.0f);
		break;

	case MM_OPT_HALL_OF_FAME:
		sdla_render_rect_from_tex(mt_search_texture(HOOK_MMENU_SCREEN_MASK_ON),
				304.0f, 320.0f, 304.0f + 186.0f, 320.0f + 43.0f, 304.0f, 320.0f,
				304.0f + 186.0f, 320.0f + 43.0f);
		break;

	case MM_OPT_EXIT:
		sdla_render_rect_from_tex(mt_search_texture(HOOK_MMENU_SCREEN_MASK_ON),
				304.0f, 391.0f, 304.0f + 71.0f, 391.0f + 43.0f, 304.0f, 391.0f,
				304.0f + 71.0f, 391.0f + 43.0f);
		break;

	default:
		break;
	}


	sdla_present_buffer();
}
