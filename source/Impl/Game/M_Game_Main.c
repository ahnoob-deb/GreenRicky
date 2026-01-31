#include "M_Game_Main.h"

#include <stdio.h>
#include <stdlib.h>

#include <SDL3/SDL.h>

#include <time.h>
#include "D_Common.h"
#include "D_fonts.h"
#include "S_Game_Objects.h"
#include "S_Quit.h"
#include "S_SDL3_Rendering.h"
#include "S_Shape_Manager.h"
#include "S_FPS_Counter.h"

static int cg_game_state;
static int cg_exit_flag;
static int cg_flag_fps;
static int cg_flag_break;

/***************************************************/
/* MODULE INTERN VARIABLES & PROTOTYPES            */
/***************************************************/

/* init the coregame. */
static int cg_init(void);

static int cg_booted = FALSE;

/***************************************************/
/* THE MAP, THE Piece_t, NEXT Piece_t AND STATISTICS   */
/***************************************************/

/* the current map */
static MapData_t cg_map_data;
/* the next Piece_t */
static Piece_t cg_next_piece;
/* the current Piece_t */
static Piece_t cg_current_piece;

/* some game statistics */
static GameStats_t cg_stats;

/***************************************************/
/* GAME OVER STATUS CHECKING                       */
/***************************************************/

/* the algorithm to check if its game over. */
static unsigned short cg_check_game_over();

/* wether its game over or not */
static unsigned short cg_game_over_flag;

/***************************************************/
/* SHAPE IDS                                       */
/***************************************************/

/* all ids of the shapes, defined in gobs.c */
extern char *gal_shape_ids[];
/***************************************************/
/*            MAIN LOOP                            */
/***************************************************/
static void cg_main_loop(void);
/***************************************************/
/* Collision detection functions                   */
/* - the heart of the game.                        */
/***************************************************/

static int cg_collision_detection(int p_direction, double add_y);
static unsigned short cg_poi_is_block(int p_index, double p_add_x,
		double p_add_y, int p_direction);

/* always up to date information of
 current collisions */
static unsigned int cg_collis_det_info;

/***************************************************/
/* Functions manipulating the Piece_ts. */
/***************************************************/

static void cg_state_update();
static void cg_spawn_new_piece(Piece_t *pce);
static void cg_park_piece(void);
static void cg_cur_piece_rotate();
static double cg_calc_fall();
static void cg_switch_next_piece();
static void cg_drop_piece();

static double cg_current_speed;
static double cg_reset_speed;

/***************************************************/
/* This is for checking full lines and let them implode. */
/***************************************************/

static void cg_check_full_lines(void);
static void cg_implode_full_lines(void);
static unsigned short cg_found_full_lines(void);
static void cg_clear_full_lines(void);
static void cg_let_line_fade(unsigned int p_y);
static void cg_map_delete_line(unsigned int p_y);
static void cg_map_tumble_lines(unsigned int p_above_y);
static void cg_check_level(void);

/* an array of the full lines marked for imploding */
static int cg_full_lines[MAX_FULL_LINES];
static int cg_full_lines_index;
/* time in ms since the full line has been marked for implode */
static int cg_toi;
static int cg_impalpha;

/***************************************************/
/* Event management.                               */
/***************************************************/

/* Dispatch of the events delivered by the Multi-Media-
 Manager like SDL3 */
//static void cg_dispatch_core_events(int p_ev);
static void cg_dispatch_keyboard_events(void);

/***************************************************/
/* Some render-functions.                          */
/***************************************************/

static void cg_core_render(void);
static void cg_piece_render(Piece_t *p_pce);
static void cg_map_render(MapData_t *p_map);
static void cg_render_stats(void);

static void cg_render_next_piece(void);

/***************************************************/

/***************************************************/
/* VARIABLES USED FOR TIME MEASUREMENT             */
/***************************************************/

// for time measurements where it is needed
static double cg_moment_of_interest1;
static double cg_moment_of_interest2;
/* old time of measure of the fall of the Piece_t */
static double cg_fall_mea_old_time;

/***************************************************/

/***************************************************/
/*            +++ IMPLEMENTATION +++               */
/***************************************************/

