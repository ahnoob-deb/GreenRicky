#ifndef INCLUDES_S_FPS_COUNTER_H_
#define INCLUDES_S_FPS_COUNTER_H_

#include <SDL3/SDL.h>

void cou_limit_fps(void);
void cou_calc_fps(void);
void cou_init(void);
Uint64 cou_get_start_ts(void);
int cou_get_fps(void);
double cou_elapsed_time(void);
void cou_inc_frame(void);
Uint64 cou_get_freq(void);


#endif /* INCLUDES_S_FPS_COUNTER_H_ */
