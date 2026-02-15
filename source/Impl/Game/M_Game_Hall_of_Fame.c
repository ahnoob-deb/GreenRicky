/***********************************************************************
 * This is the Module for :
 *   * showing the data of the highscore-table
 *   * managing the highscore-table
 *   * rendering the highscore-table to the screen
 *
 **********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "D_HOF_Data.h"
#include "M_Game_Hall_of_Fame.h"
#include "D_Common.h"
#include "D_fonts.h"
#include "S_Quit.h"
#include "S_SDL3_Rendering.h"
#include "S_FPS_Counter.h"

static int hof_exit_flag;  // wether the user wants to quit the game
static int hof_game_state; // state of the hall-of-fame-module
static int hof_flag_fps;   // wether the fps should be shown

static HSCData_t *current_score = NULL;   // a pointer to the current entry-record

static int hof_edit;  // wether the hall of fame is editable
static int hof_save;  // wether the hall of fame file can be saved
static int hof_change_entry; // wether the current entry can be changed
static int hof_change_index; // the index of the record that you can change
static int hof_letter_index; // index of the latter you typed in the highscore-table
static int hof_first_letter_set; // wether at least one letter is set for a highscore-record

static HSCData_t *hof_data[MAX_TOPS]; // an array of pointers to highscore-records

static unsigned int hof_score;      // current score

/* The main loop: Where all (user) actions are orchestrated
 * and interpreted, and every image is rendered.
 */
static void hof_main_loop(void);

/* This function interprets keyboard inputs and
 * initiates corresponding actions. */
static void hof_dispatch_keyboard_events(void);

/* For rendering the data of the highscore-table to the screen. */
static void hof_core_render(void);

/* initialize the highscore-table */
static void hof_init(unsigned int p_score);

/* print the table to the console for debugging purpose. */
static void hof_print_table(void);

/* reads the highscore-data-file which contains all data of our highscore-table. */
static void hof_read_highscore(void);

/* writes the (new) data to the highscore-data-file. */
static void hof_save_highscore(void);

/* cleanup the hall of fame after all is done. */
static void hof_free_highscore_table(void);

/* find out, which place the current highscore reaches and put it there. */
static void hof_position_current_score(void);

/* recursive repositioning of a (new) highscore-record. */
static void hof_insert_rank(int p_position, HSCData_t *p_data);

/* get the last position in the highscore-table. */
static int hof_get_max_index(void);

/* clears all pointers to the highscore-table-records to NULL */
static void hof_clear_highscore_table(void);


/**************************************************************************
 *
 * get the last position in the highscore-table.
 *
 **************************************************************************/
static int hof_get_max_index() {
	int ret = 0;
	int i = 0;
	while ((i < MAX_TOPS) && (hof_data[i] != NULL)) {
		i++;
	}

	if (i > 0)
		ret = i - 1;
	return ret;
}

/**************************************************************************
 *
 * print the table to the console for debugging purpose.
 *
 **************************************************************************/
static void hof_print_table() {
	for (int i = 0; (i < MAX_TOPS) && (hof_data[i] != NULL); i++) {
		printf("SCORE : %s, NAME : %s.\n", hof_data[i]->score,
				hof_data[i]->name);
	}
}

/**************************************************************************
 *
 * init's the hall of fame and starts the main-loop.
 *
 **************************************************************************/
void hof_run(unsigned int p_score) {
	hof_init(p_score);
	hof_main_loop();
}

/**************************************************************************
 *
 * initialize the highscore-table.
 *
 **************************************************************************/
static void hof_init(unsigned int p_score) {

	printf("init hall of fame...\n");

	hof_exit_flag = FALSE;
	hof_game_state = ST_HALL_OF_FAME;
	hof_flag_fps = FALSE;
	hof_edit = FALSE;
	hof_save = FALSE;
	hof_score = p_score;
	if (hof_score > 0) {
		hof_edit = TRUE;
		hof_save = TRUE;
	}

	printf("score is %d, hof_edit is %d.\n", hof_score, hof_edit);

	hof_clear_highscore_table();

	hof_read_highscore();

	printf("after read :\n");
	hof_print_table();

}

/**************************************************************************
 *
 * clears all pointers to the highscore-table-records to NULL.
 *
 **************************************************************************/
static void hof_clear_highscore_table() {
	for (int i = 0; i < MAX_TOPS; i++) {
		hof_data[i] = NULL;
	}
}

/**************************************************************************
 *
 * cleanup the hall of fame after all is done.
 *
 **************************************************************************/
static void hof_free_highscore_table() {

	printf("free highscore table...\n");

	for (int i = 0; (i < MAX_TOPS) && (hof_data[i] != NULL); i++) {

		printf("free highscore of %s[%s].\n", hof_data[i]->name,
				hof_data[i]->score);
		free(hof_data[i]);
		hof_data[i] = NULL;
		printf("freed highscore.\n");
	}
}

/**************************************************************************
 *
 * recursive repositioning of a (new) highscore-record.
 *
 **************************************************************************/
