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
static MyGameColor_t text_col[COUNT_TEXT_COLORS];
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

/* returns the used renderer */
SDL_Window* sdla_get_window() {
	return window;
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

	text_col[0] = COL_WHITE;          // RESERVED
	text_col[1] = COL_LIGHTBLUE;
	text_col[2] = COL_RED;
	text_col[3] = COL_YELLOW;
	text_col[4] = COL_PURPLE;
	text_col[5] = COL_ORANGE;
	text_col[6] = COL_GREEN;
	text_col[7] = COL_BLUE;

	piece_col[0] = COL_WHITE;         // RESERVED
	piece_col[1] = COL_SEAGREEN;      // T
	piece_col[2] = COL_RED;           // L
	piece_col[3] = COL_CYANBLUE;      // J
	piece_col[4] = COL_LIGHTORANGE;   // Z
    piece_col[5] = COL_PURPLE;        // S
	piece_col[6] = COL_YELLOW;        // O
	piece_col[7] = COL_DARKORANGE;    // I


	imp_col[0] = COL_IMP[0];
	imp_col[1] = COL_IMP[1];
	imp_col[2] = COL_IMP[2];

}

void sdla_render_texture_mod(MyGameTexture_t *p_tex, float p_x, float p_y,
		MyGameColor_t *col, int p_alpha) {

	if (p_tex != NULL) {

		SDL_FRect dst_rect;

		dst_rect.x = p_x;
		dst_rect.y = p_y;
		dst_rect.w = (float) p_tex->width;
		dst_rect.h = (float) p_tex->height;

		SDL_SetTextureBlendMode(p_tex->texture, SDL_BLENDMODE_BLEND);
		SDL_SetTextureAlphaMod(p_tex->texture, p_alpha);
		SDL_SetTextureColorMod(p_tex->texture, col->R, col->G, col->B);

		SDL_RenderTexture(renderer, p_tex->texture, NULL, &dst_rect);
	} else {
		printf("WARNING : p_tex is NULL : no Rendering!");
	}
}

void sdla_render_texture(MyGameTexture_t *p_tex, float p_x, float p_y) {

	if (p_tex != NULL) {

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

	SDL_Init(SDL_INIT_VIDEO);

	SDL_WindowFlags flags=0;

	if (FULLSCREEN) flags |= SDL_WINDOW_FULLSCREEN;

	window = SDL_CreateWindow(WORKINGNAME, WINDOW_WIDTH, WINDOW_HEIGHT, flags);

	if (!window) {
		SDL_Log("Error creating window: %s", SDL_GetError());
		return FALSE;
	}

	//SDL_SetHint(SDL_HINT_RENDER_DRIVER, "direct3d");
	renderer = SDL_CreateRenderer(window, NULL);

	if (!renderer) {
		SDL_Log("Error creating renderer: %s", SDL_GetError());
		return FALSE;
	}

	if (FULLSCREEN) {

		SDL_DisplayMode mode;
		mode.w = RES_WIDTH;
		mode.h = RES_HEIGHT;
		mode.refresh_rate = 0; // Use default
		mode.pixel_density = 1.0f;
		mode.format = SDL_PIXELFORMAT_UNKNOWN;

		// Set the desired mode
		//SDL_SetWindowFullscreenMode(window, &mode);

		// Switch to fullscreen
		//SDL_SetWindowFullscreen(window, true);

		SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
		SDL_SetWindowFullscreenMode(window, &mode);

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
			&piece_col[p_color], ALPHA_SOLID);
}

void sdla_draw_imp(double p_x, double p_y, int imp_val, unsigned int p_alpha) {
	int block_size = BLOCK_SIZE_PX;

	float drawx = p_x * block_size;
	float drawy = p_y * block_size;

	sdla_render_texture_mod(gfx_piece_block01, DRAW_START_X + drawx,
	DRAW_START_Y + drawy, &imp_col[imp_val], p_alpha);
}

void sdla_draw_block(double p_x, double p_y, unsigned int p_color) {
	int block_size = BLOCK_SIZE_PX;

	float drawx = p_x * block_size;
	float drawy = p_y * block_size;

	sdla_render_texture_mod(gfx_piece_block01, DRAW_START_X + drawx,
	DRAW_START_Y + drawy, &piece_col[p_color], ALPHA_SOLID);
}

void sdla_draw_free(double p_x, double p_y, unsigned int p_color) {
	int block_size = BLOCK_SIZE_PX;

	float drawx = p_x * block_size;
	float drawy = p_y * block_size;

	sdla_render_texture_mod(gfx_free_cell01, DRAW_START_X + drawx,
	DRAW_START_Y + drawy, &piece_col[p_color], ALPHA_SOLID);
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

void sdla_printf_tex2(const int p_x, const int p_y, unsigned int p_col,
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

	size_t index = 0;
	//int x = 0;
	//int y = 0;
	int draw_start_x = p_x;
	//int no = 0;
	char l_image_id[255];
	int draw = FALSE;

	for (index = 0; index < strlen(message); index++) {
		draw = FALSE;

		int ch = message[index];
		if (isdigit(ch)) {
			//int no = ch - '0';

			sprintf(l_image_id, "num0%d.png", ch);
			//printf("tex-id is : %s\n", l_image_id);

			draw = TRUE;

		} else if (isalpha(ch)) {
			int no = toupper(ch);

			sprintf(l_image_id, "font0%d.png", no);
			//printf("tex-id is : %s\n", l_image_id);
			draw = TRUE;

		} else if (isspace(ch)) {
			draw_start_x += SPACEING;
			draw = FALSE;
		}

		if (draw) {

			MyGameTexture_t *tex = mt_search_texture(l_image_id);

			SDL_SetTextureColorMod(tex->texture, r, g, b);

			sdla_render_texture_mod(tex, draw_start_x, p_y, &text_col[p_col],
			ALPHA_SOLID);
			draw_start_x += tex->texture->w;
		}

	}
}

