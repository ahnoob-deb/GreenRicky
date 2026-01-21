#include "S_SDL3_Rendering.h"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <ctype.h>
#include "D_Common.h"
#include "D_fonts.h"
#include "S_Logging.h"
#include "S_Quit.h"
#include "S_Texture_Manager.h"
#include "R_Resources.h"


static SDL_Renderer *renderer = NULL;
/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static MyGameColor_t piece_col[COUNT_PIECE_COLORS];
static MyGameColor_t map_col[COUNT_MAP_COLORS];
static MyGameColor_t imp_col[COUNT_IMP_COLORS];
static void init_colors(void);

/* Buffer size for debug strings */
#define SIZE_TXT_BUFFER 2000

/* Textures for blocks and free area */
static MyGameTexture_t *gfx_piece_block01;
static MyGameTexture_t *gfx_free_cell01;

/* Fonts textures */
static MyGameTexture_t *gfx_font01;

/* returns the used renderer */
SDL_Renderer* sdla_get_renderer() {
	return renderer;
}

void sdla_render_rect_from_tex(MyGameTexture_t *p_tex, float p_sx1, float p_sy1,
		float p_sx2, float p_sy2, float p_dx1, float p_dy1, float p_dx2,
		float p_dy2) {
	SDL_FRect srcRect = { p_sx1, p_sy1, p_sx2 - p_sx1, p_sy2 - p_sy1 };
	SDL_FRect destRect = { p_dx1, p_dy1, p_dx2 - p_dx1, p_dy2 - p_dy1 };

	unsigned short success = SDL_RenderTexture(renderer, p_tex->texture,
			&srcRect, &destRect);

	if (!success) {
		printf("Could not render rectangle portion from texture %s.\n",
				p_tex->id);
		SDL_Log("Error rendering texture portion: %s", SDL_GetError());
		quit_game_with_log_error("render error.", 1);
	}
}

static void init_colors() {

	piece_col[0]=COL_WHITE;
	piece_col[1]=COL_LIGHTBLUE;
	piece_col[2]=COL_RED;
	piece_col[3]=COL_YELLOW;
	piece_col[4]=COL_PURPLE;
	piece_col[5]=COL_ORANGE;
	piece_col[6]=COL_GREEN;
	piece_col[7]=COL_BLUE;

	map_col[0].R = 0x33;
	map_col[0].G = 0x7c;
	map_col[0].B = 0x35;
	map_col[0].A = 0xFF;

	map_col[1].R = 27;
	map_col[1].G = 30;
	map_col[1].B = 25;
	map_col[1].A = 0xFF;

	map_col[2].R = 47;
	map_col[2].G = 79;
	map_col[2].B = 32;
	map_col[2].A = 0xFF;

	map_col[3].R = 32;
	map_col[3].G = 46;
	map_col[3].B = 79;
	map_col[3].A = 0xFF;

	map_col[4].R = 26;
	map_col[4].G = 36;
	map_col[4].B = 36;
	map_col[4].A = 0xFF;

	map_col[5].R = 0xFF;
	map_col[5].G = 0xFF;
	map_col[5].B = 0xFF;
	map_col[5].A = 0xFF;
}


void sdla_render_texture_mod(MyGameTexture_t *p_tex, float p_x, float p_y,
		MyGameColor_t *col) {

	if (p_tex != NULL) {

	SDL_FRect dst_rect;

	dst_rect.x = p_x;
	dst_rect.y = p_y;
	dst_rect.w = (float) p_tex->width;
	dst_rect.h = (float) p_tex->height;

	SDL_SetTextureBlendMode(p_tex->texture, SDL_BLENDMODE_BLEND );
	SDL_SetTextureAlphaMod(p_tex->texture, col->A);
	SDL_SetTextureColorMod(p_tex->texture, col->R,
			col->G, col->B);

	SDL_RenderTexture(renderer, p_tex->texture, NULL, &dst_rect);
	} else {
		printf("WARNING : p_tex is NULL : no Rendering!");
	}
}

void sdla_render_texture(MyGameTexture_t *p_tex, float p_x, float p_y) {

	if (p_tex!=NULL) {

	SDL_FRect dst_rect;

	dst_rect.x = p_x;
	dst_rect.y = p_y;
	dst_rect.w = (float) p_tex->width;
	dst_rect.h = (float) p_tex->height;

	SDL_RenderTexture(renderer, p_tex->texture, NULL, &dst_rect);
	} else {
		printf("WARNING : p_tex is NULL : no Rendering!");
	}
}

int gfx_init_hooks() {
	gfx_font01 = mt_search_texture(HOOK_FONT_DATA_01);
	gfx_free_cell01 = mt_search_texture(HOOK_CELL_FREE_MASK);
	gfx_piece_block01 = mt_search_texture(HOOK_CELL_BLOCK_MASK);

	return TRUE;
}

