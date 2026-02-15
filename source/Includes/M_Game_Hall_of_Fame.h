/***********************************************************************
 * This is the interface for the Module for :
 *   * showing the data of the highscore-table
 *   * managing the highscore-table
 *   * rendering the highscore-table to the screen
 *
 **********************************************************************/

#ifndef M_GAME_HALL_OF_FAME_H
#define M_GAME_HALL_OF_FAME_H 1

/* init's the hall of fame and starts the main-loop.
 * Main entry point of all users of this module.
 */
void hof_run(unsigned int p_score);

/* Returns the state of the hof-module. */
int hof_get_state(void);

#endif
