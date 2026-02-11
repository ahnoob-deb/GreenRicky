/***********************************************************************
 * This is the service "FPS-Counter"
 *
 * It measures the time between two frames(frametime)
 * and remembers the number of frames generated
 * in last second gone.
 *
 * If you want you can limit to a specific target fps
 * by using function void cou_limit_fps().
  **********************************************************************/

#include "D_Common.h"
#include "S_FPS_Counter.h"
#include "S_SDL3_Rendering.h"

/***************************************************/
/* FPS DATA                                        */
/***************************************************/

static int cou_fps = 0; // contains the frames per second
static double cou_frametime_ms_shown = 0.0f; // frametime in ms
static unsigned int cou_frame = 0; // frame-counter
static unsigned int cou_target_fps = TARGET_FPS; // targeted max. fps

/***************************************************/
/*   TIME MEASUREMENT                              */
/***************************************************/

static Uint64 cou_freq = 0;  // count per second of the high resolution counter.
static Uint64 cou_start = 0; // timestamp of starting time
static Uint64 cou_now = 0;   // timestamp of NOW
static double cou_delta_time_start = 0.0f;  // the difference between now and the timestamp of the starting time.
static double cou_frametime = 0.0f;  // time gone since last frame
static int cou_sec = 0;  // current second
static int cou_oldsec = 0;  // second of last FPS-measure
static int cou_unit = 1000; // 1000 = milliseconds; 1000000 would be nanoseconds for example

/**************************************************************************
 *
 * Returns the count per second of the high resolution counter.
 *
 **************************************************************************/
Uint64 cou_get_freq(void) {
	return cou_freq;
}

/**************************************************************************
 *
 * Increment the frame counter.
 *
 **************************************************************************/
void cou_inc_frame() {
	cou_frame++;
}

/**************************************************************************
 *
 * Returns the number of frames per second.
 *
 **************************************************************************/
int cou_get_fps() {
	return cou_fps;
}

/**************************************************************************
 *
 * Get the timestamp of starting time.
 *
 **************************************************************************/
Uint64 cou_get_start_ts() {
	return cou_start;
}

/**************************************************************************
 *
 * Get the elapsed time since timestamp of starting time.
 *
 **************************************************************************/
double cou_elapsed_time() {
	return (double) (SDL_GetPerformanceCounter() - cou_start) / (double) cou_freq;
}

/**************************************************************************
 *
 * Init the Frequency and the timestamp of starting time.
 *
 **************************************************************************/
void cou_init() {
	// Get the count per second of the high resolution counter.
	cou_freq = SDL_GetPerformanceFrequency();

	// remember the start time of the core game
	cou_start = SDL_GetPerformanceCounter();
}

/**************************************************************************
 *
 * Draws the frames per second to screen using the SDL wrapper module.
 *
 **************************************************************************/
void cou_print_fps() {

	sdla_printf_tex3(FPS_POS_X, FPS_POS_Y, 3, GLOBAL_FONT_TRANSPARENCY, "FPS : %d", cou_fps);
}

/**************************************************************************
 *
 * Module Core : Measurement of the frames per second.
 *
 **************************************************************************/
void cou_calc_fps() {
	cou_now = SDL_GetPerformanceCounter();

	// calculate the difference between now and the timestamp of the starting time.
	cou_delta_time_start = (double) (cou_now - cou_start) / (double) cou_freq;
	// calculate the difference between now and the last frame
//	cou_delta_time_frame = (double) (cou_now - cou_frame_now) / (double) cou_freq;

	// truncate decimal places of the time elapsed
	cou_sec = (int) cou_delta_time_start;

	// if there is one more second gone...
	if (cou_sec >= cou_oldsec + 1) {
		// calculate the current frametime in seconds (~ 0.016667 sec.)
		cou_frametime = (double) ((1.0f / (double) cou_frame));
		// frametime shown in ms (~ 16.67 ms)
		cou_frametime_ms_shown = cou_frametime * cou_unit;
	    // remember the current second.
		cou_oldsec = cou_sec;
		// reset the second counter.
		cou_sec = 0;
		// current FPS = counted frames in last second
		cou_fps = cou_frame;
		// reset frame-counter...
		cou_frame = 0;
	}
}

/**************************************************************************
 *
 * optional: Limit the FPS and CPU-usage.
 *
 **************************************************************************/
void cou_limit_fps() {
	const float TARGET_FRAMETIME = 1000.0f / cou_target_fps; // 16.67 ms at 60 FPS
	if (cou_frametime < TARGET_FRAMETIME) {
		SDL_Delay((Uint32) (TARGET_FRAMETIME - cou_frametime));
	}
}