static void hof_insert_rank(int p_position, HSCData_t *p_data) {
	if (p_data != NULL) {
		printf("inserting %s[%s] at index %d.\n", p_data->name, p_data->score,
				p_position);

		HSCData_t *temp = NULL;

		if (p_position < MAX_TOPS - 1) {

			temp = hof_data[p_position];
			if (temp != NULL)
				hof_insert_rank(p_position + 1, temp);

			hof_data[p_position] = p_data;
		} else {
			if (hof_data[p_position] != NULL)
				free(hof_data[p_position]);
			hof_data[p_position] = p_data;
		}
	} else {
		quit_game_with_log_error("ERROR : p_data is NULL!", 1);
	}
}

/**************************************************************************
 *
 * writes the (new) data to the highscore-data-file.
 *
 **************************************************************************/
static void hof_save_highscore(void) {
	printf("saving highscore...\n");

	int max_index = hof_get_max_index();

	printf("max index is: %d\n", max_index);

	if (hof_save) {

		FILE *f = fopen(FILE_HOF, "w");

		char line[MAX_LINE_LENGTH];

		int i = 0;

		for (i = 0; i < MAX_LINE_LENGTH; i++) {
			line[i] = 0;
		}

		i = 0;

		while ((i < MAX_TOPS) && (hof_data[i] != NULL)) {
			strncpy(line, hof_data[i]->score, MAX_SCORE_LEN);
			strncat(line, ";", 2);
			strncat(line, hof_data[i]->name, MAX_NAME_LEN);
			printf("before last strncat\n");
			if ((i < MAX_TOPS - 1) && (hof_data[i + 1] != NULL))
				strncat(line, "\n", 2);
			printf("after last strncat\n");
			printf("%c", line[0]);
			fwrite(line, sizeof(char), strlen(line), f);
			i++;
		}
		fclose(f);
	} else {
		printf("WARNING : saving is disabled.\n");
	}
}

/**************************************************************************
 *
 * find out, which place the current highscore reaches and put it there.
 *
 **************************************************************************/
static void hof_position_current_score() {

	printf("new highscore? : %d\n", hof_score);

	if (hof_score > 0) {

		hof_change_entry = TRUE;
		hof_letter_index = 0;
		hof_first_letter_set = FALSE;

		int i = 0;
		int found_place = FALSE;

		current_score = (HSCData_t*) calloc(1, sizeof(HSCData_t));

		snprintf(current_score->score, MAX_SCORE_LEN, "%d", hof_score);
		snprintf(current_score->name, MAX_NAME_LEN, "%s", "");

		printf("HIGHSCORE : %s : %s\n", current_score->name,
				current_score->score);

		for (i = 0; (i < MAX_TOPS) && (hof_data[i] != NULL); i++) {
			if (atoi(hof_data[i]->score) < atoi(current_score->score)) {
				found_place = TRUE;
				break;
			}
		}

		hof_change_index = i;

		if (hof_change_index < MAX_TOPS) {
			found_place = TRUE;
		} else {
			hof_edit = FALSE;
		}

		printf("you reached place : %d\n", hof_change_index + 1);

		if ((i < MAX_TOPS) && (found_place)) {
			printf("found place - place new highscore.\n");

			hof_insert_rank(i, current_score);
		}
	}
}

/**************************************************************************
 *
 * reads the highscore-data-file which contains
 * all data of our highscore-table.
 *
 **************************************************************************/
static void hof_read_highscore() {
	FILE *f = fopen(FILE_HOF, "r");

	if (f == NULL) {
		printf("could not open hof.txt");
		quit_game_with_log_error("data management error.", 1);
	}

	char line[MAX_LINE_LENGTH];
	char *token = NULL;
	int no = 0;

	int i = 0;

	while ((i < MAX_TOPS) && (fgets(line, MAX_LINE_LENGTH, f) != NULL)) {

		int last_char = strlen(line) - 1;

		/* Remove newline-char at end of line */
		if (line[last_char] == '\n')
			line[last_char] = '\0';

		no = 0;

		token = strtok(line, HOF_DELIMITER);
		printf("\n");

		hof_data[i] = (HSCData_t*) calloc(1, sizeof(HSCData_t));

		while (token != NULL) {

			if (hof_data[i] == NULL) {
				printf("ERROR : ");
				printf("memory allocation error at loading hall of fame.");
				quit_game_with_log_error("memory allocation error", 1);
			}

			if (no == 0) {
				strncpy(hof_data[i]->score, token, MAX_SCORE_LEN - 1);
				printf("hof_data[%d]->score : %s\n", i, hof_data[i]->score);
			} else if (no == 1) {
				strncpy(hof_data[i]->name, token, MAX_NAME_LEN - 1);
				printf("hof_data[%d]->name : %s\n", i, hof_data[i]->name);
			}
			printf("added[%d] : [%s]\n", no, token);
			token = strtok(NULL, HOF_DELIMITER);
			no = !no;
		}

		i++;
	}

	fclose(f);
}

/**************************************************************************
 *
 * Returns the state of the hof-module.
 *
 **************************************************************************/
