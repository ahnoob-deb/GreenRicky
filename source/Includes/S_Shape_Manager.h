#ifndef KRN_MANAGER_SHAPES_H
#define KRN_MANAGER_SHAPES_H 1

#include "S_Game_Objects.h"

/* Allows the caller to add a shape to the internal
 * tree of shapes (delivers the ShapeNode, which contains a pointer to ShapeData)
 */
ShapeNode_t *add_shape(char *p_id, ShapeData_t **p_data);

/* Allows the caller to search a specific ShapeNode, 
 * identified by an id-string, in the internal
 * tree of shapes (delivers ShapeNodes, which contain a pointer to ShapeData)
 */
ShapeNode_t *search_shape(char *p_id);

/* Allows the caller to release the memory of whole
 * tree of shapes (delivers ShapeNodes, which contain a pointer to ShapeData)
 */
void destroy_shape_tree(void);

#endif
