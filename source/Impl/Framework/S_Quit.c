/***********************************************************************
 * This is the service "Quit" of the game. It defines functions for
 * orderly termination of the program in every case of error.
 **********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>

#include "S_Quit.h"
#include "S_SDL3_Rendering.h"
#include "S_Shape_Manager.h"
#include "S_Texture_Manager.h"

/* cleans up all data in the game */
static void cleanup_game(void);


/**************************************************************************
 *
 * Cleans up all data in the game.
 * Cannot be called directly.
 *
 **************************************************************************/
static void cleanup_game()
{

    printf("cleaning up now...\n");
    destroy_shape_tree();
    mt_destroy_texture_tree();
    printf("destroying Renderer...");
    SDL_DestroyRenderer(sdla_get_renderer());
    printf("done\n");
    printf("destroying Window...");
    SDL_DestroyWindow(sdla_get_window());
    printf("done\n");
    printf("Quit SDL...");
    SDL_Quit();
    printf("done\n");
}

/**************************************************************************
 *
 * Displays an error and terminates the program with a specific
 * exitcode in an orderly way.
 *
 * Should be called in case of major error to terminate the program.
 *
 **************************************************************************/
void quit_game_with_log_error(char *p_message, int p_exitcode)
{
    printf("%s\n", p_message);
    prepare_quit_game();
    exit(p_exitcode);
}

/**************************************************************************
 *
 * Prepares the program for orderly termination.
 *
 **************************************************************************/
void prepare_quit_game()
{   printf("prepare shutdown now...\n");
    cleanup_game();
    printf("shutdown complete.\n");
}