unsigned int cg_run() {
	cg_init();
	cg_main_loop();
	printf("returning score : %u\n", cg_stats.score);
	return (cg_stats.score);
}

int cg_get_game_state() {
	return cg_game_state;
}

/* Reset all values of core game. */
static int cg_init() {
	printf("init core game...\n");

	if (!cg_booted) {
		/* load and init the shape data */
		gal_shape_init();
		cg_booted = TRUE;
	}

	cg_exit_flag = FALSE;
	cg_game_state = ST_CORE_GAME;
	cg_flag_fps = FALSE;

	/* for now, time measure of peace fall is set to
	 the core games start time */
	cg_fall_mea_old_time = cou_get_start_ts();

	/* init timer for imploding full lines and its color */
	cg_toi = 0;
	cg_impalpha = ALPHA_SOLID;

	/* reset statistics */
	cg_stats.count_pieces_landed = 0;
	cg_stats.rows_destroyed = 0;
	cg_stats.score = START_SCORE;
	cg_stats.level = 1;

	cg_flag_break = FALSE;

	/* reset all bits of collision detection */
	cg_collis_det_info = 0;

	/* reset time measurement variables */
	//cg_moment_of_interest1 = SDL_GetPerformanceCounter();
	//cg_moment_of_interest2 = cg_moment_of_interest1;
	/* game is not over yet */
	cg_game_over_flag = 0;

	cg_current_speed = L1_FALL_PER_SECOND;
	cg_reset_speed = L1_FALL_PER_SECOND;

	/* reset the array of full lines */
	cg_clear_full_lines();

	/* reset the map */
	gal_map_init(&cg_map_data, 0, 0, MAP_WIDTH, MAP_HEIGHT);

	/* init new Piece_t structures... */
	gal_piece_init(&cg_current_piece, 0, 0, 1, gal_shape_ids[0], 0);
	gal_piece_init(&cg_next_piece, 0, 0, 1, gal_shape_ids[0], 0);

	/* fill the new Piece_t with random values */
	cg_spawn_new_piece(&cg_current_piece);
	cg_spawn_new_piece(&cg_next_piece);

	return TRUE;
}

/* render a Piece_t to screen. */
static void cg_piece_render(Piece_t *p_pce) {
	int color = p_pce->id_no + 1;
	int value_of_shape = 0;

	int drawx = 0;
	int drawy = 0;

	ShapeData_t *sh = p_pce->sh_data[p_pce->direction];

	int index = 0;
	int x = 0;
	int y = 0;

	for (index = 0; index < PIECE_WIDTH * PIECE_HEIGHT; index++) {

		x = p_pce->x;
		y = p_pce->y;

		drawx = index % PIECE_WIDTH - sh->min_x;
		drawy = index / PIECE_WIDTH - sh->min_y;

		value_of_shape = sh->matrix[index];

		if (value_of_shape > FREE) {
			sdla_draw_block(x + drawx, y + drawy, color);
		}
	}
}

/* render a map to screen. */
static void cg_map_render(MapData_t *p_map) {
	/* Draw the Piece_t shape in the console */

	/* Initialize the value of the shape */
	int value_of_matrix = 0;

	int drawx = 0;
	int drawy = 0;
	int percent = 0;

	for (drawy = 0; drawy < MAP_HEIGHT; drawy++) {
		for (drawx = 0; drawx < MAP_WIDTH; drawx++) {

			value_of_matrix = p_map->matrix[drawy * MAP_WIDTH + drawx];

			if (value_of_matrix > FREE) {
				sdla_draw_block(p_map->x + drawx, p_map->y + drawy,
						value_of_matrix);
			} else if (value_of_matrix == FREE) {
				sdla_draw_free(p_map->x + drawx, p_map->y + drawy,
						p_map->color);
			} else if (value_of_matrix == IMPLODING) {
				percent = cg_toi / (IMPLODING_TIME / 100);
				cg_impalpha = ALPHA_SOLID - ((ALPHA_SOLID / 100) * percent);
				printf("ALPHA : %d\n", cg_impalpha);
				sdla_draw_imp(p_map->x + drawx, p_map->y + drawy, 0,
						cg_impalpha);
			}
		}
	}
}

