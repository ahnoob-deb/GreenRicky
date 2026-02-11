/***********************************************************************
 * This is the interface for the service "Quit" of the game.
 * It defines functions for orderly termination of the program
 * in every case of error.
 **********************************************************************/

#ifndef KRN_QUIT_H
#define KRN_QUIT_H 1

/* Displays an error and terminates the program. */
void quit_game_with_log_error(char *p_message, int p_exitcode);
/* Prepares the program for orderly termination. */
void prepare_quit_game(void);

#endif
