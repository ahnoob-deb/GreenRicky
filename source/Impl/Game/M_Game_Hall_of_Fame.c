#include <D_HOF_Data.h>
#include "M_Game_Hall_of_Fame.h"

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "D_Common.h"
#include "S_Quit.h"
#include "S_SDL3_Rendering.h"

static int hof_exit_flag;
static int hof_game_state;
static int hof_flag_fps;

static HSCData *current_score = NULL;

static int hof_edit;
static int hof_save;
static int hof_change_entry;
static int hof_change_index;
static int hof_letter_index;
static int hof_first_letter_set;

static HSCData *hof_data[MAX_TOPS];

static unsigned int hof_score;

static void hof_main_loop(void);

static void hof_dispatch_core_events(int p_ev);
static void hof_core_render(void);

static void hof_init(unsigned int p_score);

static void print_hof_table(void);

static void read_highscore(void);

static void save_highscore(void);

static void free_highscore_table(void);

static void position_current_score(void);

static void hof_insert_rank(int p_position, HSCData *p_data);

static int hof_get_max_index(void);
static void init_highscore_table(void);

static int hof_get_max_index() {
	int ret = 0;
	int i = 0;
	while ((hof_data[i] != NULL) && i < MAX_TOPS) {
		i++;
	}

	if (i > 0)
		ret = i - 1;
	return ret;
}

static void print_hof_table() {
	for (int i = 0; (i < MAX_TOPS) && (hof_data[i] != NULL); i++) {
		printf("SCORE : %s, NAME : %s.\n", hof_data[i]->score,
				hof_data[i]->name);
	}
}

void hof_run(unsigned int p_score) {
	hof_init(p_score);
	hof_main_loop();
}

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

	init_highscore_table();
	printf("after init :\n");
	print_hof_table();

	read_highscore();

	printf("after read :\n");
	print_hof_table();

}

static void init_highscore_table() {
	for (int i = 0; i < MAX_TOPS; i++) {
		hof_data[i] = NULL;
	}
}

static void free_highscore_table() {

	printf("free highscore table...\n");

	for (int i = 0; (i < MAX_TOPS) && (hof_data[i] != NULL); i++) {

		printf("free highscore of %s[%s].\n", hof_data[i]->name,
				hof_data[i]->score);
		free(hof_data[i]);
		hof_data[i] = NULL;
		printf("freed highscore.\n");
	}
}

static void hof_insert_rank(int p_position, HSCData *p_data) {

	HSCData *temp = NULL;

	if (p_position < MAX_TOPS - 1) {

		temp = hof_data[p_position];

		hof_insert_rank(p_position + 1, temp);

		hof_data[p_position] = p_data;
	} else {
		free(hof_data[p_position]);
		hof_data[p_position] = p_data;
	}
}

static void save_highscore(void) {
	printf("saving highscore...\n");

	int max_index = hof_get_max_index();

	printf("max index is: %d\n", max_index);

	/*FILE *f = fopen(FILE_HOF, "w");

	 fclose(f);*/
}

static void position_current_score() {

	printf("new highscore? : %d\n", hof_score);

	if (hof_score > 0) {

		hof_change_entry = TRUE;
		hof_letter_index = 0;
		hof_first_letter_set = FALSE;

		int i = 0;
		int found_place = FALSE;

		current_score = calloc(1, sizeof(HSCData));

		snprintf(current_score->score, MAX_SCORE_LEN, "%d", hof_score);
		snprintf(current_score->name, MAX_SCORE_LEN, "%s", "");

		printf("HIGHSCORE : %s : %s\n", current_score->name,
				current_score->score);

		for (i = 0; i < MAX_TOPS; i++) {
			if (atoi(hof_data[i]->score) <= atoi(current_score->score)) {
				found_place = TRUE;
				break;
			}
		}

		hof_change_index = i;

		printf("you reached place #%d\n", hof_change_index + 1);

		if (found_place == TRUE && i < MAX_TOPS) {
			printf("found place - replacing old highscore.\n");

			hof_insert_rank(i, current_score);
		}
	}
}