/* clears the full-lines-array and the index counter. The array indicates,
 * which lines in the map are full. Contains in max MAX_FULL_LINES(=4)
 * integers for the y-value of the full line.
 * A value of NO_LINE(=-1) is reserved for "there is no line",
 * as the map has no negative y-values.
 */
static void cg_clear_full_lines() {
	size_t index = 0;

	for (index = 0; index < MAX_FULL_LINES; index++) {
		cg_full_lines[index] = NO_LINE;
	}

	cg_full_lines_index = 0;
}

/* check if game over state is given.
 this means here : check if there is a block in the
 top row of map. */
static unsigned short cg_check_game_over() {
	size_t x = 0;
	size_t y = 0;

	for (x = 0; x < MAP_WIDTH; x++) {
		if (cg_map_data.matrix[y * MAP_WIDTH + x] > FREE) {
			cg_game_over_flag = 1;
			return TRUE;
		}
	}
	return FALSE;
}

/* a sub-function of the central collision detection - it's the heart of the game.
 Its used to calculate, if at specific point of interest
 is a block or free area. */
static unsigned short cg_poi_is_block(int p_index, double p_add_x,
		double p_add_y, int p_direction) {
	double check_x = cg_current_piece.x + (p_index % PIECE_WIDTH) + p_add_x
			- cg_current_piece.sh_data[p_direction]->min_x;

	double check_y = cg_current_piece.y + (p_index / PIECE_WIDTH) + p_add_y
			- cg_current_piece.sh_data[p_direction]->min_y;

	int x = check_x;
	int y = check_y;

	if ((x < 0) || (x >= MAP_WIDTH))
		return TRUE;
	if ((y < 0) || (y >= MAP_HEIGHT))
		return TRUE;

	/* check if in point of interest is a block */
	if (cg_map_data.matrix[y * MAP_WIDTH + x] > FREE) {
		return TRUE;
	}

	return FALSE;
}

/* the central collision detection. the heart of the tetris game.
 * This function checks for collisions between the Piece_t and the map
 * If a collision is detected at all borders, the Piece_t is set to not moveable.
 *
 * This function returns an integer, where several bits are used to
 * mark a collision :
 *
 *  Collision Bit masks
 *
 *  collision below, bit 2 is set
 *  COLL_BELOW  : 4   (binary 100)
 *
 *  collision left, bit 0 is set.
 *  COLL_LEFT   : 1   (binary 001)
 *
 *  collision right, bit 1 is set
 *  COLL_RIGHT  : 2   (binary 010)
 *
 *  collision left+right+below, bit 0,1,2 are set (=7)
 *  COLL_ALL    : 7   (binary 111)
 */
static int cg_collision_detection(int p_direction, double p_add_y) {

	int collision_bits = 0;

	/* This function checks for collisions between the Piece_t and the map.
	 If a collision is detected, the Piece_t is set to not moveable
	 and the unsigned short "1" is returned */

	if (!cg_current_piece.moveable) {
		/* set all status bits in return */
		return COLL_ALL;
	}

	/* unsigned int map_poi_y = 0; */
	int index = 0;

	for (index = 0;
			(index < PIECE_WIDTH * PIECE_HEIGHT) && (collision_bits != COLL_ALL);
			index++) {

		if (cg_current_piece.sh_data[p_direction]->matrix[index] > FREE) {
			/* calculate point of interest in the map
			 dont forget : the Piece_t's x and y coordinates are
			 relative in the maps field. */

			/* check if Piece_t overlaps with another Piece_t in the map */
			if (cg_poi_is_block(index, 0, 0, p_direction)) {
				/* set bit for collision below */
				collision_bits = COLL_ALL;
			}

			/* check below Piece_t for collision */
			if (cg_poi_is_block(index, 0, p_add_y, p_direction)) {
				/* set bit for collision below */
				collision_bits |= COLL_BELOW;
			}
			/* check if on the left side is a block */
			if (cg_poi_is_block(index, -1, p_add_y, p_direction)) {
				/* on the left side is a block */
				collision_bits |= COLL_LEFT;
			}
			/* check if on the right side is a block */
			if (cg_poi_is_block(index, 1, p_add_y, p_direction)) {
				/* on the right side is a block */
				collision_bits |= COLL_RIGHT;
			}
		}
	}
	return collision_bits;
}

