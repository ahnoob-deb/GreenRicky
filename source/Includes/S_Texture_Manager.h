#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H 1

#include <SDL3/SDL.h>

#define MAX_ID_LENGTH 255

/* MAIN MENU TEXTURE-FILES AND HOOK-IDs */
#define FILE_MMENU_SCREEN_MASK_OFF "../data/pics/masks/Cetris-mmenu-alpha-OFF-001.png"
#define HOOK_MMENU_SCREEN_MASK_OFF "HOOK_MM_SCR_MSK_OFF_TEX01"
#define FILE_MMENU_SCREEN_MASK_ON "../data/pics/masks/Cetris-mmenu-alpha-ON-001.png"
#define HOOK_MMENU_SCREEN_MASK_ON "HOOK_MM_SCR_MSK_ON_TEX01"

/* INGAME TEXTURE-FILES AND HOOK-IDs */
#define FILE_INGAME_SCREEN_MASK "../data/pics/masks/Cetris-ingame-alpha001.png"
#define HOOK_INGAME_SCREEN_MASK "HOOK_CG_SCR_MSK_BG_TEX01"
#define FILE_CELL_BLOCK_MASK "../data/pics/blocks/cetris-block20x20.png"
#define HOOK_CELL_BLOCK_MASK "HOOK_CG_CELL_BLOCK_TEX01"
#define FILE_CELL_FREE_MASK "../data/pics/blocks/cetris-free-block20x20.png"
#define HOOK_CELL_FREE_MASK "HOOK_CG_CELL_FREE_TEX01"

#define FILE_CELL_BLOCK_MASK_ALPHA "../data/pics/blocks/cetris-block20x20.png"
#define HOOK_CELL_BLOCK_MASK_ALPHA "HOOK_CG_CELL_BLOCK_ALPHA_100_TEX01"

/* HALL OF FAME TEXTURE-FILES AND HOOK-IDs */
#define FILE_HOF_SCREEN_MASK "../data/pics/masks/Cetris-hof-alpha001.png"
#define HOOK_HOF_SCREEN_MASK "HOOK_HOF_SCR_MSK_TEX01"

/* FONTS TEXTURE-FILES AND HOOK-IDs */
#define FILE_FONT_DATA_01 "../data/fonts/FONT-CANTARELL-REGULAR-7-BOLD.png"
#define HOOK_FONT_DATA_01 "HOOK_FONTS_DATA_TEX01"


typedef struct MyGameTexture
{
  char *id;
  SDL_Texture *texture;
  int width;
  int height;
  struct MyGameTexture *left;
  struct MyGameTexture *right;

} MyGameTexture_t;

int mt_load_texture(char *p_filename, char *p_tex_hook_id,MyGameTexture_t **p_hook);
MyGameTexture_t *mt_add_texture(char p_id[], SDL_Texture *p_tex, int p_width, int p_height);
MyGameTexture_t *mt_search_texture(char p_id[]);


void mt_init(void);
void mt_destroy_texture_tree(void);

#endif
