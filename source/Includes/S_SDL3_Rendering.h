#ifndef KO_GFX_SDL3_H
#define KO_GFX_SDL3_H 1

#include "S_Texture_Manager.h"

typedef struct {
	unsigned int R;
	unsigned int G;
	unsigned int B;
	unsigned int A;
} MyGameColor_t;

#define COUNT_IMP_COLORS 3

int gfx_init_hooks(void);

void sdla_render_texture_mod(MyGameTexture_t *p_tex, float p_x, float p_y,
		MyGameColor_t *col, int p_alpha);
void sdla_render_texture(MyGameTexture_t *p_tex, float p_x, float p_y);
SDL_Window* sdla_get_window(void);
SDL_Renderer* sdla_get_renderer(void);
int sdla_boot_mmAPI_SDL(void);
int sdla_process_events(void);
void sdla_clear_buffer(void);
void sdla_present_buffer(void);
void sdla_draw_imp(double p_x, double p_y, int imp_val, unsigned int p_alpha);
void sdla_draw_block(double p_x, double p_y, unsigned int p_color);
void sdla_draw_block_abs(size_t p_x, size_t p_y, size_t p_pm_x, size_t p_pm_y,
		int p_color);
void sdla_draw_free(double p_x, double p_y, unsigned int p_color);
void sdla_printf(const int p_x, const int p_y, const int p_color,
		const char *p_message, ...);

void sdla_printf_tex(const int p_x, const int p_y, unsigned int p_col,
		const char *p_message, ...);

void sdla_render_rect_from_tex(MyGameTexture_t *p_tex, float p_sx1, float p_sy1,
		float p_sx2, float p_sy2, float p_dx1, float p_dy1, float p_dx2,
		float p_dy2);

#endif