/* rotate Piece_t */
static void cg_cur_piece_rotate() {
	gal_piece_rotate(&cg_current_piece);
}

/* calc the Piece_ts current fall */
static double cg_calc_fall() {
	/* measure of current time since programs start */
	double current_time = cou_elapsed_time();
	/* calculate the passed time since last measure in a fraction of a second */
	double second_fraction = (double) (current_time - cg_fall_mea_old_time);
	/* calculate the value of incrementing y */
	double fall_y_axis = cg_current_speed * second_fraction;

	/* save time of last measure */
	cg_fall_mea_old_time = current_time;

	return fall_y_axis;
}

/* Mark a line as an imploding line */
static void cg_let_line_fade(unsigned int p_y) {
	size_t x = 0;

	for (x = 0; x < cg_map_data.width; x++) {
		cg_map_data.matrix[p_y * cg_map_data.width + x] = IMPLODING;
	}
}

/* Check the map for full lines and mark them for imploding */
static void cg_check_full_lines() {
	cg_full_lines_index = 0;

	int map_width = cg_map_data.width;

	size_t x = 0;
	size_t y = 0;

	/* for time measurements where it is needed */
	//double moment_of_interest1;
	for (y = 0; y < cg_map_data.height; y++) {
		unsigned short line_is_full = 1;
		for (x = 0; x < cg_map_data.width; x++) {

			if (cg_map_data.matrix[y * map_width + x] <= FREE) {
				line_is_full = 0;
			}
		}
		if (line_is_full) {
			//moment_of_interest1 = SDL_GetPerformanceCounter();
			cg_let_line_fade(y);
			cg_full_lines[cg_full_lines_index] = y;
			cg_full_lines_index++;
		}
	}
}

/* After a line implodes, all above has to be move down one row. */
static void cg_map_tumble_lines(unsigned int p_above_y) {
	int up_to_index = (p_above_y - 1) * cg_map_data.width + cg_map_data.width;

	if (up_to_index >= MAP_WIDTH * MAP_HEIGHT) {

		printf("up_to_index : out of range [%d]", up_to_index);
		quit_game_with_log_error("index out of range.", 1);
	}

	/* bring a copy from matrix above y to buffer */
	int index_matrix = 0;
	int index_buffer = 0;
	while (index_matrix < up_to_index)
		cg_map_data.buffer[index_buffer++] = cg_map_data.matrix[index_matrix++];

	/* start one line below - and copy back from buffer to matrix */
	index_matrix = cg_map_data.width;
	index_buffer = 0;
	while (index_buffer < up_to_index)
		cg_map_data.matrix[index_matrix++] = cg_map_data.buffer[index_buffer++];
}

/* Mark a line as completely FREE. */
static void cg_map_delete_line(unsigned int p_y) {
	size_t x = 0;

	for (x = 0; x < cg_map_data.width; x++) {
		cg_map_data.matrix[p_y * cg_map_data.width + x] = FREE;
	}
}

/* Let lines listed in array of "full_lines" implode. */
static void cg_implode_full_lines() {
	unsigned int count_lines = 0;

	size_t index = 0;

	for (index = 0; index < MAX_FULL_LINES && cg_full_lines[index] > NO_LINE;
			index++)

			{
		/* mark as free */
		cg_map_delete_line(cg_full_lines[index]);

		/* let all above fall down one row */
		cg_map_tumble_lines(cg_full_lines[index]);

		/* update statistics */
		cg_stats.rows_destroyed++;

		count_lines++;
	}

	int sco = 0;

	/* update the score depending on how many lines where destroyed */
	if (count_lines < MAX_FULL_LINES) {
		sco = count_lines * SCORE_PER_LINE;
	} else if (count_lines == MAX_FULL_LINES) {

		sco += SCORE_FOUR_LINES;
	}

	// update the score statistics
	cg_stats.score += sco;

	// clear the array of "full_lines" now
	cg_clear_full_lines();
}

/* Spawn a new Piece_t by changing its values, not creating a new Piece_t (!!!).
 * This is possible, because a landed Piece_ts becomes part of the map.
 * (Classic "Randomizer" System (NES) --- for details see :
 *  docs/notes/Calculation-of-next-piece.md) */
