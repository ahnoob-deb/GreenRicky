#include "S_Game_Objects.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
 
#include "D_Common.h"
#include "R_Resources.h"
#include "S_Quit.h"
#include "S_Shape_Manager.h"


char *gal_shape_ids[COUNT_SHAPES] = {
    "SHAPE_T",       // T
    "SHAPE_L_LEFT",  // L
    "SHAPE_L_RIGHT", // J
    "SHAPE_Z_LEFT",  // Z
    "SHAPE_Z_RIGHT", // S
    "SHAPE_BLOCK",   // O
    "SHAPE_STICK"};  // I


/* load a shape into the shape-tree */
static void gal_load_shape(const int *p_shape, char *p_id);

static void gal_shape_update_dimensions(ShapeData_t *p_shape);

static void gal_shape_calc_min_x(ShapeData_t *p_sd);
static void gal_shape_calc_min_y(ShapeData_t *p_sd);

static void gal_shape_calc_max_x(ShapeData_t *p_sd);
static void gal_shape_calc_max_y(ShapeData_t *p_sd);

static void gal_piece_check_map_bounds(Piece_t *p_pce);

static void gal_shape_check_node(char *p_id);

/* allocate memory for a Piece. */
static Piece_t *gal_alloc_piece(void);

/* Allocate memory for a Piece. This will return a pointer to the Piece or NULL
 * in case of error.
 */
static Piece_t *gal_alloc_piece(void)
{
    return (Piece_t *)malloc(sizeof(Piece_t));
}

/* loads all shapes needed in the game into the tree and checks, wether
 * they can be found.
 */
void gal_shape_init()
{
    printf("init shapes...\n");

    gal_load_shape(SHAPE_T[0], "SHAPE_T");

    gal_load_shape(SHAPE_L_LEFT[0], "SHAPE_L_LEFT");
    gal_load_shape(SHAPE_L_RIGHT[0], "SHAPE_L_RIGHT");

    gal_load_shape(SHAPE_Z_LEFT[0], "SHAPE_Z_LEFT");
    gal_load_shape(SHAPE_Z_RIGHT[0], "SHAPE_Z_RIGHT");

    gal_load_shape(SHAPE_BLOCK[0], "SHAPE_BLOCK");

    gal_load_shape(SHAPE_STICK[0], "SHAPE_STICK");

    gal_shape_check_node("SHAPE_T");
    gal_shape_check_node("SHAPE_L_LEFT");
    gal_shape_check_node("SHAPE_L_RIGHT");
    gal_shape_check_node("SHAPE_Z_LEFT");
    gal_shape_check_node("SHAPE_Z_RIGHT");
    gal_shape_check_node("SHAPE_BLOCK");
    gal_shape_check_node("SHAPE_STICK");
}


/* clear the map matrix with the value defined for FREE field */
void gal_map_clear(MapData_t *p_map)
{
    printf("cleanup map...\n");
    memset(p_map->matrix, FREE, MAP_WIDTH * MAP_HEIGHT * sizeof(unsigned int));
}

/* load the map matrix */
static void gal_load_map_matrix(MapData_t *p_map)
{
    int index = 0;

    for (index = 0; index < MAP_WIDTH * MAP_HEIGHT; index++)
    {
        p_map->matrix[index] = MAP_SHAPE[index];
    }
}

/* print map data for debugging purpose. */
static void gal_print_MapData(MapData_t *p_map)
{
    int index = 0;

    printf("map data : ");
    for (index = 0; index < MAP_WIDTH * MAP_HEIGHT; index++)
    {

        printf("%d", p_map->matrix[index]);
    }
    printf("\n");
}

/* load and initialize a map */
int gal_map_init(MapData_t *p_map,
             const unsigned int p_x,
             const unsigned int p_y,
             const unsigned int p_width,
             const unsigned int p_height)
{
    printf("initialising map data...\n");

    /* load initial map values. */

    p_map->x = p_x;
    p_map->y = p_y;

    p_map->width = p_width;
    p_map->height = p_height;

    p_map->color = MAP_COLOR;

    printf("loading map data...\n");

    gal_load_map_matrix(p_map);

    gal_print_MapData(p_map);

    return TRUE;
};


/* Allocate memory for a Piece and init valid values.
 *
 * return : A pointer to the completed Piece.
 *          If there was an error while creating the Piece,
 *          the game will be finished.
 */
