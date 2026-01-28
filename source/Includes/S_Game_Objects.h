#ifndef S_GAME_OBJECTS_H
#define S_GAME_OBJECTS_H 1

#include "D_Common.h"

/* SHAPE */

/* ShapeData : a structure which holds the information of 
 * a shape of a Piece. (0,0) to (max_x, max_y) defines the 
 * the small as possible rectangle, where the shape fits in. 
 * The field matrix contains whole the definition of the shape. 
 */
typedef struct
{
    int min_x;
	int min_y;
	int max_x;
	int max_y;
    int width;
	int height;
    int matrix[PIECE_WIDTH * PIECE_HEIGHT];
} ShapeData_t;

/* ShapeNode(struct shape_node) : a recursive structure, which represents a 
 * node in the tree of shapes. Each ShapeNode is identified by an
 * id-string. ShapeData **data is a pointer to a pointer of ShapeData.
 * left and right pointers lead to child-nodes in the tree. 
 */
typedef struct shape_node
{
    char *id;
    ShapeData_t **data;
    struct shape_node *left;
    struct shape_node *right;
} ShapeNode_t;

void gal_shape_init(void);
void gal_shape_print(ShapeData_t **p_shape);
void gal_destroy_shape(ShapeData_t **sd);


/* PIECE */

typedef struct
{

    int id_no;
	double x;
    double y;
    ShapeData_t **sh_data;
    int direction;
    unsigned short display;
    unsigned short moveable;
    unsigned short ready_for_landing;
    unsigned int color;

} Piece_t;

Piece_t *gal_piece_new(const unsigned int p_x, const unsigned int p_y,
                 unsigned int p_color,
                 char *p_shape_id, const int p_dir);

void gal_piece_init(Piece_t *pce,const unsigned int p_x, const unsigned int p_y,
                 unsigned int p_color,
                 char *p_shape_id, const int p_dir);

void gal_piece_delete(Piece_t *p_pce);
int gal_piece_next_direction(Piece_t *p_pce);
void gal_piece_rotate(Piece_t *p_pce);
void gal_piece_move(Piece_t *p_pce, double dx, double dy);


/* MAP */

/* the data structure of the core games playarea */
typedef struct
{
    unsigned int color;
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
    unsigned int matrix[MAP_WIDTH * MAP_HEIGHT];
    unsigned int buffer[MAP_WIDTH * MAP_HEIGHT];
} MapData_t;


/* load and initialize a map */
int gal_map_init(MapData_t *p_map,
            const unsigned int p_x,
            const unsigned int p_y,
            const unsigned int p_width,
            const unsigned int p_height);

void gal_map_clear(MapData_t *p_map);

#endif