static void cg_spawn_new_piece(Piece_t *pce) {

	int rand_shape_no = 0;

	char *new_shape_id = NULL;

	// roll a new shape-nr max twice...
	for (int i = 0; i < 2; i++) {
		/* random shape */
		rand_shape_no = rand() % COUNT_SHAPES;
		new_shape_id = gal_shape_ids[rand_shape_no];

		// if new piece is different to last...
		if (rand_shape_no != cg_current_piece.id_no)
			// ... its our new piece...
			break;
		printf("rolled for new piece twice...");
	}

	ShapeNode_t *sn = search_shape(new_shape_id);

	if (sn == NULL) {
		printf("could not find shape with id %s for Piece_t.", new_shape_id);
		quit_game_with_log_error("data management error.", 1);
	}

	/* random color
	 COLOR[0] is reserved for collapsing lines (for now) !!! */
	//int rand_color = rand() % (COUNT_PIECE_COLORS - 1) + 1;
	/* random direction of the shape */
	int rand_direction = rand() % COUNT_DIRECTIONS;

	/* change the values of the Piece_t */
	pce->id_no = rand_shape_no;
	pce->x = SPAWN_X;
	pce->y = SPAWN_Y;
	//pce->color = rand_shape_no;
	pce->direction = rand_direction;
	pce->sh_data = sn->data;
	pce->ready_for_landing = 0;
	pce->moveable = 1;
	pce->display = 1;
}

static void cg_switch_next_piece() {
	/* change the values of the Piece */
	cg_current_piece.id_no = cg_next_piece.id_no;
	cg_current_piece.x = SPAWN_X;
	cg_current_piece.y = SPAWN_Y;
	//cg_current_piece.color = cg_next_piece.color;
	cg_current_piece.direction = cg_next_piece.direction;
	cg_current_piece.sh_data = cg_next_piece.sh_data;
	cg_current_piece.ready_for_landing = 0;
	cg_current_piece.moveable = 1;
	cg_current_piece.display = 1;

	cg_spawn_new_piece(&cg_next_piece);
}

static void cg_drop_piece() {

	double i = 1.0;

	for (i = 1.0; i < MAP_HEIGHT; i += 1.0) {
		int col = cg_collision_detection(cg_current_piece.direction, i);

		if ((col & COLL_BELOW) == COLL_BELOW)
			break;
	}

	cg_current_piece.y += i - 1.0;
	cg_park_piece();
}

/* land the Piece_t when it collides.
 Piece_t will vanish and become part of the map. */
static void cg_park_piece() {
	/* when a Piece_t lands, here, its shape will be drawn NOW
	 AND HERE into the map and afterwards its values will be
	 changed in cg_spawn_new_piece().
	 Iterate the whole matrix of the Piece_t for this. */

	size_t index = 0;

	for (index = 0; index < PIECE_WIDTH * PIECE_HEIGHT; index++) {
		/* if a block is at the current index ... */
		if (cg_current_piece.sh_data[cg_current_piece.direction]->matrix[index]
				> FREE) {
			/* ... calculate the block, which will be parked in the map */
			int park_x =
					cg_current_piece.x + (index % PIECE_WIDTH)
							- cg_current_piece.sh_data[cg_current_piece.direction]->min_x;
			int park_y =
					cg_current_piece.y + (index / PIECE_WIDTH)
							- cg_current_piece.sh_data[cg_current_piece.direction]->min_y;

			/* write the color value into the map */
			cg_map_data.matrix[park_y * MAP_WIDTH + park_x] =
					cg_current_piece.id_no + 1;
		}
	}
	/* update game stats. */
	cg_stats.count_pieces_landed++;

	cg_stats.score += SCORE_PER_PARK;

	/* the Piece_t can't be moved from now on till new spawn */
	cg_current_piece.moveable = 0;
}

/* calculate, if the array "full_lines" contains marked lines for imploding */
static unsigned short cg_found_full_lines() {
	int sum = 0;

	size_t index = 0;

	/* generate the sum of the whole array "full_lines" */
	for (index = 0; index < MAX_FULL_LINES; index++)
		sum += cg_full_lines[index];

	/* if the sum is greater than MAX_FULL_LINES * NO_LINE, not all indexes where marked with NO_LINE,
	 which means there is at least one line marked for imploding.
	 This means, macro NO_LINE always has to be lower than any possible
	 y-index marked here. */
	if (sum > MAX_FULL_LINES * NO_LINE)
		return TRUE;
	else
		return FALSE;
}