Piece_t *gal_piece_new(const unsigned int p_x, const unsigned int p_y,
                 unsigned int p_color,
                 char *p_shape_id, const int p_dir)
{

    Piece_t *pce = gal_alloc_piece();
    if (pce == NULL)
    {
        printf("Could not allocate mem for Piece.");
        quit_game_with_log_error("memory allocation error", 1);
    }

    ShapeNode_t *sn = search_shape(p_shape_id);

    if (sn == NULL)
    {
        printf("could not find shape with id %s for Piece.", p_shape_id);
        quit_game_with_log_error("data management error.", 1);
    }


    pce->id_no=p_color;
    pce->x = p_x;
    pce->y = p_y;
    pce->direction = p_dir;
    pce->sh_data = sn->data;

    pce->moveable = 1;
    pce->ready_for_landing = 0;

    return pce;
}

void gal_piece_init(Piece_t *pce, const unsigned int p_x, const unsigned int p_y,
                unsigned int p_color,
                char *p_shape_id, const int p_dir)
{

    ShapeNode_t *sn = search_shape(p_shape_id);

    if (sn == NULL)
    {
        printf("could not find shape with id %s for Piece.", p_shape_id);
        quit_game_with_log_error("data management error.", 1);
    }

    pce->x = p_x;
    pce->y = p_y;
    pce->id_no = p_color;
    pce->direction = p_dir;
    pce->sh_data = sn->data;

    pce->moveable = 1;
    pce->ready_for_landing = 0;
}

/* This will destroy a given Piece. */
void gal_piece_delete(Piece_t *p_pce)
{

    printf("freeing Piece.");
    free(p_pce);
}


/* Ensure the Piece stays within the boundaries of the game area */
static void gal_piece_check_map_bounds(Piece_t *p_pce)
{
    ShapeData_t *sh = p_pce->sh_data[p_pce->direction];

    /* Prevent moving left out of bounds */
    if (p_pce->x < 0)
        p_pce->x = 0;
    /* Prevent moving up out of bounds */
    if (p_pce->y < 0)
        p_pce->y = 0;
    /* Prevent moving right out of bounds */
    if (p_pce->x + sh->width >= MAP_WIDTH)
        p_pce->x = (MAP_WIDTH - 1) - sh->width;
    /* Prevent moving down out of bounds */
    if (p_pce->y + sh->height >= MAP_HEIGHT)
        p_pce->y = (MAP_HEIGHT - 1) - sh->height;
}

int gal_piece_next_direction(Piece_t *p_pce)
{
    int dir = p_pce->direction;
    dir++;
    dir %= COUNT_DIRECTIONS;

    return dir;
}

/* rotate a given Piece. */
void gal_piece_rotate(Piece_t *p_pce)
{
    if (!p_pce->moveable)
    {
        /* If the Piece is not moveable, do nothing */
        return;
    }

    p_pce->direction = gal_piece_next_direction(p_pce);

    /* check if some part of the Pieces shape is out of bounds */
    gal_piece_check_map_bounds(p_pce);
}

/* move a given Piece in p_dx and p_dy direction. */
void gal_piece_move(Piece_t *p_pce, double p_dx, double p_dy)
{
    if (!p_pce->moveable)
    {
        /* If the Piece is not moveable, do nothing */
        return;
    }

    /* Move the Piece by dx and dy */

    p_pce->x += p_dx;

    p_pce->y += p_dy;

    /* check if some part of the Pieces shape is out of bounds
       or in other Pieces in map. */
    gal_piece_check_map_bounds(p_pce);
}

/* SHAPE */
static void gal_shape_update_dimensions(ShapeData_t *p_shape)
{

    gal_shape_calc_min_x(p_shape);
    gal_shape_calc_min_y(p_shape);

    gal_shape_calc_max_x(p_shape);
    gal_shape_calc_max_y(p_shape);

    p_shape->width = p_shape->max_x - p_shape->min_x;
    p_shape->height = p_shape->max_y - p_shape->min_y;

}

static void gal_shape_calc_min_x(ShapeData_t *p_sd)
{
    int first_block_x = 0;
    unsigned short found = 0;

    int x = 0;
    int y = 0;

    for (x = 0; x < PIECE_WIDTH; x++)
    {

        for (y = 0; y < PIECE_HEIGHT && !found; y++)
        {

            if (p_sd->matrix[y * PIECE_WIDTH + x] > FREE)
            {
                first_block_x = x;
                found = 1;
                break; /* first occurrence - and out... */
            }
        }
    }
    p_sd->min_x = first_block_x;
}