int sdla_boot_mmAPI_SDL() {

	SDL_SetAppMetadata("Cetris", "0.1.0", "Cetris");

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
		return FALSE;
	}

	if (!SDL_CreateWindowAndRenderer("Cetris", WINDOW_WIDTH, WINDOW_HEIGHT, 0,
			&window, &renderer)) {
		SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
		return FALSE;
	}

	init_colors();

	return TRUE;
}

void sdla_clear_buffer() {

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE); /* black, full alpha */
	SDL_RenderClear(renderer); /* start with a blank canvas. */
}

void sdla_present_buffer() {
	SDL_RenderPresent(renderer); /* put it all on the screen! */
}

void sdla_draw_block_abs(size_t p_x, size_t p_y, size_t p_pm_x, size_t p_pm_y,
		int p_color) {
	int block_size = BLOCK_SIZE_PX;

	float drawx = p_pm_x * block_size;
	float drawy = p_pm_y * block_size;

	sdla_render_texture_mod(gfx_piece_block01, p_x + drawx, p_y + drawy,
			&piece_col[p_color]);
}

void sdla_draw_imp_block(double p_x, double p_y, unsigned int p_color) {
	int block_size = BLOCK_SIZE_PX;

	float drawx = p_x * block_size;
	float drawy = p_y * block_size;

	sdla_render_texture_mod(gfx_piece_block01, DRAW_START_X + drawx,
	DRAW_START_Y + drawy, &imp_col[p_color]);
}

void sdla_draw_block(double p_x, double p_y, unsigned int p_color) {
	int block_size = BLOCK_SIZE_PX;

	float drawx = p_x * block_size;
	float drawy = p_y * block_size;

	sdla_render_texture_mod(gfx_piece_block01, DRAW_START_X + drawx,
	DRAW_START_Y + drawy, &piece_col[p_color]);
}

void sdla_draw_free(double p_x, double p_y, unsigned int p_color) {
	int block_size = BLOCK_SIZE_PX;

	float drawx = p_x * block_size;
	float drawy = p_y * block_size;

	sdla_render_texture_mod(gfx_free_cell01, DRAW_START_X + drawx,
	DRAW_START_Y + drawy, &piece_col[p_color]);
}

int sdla_process_events() {

	SDL_Event event;

	if (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT) {
			return EV_INSTANT_QUIT;
		} else if (event.type == SDL_EVENT_KEY_DOWN) {

			return(event.key.key);
		}
	}
	return EV_NO_EVENT;
}

void sdla_printf(const int p_x, const int p_y, const int p_color,
		const char *p_message, ...) {
	char buffer_text[SIZE_TXT_BUFFER] = { 0 };

	va_list args;
	va_start(args, p_message);

	vsprintf(buffer_text, p_message, args);

	va_end(args);

	const char *message = buffer_text;

	SDL_SetRenderDrawColor(renderer, 0, 100 + p_color * 10, 100, 255);

	SDL_RenderDebugText(renderer, p_x, p_y, message);
}

void sdla_printf_tex(const int p_x, const int p_y, unsigned int p_col,
		const char *p_message, ...) {
	char buffer_text[SIZE_TXT_BUFFER] = { 0 };

	va_list args;
	va_start(args, p_message);

	vsprintf(buffer_text, p_message, args);

	va_end(args);

	const char *message = buffer_text;

	unsigned int r = (p_col >> 16) & 0xFF;
	unsigned int g = (p_col >> 8) & 0xFF;
	unsigned int b = p_col & 0xFF;

	SDL_SetTextureColorMod(gfx_font01->texture, r, g, b);

	size_t index = 0;
	int x = 0;
	int y = 0;
	int draw_start_x = p_x;
	int no = 0;

	for (index = 0; index < strlen(message); index++) {

		int ch = message[index];
		if (isalnum(ch)) {
			if (isdigit(ch)) {
				int no = ch - '0';
				x = no * LETTER_MATRIX_WIDTH;
				y = 0;

				sdla_render_rect_from_tex(gfx_font01, x, y,
						x + LETTER_MATRIX_WIDTH, y + LETTER_MATRIX_HEIGHT,
						draw_start_x, p_y, draw_start_x + LETTER_MATRIX_WIDTH,
						p_y + LETTER_MATRIX_HEIGHT);

				draw_start_x += SPACEING;
			} else if (isalpha(ch)) {
				if (islower(ch)) {
					no = ch - 'a';
					y = LETTER_MATRIX_HEIGHT * 1;
				} else if (isupper(ch)) {
					no = ch - 'A';
					y = LETTER_MATRIX_HEIGHT * 2;
				}

				x = no * LETTER_MATRIX_WIDTH;

				sdla_render_rect_from_tex(gfx_font01, x, y,
						x + LETTER_MATRIX_WIDTH, y + LETTER_MATRIX_HEIGHT,
						draw_start_x, p_y, draw_start_x + LETTER_MATRIX_WIDTH,
						p_y + LETTER_MATRIX_HEIGHT);

				draw_start_x += SPACEING;
			}
		} else if (isspace(ch)) {
			draw_start_x += SPACEING;
		}
	}
}