static void cg_check_level() {

	int reset_speed = FALSE;

	if ((cg_stats.level <= 6) && (cg_stats.score >= LEVEL6_BORDER)) {
		cg_reset_speed = L7_FALL_PER_SECOND;
		cg_stats.level = 7;
		printf("level=7\n");
		reset_speed = TRUE;
	} else if ((cg_stats.level <= 5) && (cg_stats.score >= LEVEL5_BORDER)) {
		cg_reset_speed = L6_FALL_PER_SECOND;
		cg_stats.level = 6;
		printf("level=6\n");
		reset_speed = TRUE;
	} else if ((cg_stats.level <= 4) && (cg_stats.score >= LEVEL4_BORDER)) {
		cg_reset_speed = L5_FALL_PER_SECOND;
		cg_stats.level = 5;
		printf("level=5\n");
		reset_speed = TRUE;
	} else if ((cg_stats.level <= 3) && (cg_stats.score >= LEVEL3_BORDER)) {
		cg_reset_speed = L4_FALL_PER_SECOND;
		cg_stats.level = 4;
		printf("level=4\n");
		reset_speed = TRUE;
	} else if ((cg_stats.level <= 2) && (cg_stats.score >= LEVEL2_BORDER)) {
		cg_reset_speed = L3_FALL_PER_SECOND;
		cg_stats.level = 3;
		printf("level=3\n");
		reset_speed = TRUE;
	} else if ((cg_stats.level == 1) && (cg_stats.score >= LEVEL1_BORDER)) {
		cg_reset_speed = L2_FALL_PER_SECOND;
		cg_stats.level = 2;
		printf("level=2\n");
		reset_speed = TRUE;
	}

	if (reset_speed) {
		printf("reseting speed\n");
		cg_current_speed = cg_reset_speed;
	}

}

// This function updates the games state in every frame.
static void cg_state_update() {

	static double fall_y = 0;
	static double time_gone = 0;

	// if there are full lines to destroy ...
	if (cg_found_full_lines()) {
		// and the doom-timer till markup of line(s) is over...
		cg_toi = ((SDL_GetPerformanceCounter() - cg_moment_of_interest1)
				/ cou_get_freq()) * SEC_UNIT;
		printf("TOW : %d\n", cg_toi);
		if (cg_toi >= IMPLODING_TIME) {
			// ... let them implode.
			cg_implode_full_lines();

			cg_check_level();
		}
	}

	// calculate how far the piece moved in current time elapsed
	fall_y = cg_calc_fall();
	// calculate the collision info
	cg_collis_det_info = cg_collision_detection(cg_current_piece.direction,
			fall_y);

	// if there is no collision below : move the Piece_t down...
	if ((cg_collis_det_info & COLL_BELOW) != COLL_BELOW) {
		cg_current_piece.ready_for_landing = 0;
		gal_piece_move(&cg_current_piece, 0, fall_y);
	}
	// but, if there was collision below...
	else {
		// TODO : LOGIC FOR LAST MOVE HAS A BUG : SOMETIMES
		// IT DOES NOT MENTION A COLLISION.
		// IS IT BECAUSE THERE IS NO MORE COLLISION DETECTION HERE?!
		// TEST WITH NEW COLLISION DATA AFTER PIECE MARKED FOR
		// LANDING.

		// if the Piece_t is already marked for parking
		if (cg_current_piece.ready_for_landing) {
			// check time gone since ready_for_landing
			cg_moment_of_interest2 = SDL_GetPerformanceCounter();
			time_gone = ((cg_moment_of_interest2 - cg_moment_of_interest1)
					/ cou_get_freq()) * SEC_UNIT;
			printf("time_gone since collision below : %f\n", time_gone);
			// if LAST_MOVE_WAIT ms are gone... */
			if (time_gone > LAST_MOVE_WAIT) {
				printf("waited %f ms.\n", time_gone);
				// ... park the Piece_t in the map - finally ;)
				cg_park_piece();
				cg_check_level();
				// check if there are full lines
				cg_check_full_lines();
				// then, switch to the next Piece.
				cg_switch_next_piece();
			}
		}
		// if the Piece_t is not yet marked for landing but collides...
		else {
			printf("piece ready for landing...\n");
			// mark the Piece_t for landing
			cg_current_piece.ready_for_landing = 1;
			// and measure the time - from now on
			// we have to wait LAST_MOVE_WAIT ms
			cg_moment_of_interest1 = SDL_GetPerformanceCounter();
		}
	}
}