int hof_get_state() {
	return hof_game_state;
}

/**************************************************************************
 *
 * The main loop: Where all (user) actions are orchestrated
 * and interpreted, and every image is rendered.
 *
 **************************************************************************/
static void hof_main_loop() {
	printf("entering hall of fame loop...");

	printf("setting score %u\n", hof_score);

	hof_position_current_score();

	hof_print_table();

	int limit_fps_flag = LIMIT_FPS;

	while (!hof_exit_flag && hof_game_state == ST_HALL_OF_FAME) {

		cou_inc_frame();
		cou_calc_fps();
		if (limit_fps_flag) {
			cou_limit_fps();
		}

		hof_dispatch_keyboard_events();
		hof_core_render();
	}

	hof_free_highscore_table();

}

/**************************************************************************
 *
 * This function interprets keyboard inputs and
 * initiates corresponding actions.
 *
 **************************************************************************/
static void hof_dispatch_keyboard_events() {

	SDL_Event event;

	if (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT) {
			hof_game_state = ST_EXIT;
		} else if (event.type == SDL_EVENT_KEY_DOWN) {

			printf("HOF-DISPATCH-EVENT ::: %d\n", event.key.key);
			printf("HOF-EDIT ::: %d\n", hof_edit);
			printf("FIRST_LETTER_SET ::: %d\n", hof_first_letter_set);

			if (event.key.key == SDLK_ESCAPE) {
				hof_game_state = ST_MAIN_MENU;
				return;
			}
			if (event.key.key == SDLK_F10) {
				hof_flag_fps = !hof_flag_fps;
				return;
			}
			// TYPING
			if (hof_edit) {
				if (event.key.key == SDLK_RETURN) {
					if (hof_first_letter_set) {
						hof_edit = FALSE;
						hof_save_highscore();
					}
					return;
				}
				if (event.key.key == SDLK_SPACE) {
					if (hof_first_letter_set) {
						if (hof_letter_index < MAX_NAME_LEN) {
							printf("adding space at %d\n", hof_letter_index);
							hof_data[hof_change_index]->name[hof_letter_index++] =
									' ';
						}
					}
					return;
				}
				if (event.key.key == SDLK_BACKSPACE) {
					if (hof_letter_index > 0) {
						printf("removing char at %d\n", hof_letter_index - 1);
						hof_data[hof_change_index]->name[--hof_letter_index] =
								'\0';
					} else
						hof_first_letter_set = FALSE;
					return;
				}
				switch (event.key.key) {
				case (SDLK_TAB):
				case (SDLK_LCTRL):
				case (SDLK_RCTRL):
				case (SDLK_LALT):
				case (SDLK_RALT):
				case (SDLK_LSHIFT):
				case (SDLK_RSHIFT):
				case (SDLK_CAPSLOCK):
					printf("HOF-DISPATCH-EVENT ::: KEYMOD pressed\n");
					return;
					break;
				}
				if (((event.key.key >= (int) SDLK_0)
						&& (event.key.key <= (int) SDLK_9))
						|| ((event.key.key >= (int) SDLK_A)
								&& (event.key.key <= (int) SDLK_Z))) {
					printf("adding printable char at %d\n", hof_letter_index);
					if (hof_letter_index < MAX_NAME_LEN - 1) {
						hof_data[hof_change_index]->name[hof_letter_index++] =
								event.key.key;
						hof_first_letter_set = TRUE;
					}
				}
			}
		}
	}
}

/**************************************************************************
 *
 * For rendering the data of the highscore-table to the screen.
 *
 **************************************************************************/
static void hof_core_render() {
	sdla_clear_buffer();

	/* render the ingame background image */
	sdla_render_texture(mt_search_texture(HOOK_HOF_SCREEN_MASK), 0.0f, 0.0f);

	int i = 0;
	int x = 450;
	int y = 325;
	unsigned int hof_color = 4;
	unsigned int hof_cscore_color = 2;

	int transparency = ALPHA_SOLID;

	sdla_printf_tex3(589, 102, 3, GLOBAL_FONT_TRANSPARENCY, "HIGHSCORES");

	while ((i < MAX_TOPS) && (hof_data[i] != NULL)) {
		if ((i == hof_change_index) && hof_edit) {
			if (strlen(hof_data[i]->score) > 0) {
				sdla_printf_tex3(x, y, hof_cscore_color, ALPHA_SOLID,
						hof_data[i]->score);
				sdla_printf_tex3(x + 5 * YSPACEING, y, hof_cscore_color,
						ALPHA_SOLID, hof_data[i]->name);
			}

		} else {
			sdla_printf_tex3(x, y, hof_color, transparency, hof_data[i]->score);
			sdla_printf_tex3(x + 5 * YSPACEING, y, hof_color, transparency,
					hof_data[i]->name);
		}
		y += YSPACEING;
		i++;
	}
	/* if activated, show fps-counter */
	if (hof_flag_fps) {
		cou_print_fps();
	}
	sdla_present_buffer();
}
