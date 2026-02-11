/***********************************************************************
 * This is the interface of the Service "FPS-Counter" of the game.
 **********************************************************************/
#ifndef INCLUDES_S_FPS_COUNTER_H_
#define INCLUDES_S_FPS_COUNTER_H_

/* SDL.h : We need Uint64 here */
#include <SDL3/SDL.h>

/* position of the fps counter. */
#define FPS_POS_X 900
#define FPS_POS_Y 50

/* optional: Limit the FPS and CPU-usage. */
void cou_limit_fps(void);
/* Module Core : Measurement of the frames per second. */
void cou_calc_fps(void);
/* Init the Frequency and the timestamp of starting time. */
void cou_init(void);
/* Get the timestamp of starting time. */
Uint64 cou_get_start_ts(void);
/* Returns the number of frames per second. */
int cou_get_fps(void);
/* Get the elapsed time since timestamp of starting time. */
double cou_elapsed_time(void);
/* Increment the frame counter. */
void cou_inc_frame(void);
/* Returns the count per second of the high resolution counter. */
Uint64 cou_get_freq(void);
/* Draws the frames per second to screen using the SDL wrapper module. */
void cou_print_fps(void);


#endif /* INCLUDES_S_FPS_COUNTER_H_ */
