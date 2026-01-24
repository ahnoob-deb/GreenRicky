#include "S_Quit.h"

#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>

#include "S_SDL3_Rendering.h"
#include "S_Shape_Manager.h"
#include "S_Texture_Manager.h"

static void cleanup_game(void);

static void cleanup_game()
{

    printf("shutting down...\n");
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

void quit_game_with_log_error(char *p_message, int p_exitcode)
{
    printf("%s\n", p_message);
    //printf("counted %d errors.\n", get_count_errors());
    cleanup_game();
    exit(p_exitcode);
}

void prepare_quit_game()
{   printf("shutting down now...\n");
    cleanup_game();
}