/* The core games main loop, called by the game manager. */
static void cg_main_loop() {
	/* reset the core-game stats and values ... */
	cg_init();

	int limit_fps_flag = LIMIT_FPS;

	cg_moment_of_interest1 = SDL_GetPerformanceCounter();
	cg_moment_of_interest2 = cg_moment_of_interest1;


	printf("entering core loop...\n");
	printf("game state is : %d\n", cg_game_state);

	/* loop until exit game or other main game state */
	while (cg_game_state == ST_CORE_GAME) {

		cou_inc_frame();
		cou_calc_fps();
		if (limit_fps_flag) {
			cou_limit_fps();
		}

		/* check for game over state */
		cg_check_game_over();

		/* if not game over ... */
		if (!cg_game_over_flag) {

			// check keys pressed and dispatch the delivered events
			cg_dispatch_keyboard_events();
			// If the game is not in break state.
			if (!cg_flag_break) {

				cg_state_update();
			}
			/* render all. */
			cg_core_render();
		} else /* if (cg_game_over_flag) */
		{
			/* Change the state of the game to state HALL OF FAME
			 This will end THIS main while-loop in next loop. */
			cg_game_state = ST_HALL_OF_FAME;
		}
	}
	printf("exit core loop.\n");
	printf("game state is : %d.\n", cg_game_state);
}

static void cg_render_next_piece() {

	int color = cg_next_piece.id_no + 1;
	int value_of_shape = 0;

	unsigned int drawx = 0;
	unsigned int drawy = 0;

	ShapeData_t *sh = cg_next_piece.sh_data[cg_next_piece.direction];

	size_t index = 0;

	sdla_printf_tex2(310, 106, 3, "NEXT");
	sdla_printf_tex2(310, 106 + YSPACEING, 3, "PIECE");

	for (index = 0; index < PIECE_WIDTH * PIECE_HEIGHT; index++) {

		drawx = index % PIECE_WIDTH - sh->min_x;
		drawy = index / PIECE_WIDTH - sh->min_y;

		value_of_shape = sh->matrix[index];

		if (value_of_shape > FREE) {
			sdla_draw_block_abs(NEXT_PIECE_X,
			NEXT_PIECE_Y, drawx, drawy, color);
		}
	}
}

/* simply renders the current statistics to screen. */
static void cg_render_stats() {

	int transparency = GLOBAL_FONT_TRANSPARENCY;

	sdla_printf_tex2(922, 113, 3, "STATISTICS");

	// lines
	sdla_printf_tex3(DRAW_STATISTICS_START_X, DRAW_STATISTICS_START_Y, 5,
			transparency, "LINES");
	sdla_printf_tex3(DRAW_STATISTICS_START_X,
	DRAW_STATISTICS_START_Y + YSPACEING, 5, transparency, "%d",
			cg_stats.rows_destroyed);

	// landed pieces
	sdla_printf_tex3(DRAW_STATISTICS_START_X,
	DRAW_STATISTICS_START_Y + STATISTICS_LINE_SPACING * 3, 5, transparency,
			"PIECES");

	sdla_printf_tex3(DRAW_STATISTICS_START_X,
	DRAW_STATISTICS_START_Y + STATISTICS_LINE_SPACING * 4, 5, transparency,
			"%d", cg_stats.count_pieces_landed);

	// score
	sdla_printf_tex3(DRAW_STATISTICS_START_X,
	DRAW_STATISTICS_START_Y + STATISTICS_LINE_SPACING * 5, 5, transparency,
			"SCORE");

	sdla_printf_tex3(DRAW_STATISTICS_START_X,
	DRAW_STATISTICS_START_Y + STATISTICS_LINE_SPACING * 6, 5, transparency,
			"%d", cg_stats.score);

	// level
	sdla_printf_tex3(DRAW_STATISTICS_START_X,
	DRAW_STATISTICS_START_Y + STATISTICS_LINE_SPACING * 8, 5, transparency,
			"LEVEL %d", cg_stats.level);
}