static void gal_shape_calc_min_y(ShapeData_t *p_sd)
{
    int first_block_y = 0;
    unsigned short found = 0;

    int y = 0;
    int x = 0;

    for (y = 0; y < PIECE_HEIGHT && !found; y++)
    {
        for (x = 0; x < PIECE_WIDTH; x++)
        {

            if (p_sd->matrix[y * PIECE_WIDTH + x] > FREE)
            {
                first_block_y = y;
                found = 1;
                break; /* first occurrence - and out... */
            }
        }
    }
    p_sd->min_y = first_block_y;
}

static void gal_shape_calc_max_x(ShapeData_t *p_sd)
{

    /* Following we have to find out, where from the right side
       starting the first BLOCK can be found. This is needed in
       first place in AntrisPiece.move() for border collision check. */

    unsigned short found = 0;
    int first_block_x = 0;

    int x = 0;
    int y = 0;

    for (x = PIECE_WIDTH - 1; x >= 0 && !found; x--)
    {

        for (y = 0; y < PIECE_HEIGHT; y++)
        {

            if (p_sd->matrix[y * PIECE_WIDTH + x] > FREE)
            {
                first_block_x = x;
                found = 1;
                break; /* first occurrence - and out... */
            }
        }
    }
    p_sd->max_x = first_block_x;
}

static void gal_shape_calc_max_y(ShapeData_t *p_sd)
{

    /* Following we have to find out, in which line
       the first BLOCK is found.
       Because a Piece may have a FREE last line,
       the calculation for the borders check in AntrisPiece.move
       would deliver wrong values for hitting the ground, if
       last line of current_shape is free. */

    int first_block_y = 0;

    int index = 0;

    for (index = PIECE_WIDTH * PIECE_HEIGHT - 1; index >= 0; index--)
    {
        if (p_sd->matrix[index] > FREE)
        {
            first_block_y = index / PIECE_WIDTH;
            break; /* first occurrence - and out... */
        }
    }

    p_sd->max_y = first_block_y;
}

/* load a shape into the shape-tree */
static void gal_load_shape(const int *p_shape, char *p_id)
{

    printf("load shape with id [%s] ...\n", p_id);

    ShapeData_t **sd;

    printf("allocating memory for table ... ");

    sd = calloc(COUNT_DIRECTIONS, sizeof(struct ShapeData_t *));

    printf("ok\n");

    int shape_number = 0;

    for (shape_number = 0;
         shape_number < COUNT_DIRECTIONS;
         shape_number++)
    {
        printf("allocating memory for shape ... ");

        sd[shape_number] = calloc(1, sizeof(ShapeData_t));

        printf("ok\n");
    }

    int size_matrix = PIECE_WIDTH * PIECE_HEIGHT;

    printf("loading matrix[%d * %d]", PIECE_WIDTH, PIECE_HEIGHT);
    printf("dimensions-data for shape with id '%s'\n", p_id);

    int index = 0;

    for (shape_number = 0;
         shape_number < COUNT_DIRECTIONS;
         shape_number++)
    {
        for (index = 0; index < size_matrix; index++)
        {
            sd[shape_number]->matrix[index] =
                p_shape[shape_number * size_matrix + index];
        }

        gal_shape_update_dimensions(sd[shape_number]);

        printf("min_x:%d, min_y:%d, max_x:%d, max_y:%d\n", sd[shape_number]->min_x,
                  sd[shape_number]->min_y, sd[shape_number]->max_x, sd[shape_number]->max_y);
    }
    add_shape(p_id, sd);
}

/*  print shape data to console, just for verification here. */
void gal_shape_print(ShapeData_t **p_shape)
{

    printf("print shape : \n");

    int size_matrix = PIECE_WIDTH * PIECE_HEIGHT;

    int shape_number = 0;
    int index = 0;

    for (shape_number = 0;
         shape_number < COUNT_DIRECTIONS;
         shape_number++)
    {
        printf("DIRECTION %d : [", shape_number);

        for (index = 0; index < size_matrix; index++)
        {

            printf("%d", p_shape[shape_number]->matrix[index]);
        }

        printf("]\n");
    }

    printf("\n");
}

/* destroy a whole shape. */
void gal_destroy_shape(ShapeData_t **sd)
{
    int shape_number = 0;

    printf("free shape :\n");
    for (shape_number = 0;
         shape_number < COUNT_DIRECTIONS;
         shape_number++)
    {

        printf("free shape direction #%d...\n", shape_number);

        free(sd[shape_number]);
    }

    free(sd);
}

/* checks if a given shape with id p_id can be found in the shape tree. */
static void gal_shape_check_node(char *p_id)
{

    ShapeNode_t *node;

    node = search_shape(p_id);

    if (node == NULL)
    {
        printf("%s : ", p_id);
        quit_game_with_log_error("shape not found!", 1);
    }

    gal_shape_print(node->data);
}
