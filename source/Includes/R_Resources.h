#ifndef R_RESOURCES_H
#define R_RESOURCES_H 1

#include "D_Common.h"
#include "S_SDL3_Rendering.h"

extern MyGameColor_t COL_WHITE;
extern MyGameColor_t COL_LIGHTBLUE;
extern MyGameColor_t COL_RED;
extern MyGameColor_t COL_YELLOW;
extern MyGameColor_t COL_PURPLE;
extern MyGameColor_t COL_ORANGE;
extern MyGameColor_t COL_GREEN;
extern MyGameColor_t COL_BLUE;

extern MyGameColor_t COL_SEAGREEN;
extern MyGameColor_t COL_CYANBLUE;
extern MyGameColor_t COL_DARKORANGE;
extern MyGameColor_t COL_LIGHTORANGE;


extern MyGameColor_t COL_IMP[];


extern int MAP_SHAPE[MAP_WIDTH * MAP_HEIGHT];

extern int SHAPE_T[COUNT_DIRECTIONS][PIECE_WIDTH * PIECE_HEIGHT];       
extern int SHAPE_L_LEFT[COUNT_DIRECTIONS][PIECE_WIDTH * PIECE_HEIGHT];  
extern int SHAPE_L_RIGHT[COUNT_DIRECTIONS][PIECE_WIDTH * PIECE_HEIGHT]; 
extern int SHAPE_Z_LEFT[COUNT_DIRECTIONS][PIECE_WIDTH * PIECE_HEIGHT];  
extern int SHAPE_Z_RIGHT[COUNT_DIRECTIONS][PIECE_WIDTH * PIECE_HEIGHT]; 
extern int SHAPE_BLOCK[COUNT_DIRECTIONS][PIECE_WIDTH * PIECE_HEIGHT];   
extern int SHAPE_STICK[COUNT_DIRECTIONS][PIECE_WIDTH * PIECE_HEIGHT];   

#endif