/* Here, the state of the core game will be drawn to screen */
static void cg_core_render() {
	sdla_clear_buffer();

	/* render the ingame background image */
	sdla_render_texture(mt_search_texture(HOOK_INGAME_SCREEN_MASK), 0.0f, 0.0f);

	sdla_printf_tex2(210, 480, 3, HEADLINE1);
	sdla_printf_tex2(210, 480 + YSPACEING, 3, HEADLINE2);

	/* then, add the map ... */
	cg_map_render(&cg_map_data);
	/* ... afterwards render the Piece_t... */
	if (cg_current_piece.display)
		cg_piece_render(&cg_current_piece);

	/* if activated, show fps-counter */
	if (cg_flag_fps) {
		cou_print_fps();
	}
	/* print the current stats. */
	cg_render_stats();

	cg_render_next_piece();
	sdla_present_buffer();
}

/* dispatch the events, delivered by the SDL3-Keyboard-Scan */
static void cg_dispatch_keyboard_events() {

	SDL_Event event;

	if (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT) {
			cg_game_state = ST_EXIT;
		} else if (event.type == SDL_EVENT_KEY_DOWN) {

			printf("CORE-GAME-DISPATCH-EVENT KEYDOWN ::: %d\n", event.key.key);

			// allow this keys only when there is no break...
			if (!cg_flag_break) {
				if (event.key.key == SDLK_SPACE) {
					// check for collision of current and next directions shape
					int next_dir = gal_piece_next_direction(&cg_current_piece);
					int collis_det_info_cur = cg_collision_detection(
							cg_current_piece.direction, 0.0f);
					int collis_det_info_nxt = cg_collision_detection(next_dir,
							0.0f);

					// if no collision with current Piece_t...
					if ((collis_det_info_cur & COLL_ALL) != COLL_ALL) {
						// ... if next shape does not collide ...
						if ((collis_det_info_nxt & COLL_ALL) != COLL_ALL) {
							// Space is pressed : rotate the Piece_t if theres no collision below.
							cg_cur_piece_rotate();
						}
					}

					return;
				}

				if (event.key.key == SDLK_RETURN) {
					cg_drop_piece();
					return;
				}

				if (event.key.key == SDLK_LEFT) {
					// Left arrow is pressed, and left side is not blocked, move Piece_t left
					if ((cg_collis_det_info & COLL_LEFT) != COLL_LEFT) {
						gal_piece_move(&cg_current_piece, -1, 0);
					}
					return;
				}

				if (event.key.key == SDLK_RIGHT) {
					// Right arrow is pressed, and right side is not blocked, move Piece right
					if ((cg_collis_det_info & COLL_RIGHT) != COLL_RIGHT) {
						gal_piece_move(&cg_current_piece, 1, 0);
					}
					return;
				}
				if (event.key.key == SDLK_DOWN) {
					// If Down Arrow is pressed, increment the move factor...
					cg_current_speed = MAX_MOVE_SPEED;
					return;
				}
			}
			if (event.key.key == SDLK_F10) {
				// F10 turns FPS-Counter on/off...
				cg_flag_fps = !cg_flag_fps;
				return;
			}
			if (event.key.key == SDLK_ESCAPE) {
				// Escape key pressed, means in core game : back to main menu
				cg_game_state = ST_MAIN_MENU;

				return;
			}
			if (event.key.key == SDLK_PAUSE) {
				// Escape key pressed, means in core game : back to main menu
				cg_flag_break = !cg_flag_break;
				if (!cg_flag_break) {
					cg_fall_mea_old_time = cou_elapsed_time();
				}

				return;
			}
		} else if (event.type == SDL_EVENT_KEY_UP) {

			printf("CORE-GAME-DISPATCH-EVENT KEYDOWN ::: %d\n", event.key.key);

			if (event.key.key == SDLK_DOWN) {
				cg_current_speed = cg_reset_speed;
			}
		}
	}
}