static void read_highscore() {
	FILE *f = fopen(FILE_HOF, "r");

	if (f == NULL) {
		printf("could not open hof.txt");
		quit_game_with_log_error("data management error.", 1);
	}

	char line[MAX_LINE_LENGTH];
	char *token = NULL;
	int no = 0;

	int i = 0;

	while ((fgets(line, MAX_LINE_LENGTH, f) != NULL) && (i < MAX_TOPS)) {
		no = 0;


		token = strtok(line, HOF_DELIMITER);
		printf("\n");


		hof_data[i] = (HSCData*) calloc(1, sizeof(HSCData));

		while (token != NULL) {

			if (hof_data[i] == NULL) {
				printf("ERROR : ");
				printf("memory allocation error at loading hall of fame.");
				quit_game_with_log_error("memory allocation error", 1);
			}

			if (no == 0) {
				strncpy(hof_data[i]->score, token, MAX_SCORE_LEN);
				printf("hof_data[%d]->score : %s\n", i, hof_data[i]->score);
			} else if (no == 1) {
				strncpy(hof_data[i]->name, token, MAX_NAME_LEN);
				printf("hof_data[%d]->name : %s\n", i, hof_data[i]->name);
			}
			printf("added[%d] : [%s]\n", no, token);
			token = strtok(NULL, HOF_DELIMITER);
			no = !no;
		}

		i++;
        printf("%d\n",i);
	}

	fclose(f);
}

int hof_get_game_state() {
	return hof_game_state;
}

static void hof_main_loop() {
	printf("entering hall of fame loop...");

	printf("setting score %u\n", hof_score);

	position_current_score();

	while (!hof_exit_flag && hof_game_state == ST_HALL_OF_FAME) {
		hof_dispatch_core_events(sdla_process_events());
		hof_core_render();
	}

	free_highscore_table();

}

static void hof_dispatch_core_events(int p_ev) {

	if (p_ev != EV_NO_EVENT) {

		printf("HOF-DISPATCH-EVENT ::: %d\n", p_ev);

		if (p_ev == SDLK_ESCAPE) {
			hof_game_state = ST_MAIN_MENU;
			save_highscore();
			return;
		}
		if (p_ev == EV_INSTANT_QUIT) {
			hof_game_state = ST_EXIT;
			save_highscore();
			return;
		}
		if (p_ev == SDLK_F10) {
			hof_flag_fps = !hof_flag_fps;
			return;
		}
		/* TYPING */
		if (hof_edit) {
			if (p_ev == SDLK_RETURN) {
				if (hof_first_letter_set) {
					hof_edit = FALSE;
				}
				return;
			}
			if (p_ev == SDLK_SPACE) {
				if (hof_first_letter_set) {
					if (hof_letter_index < MAX_NAME_LEN)
						hof_data[hof_change_index]->name[hof_letter_index++] =
								' ';
					hof_first_letter_set = TRUE;
				}
				return;
			}
			if (p_ev == SDLK_BACKSPACE) {
				if (hof_letter_index > 0)
					hof_data[hof_change_index]->name[--hof_letter_index] = '\0';
				else
					hof_first_letter_set = FALSE;
				return;
			}
			switch (p_ev) {
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
			if (((p_ev >= (int) SDLK_0) && (p_ev <= (int) SDLK_9))
					|| ((p_ev >= (int) SDLK_A) && (p_ev <= (int) SDLK_Z))) {
				if (hof_letter_index < MAX_NAME_LEN) {
					hof_data[hof_change_index]->name[hof_letter_index++] = p_ev;
					hof_first_letter_set = TRUE;
				}
			}
		}
	}
}

static void hof_core_render() {
	sdla_clear_buffer();

	/* render the ingame background image */
	sdla_render_texture(mt_search_texture(HOOK_HOF_SCREEN_MASK), 0.0f, 0.0f);

	int i = 0;
	int x = 80;
	int y = 170;
	unsigned int hof_color = HOF_COLOR;
	//unsigned int hof_cscore_color = HOF_CSCORE_COLOR;

	while (hof_data[i] != NULL && i < MAX_TOPS) {
		/*	if ((i == hof_change_index) && hof_edit) {
		 if (strlen(hof_data[i]->score) > 0) {
		 sdla_printf_tex(x, y, hof_cscore_color, hof_data[i]->score);
		 sdla_printf_tex(x + 5 * 40, y, hof_cscore_color,
		 hof_data[i]->name);
		 }

		 } else {*/
		sdla_printf_tex(x, y, hof_color, hof_data[i]->score);
		sdla_printf_tex(x + 5 * 40, y, hof_color, hof_data[i]->name);
		/*}*/
		y += 30;
		i++;
	}

	sdla_present_buffer();
}
