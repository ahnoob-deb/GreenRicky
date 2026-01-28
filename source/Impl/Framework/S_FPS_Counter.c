#include "D_Common.h"

#include "S_FPS_Counter.h"

#include <SDL3/SDL.h>

/***************************************************/
/* FPS DATA                                        */
/***************************************************/

static int cou_fps = 0;
static double cou_frametime_ms_shown = 0.0f;
static unsigned int cou_frame = 0;
static unsigned int cou_target_fps = TARGET_FPS;

/***************************************************/
/*   TIME MEASUREMENT                              */
/***************************************************/

static Uint64 cou_freq = 0;
static Uint64 cou_start = 0;
//static Uint64 cou_last = 0;
static Uint64 cou_now = 0;
static Uint64 cou_frame_now = 0;
static double cou_delta_time_start = 0.0f;
static double cou_delta_time_frame = 0.0f;
static double cou_frametime = 0.0f;
static int cou_sec = 0;
static int cou_oldsec = 0;
static int cou_unit = 1000;

Uint64 cou_get_freq(void) {
	return cou_freq;
}

void cou_inc_frame() {
	cou_frame++;
}

int cou_get_fps() {
	return cou_fps;
}

Uint64 cou_get_start_ts() {
	return cou_start;
}

double cou_elapsed_time() {
	return (double) (SDL_GetPerformanceCounter() - cou_start) / (double) cou_freq;
}

void cou_init() {
	cou_freq = SDL_GetPerformanceFrequency();

	/* save the start time of the core game */
	cou_start = SDL_GetPerformanceCounter();
}



void cou_calc_fps() {
	cou_now = SDL_GetPerformanceCounter();

	cou_delta_time_start = (double) (cou_now - cou_start) / (double) cou_freq;
	cou_delta_time_frame = (double) (cou_now - cou_frame_now) / (double) cou_freq;

	cou_sec = (int) cou_delta_time_start;

	if (cou_sec >= cou_oldsec + 1) {
		cou_frametime = (double) ((1.0f / (double) cou_frame));
		cou_frametime_ms_shown = cou_frametime * cou_unit;
		cou_oldsec = cou_sec;
		cou_sec = 0;
		cou_fps = cou_frame;
		cou_frame = 0;
	}
}

void cou_limit_fps() {
	// optional: limit frame rate
	const float TARGET_FRAMETIME = 1000.0f / cou_target_fps; // 16.67 ms bei 60 FPS
	if (cou_frametime < TARGET_FRAMETIME) {
		SDL_Delay((Uint32) (TARGET_FRAMETIME - cou_frametime));
	}
}
