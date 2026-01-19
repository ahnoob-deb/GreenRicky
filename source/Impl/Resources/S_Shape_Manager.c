#include "S_Shape_Manager.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "S_Game_Objects.h"

#include "S_Quit.h"

/* max length of an id string in the shape tree */
#define MAX_ID_LENGTH 255

/* root of the shape tree */
static ShapeNode_t *root_shapes = NULL;

/* INTERNAL FUNCTION PROTOTYPES */
/* allocate memory for a shape node */
static ShapeNode_t *alloc_ShapeNode(void);
/* add a ShapeNode_t to the tree of shapes */
static ShapeNode_t *tree_add_shape(ShapeNode_t *p_node, ShapeData_t **p_data, char *p_id);

/* INTERNAL FUNCTION DEFINITIONS */

/* description : allocate memory for a shape node.
 * If successfull, it returns a pointer to the shape node.
 * In case of error it returns NULL.
 */
static ShapeNode_t *alloc_ShapeNode()
{
    return (ShapeNode_t *)malloc(sizeof(ShapeNode_t));
}

/* description : add a shape pointer to the tree of shapes.
 * Technically, the tree represents a tree-map, where you can
 * search a ShapeNode_t pointer by an id-string.
 */
static ShapeNode_t *tree_add_shape(ShapeNode_t *p_node, ShapeData_t **p_data, char *p_id)
{
    printf("adding shape with id [%s].\n", p_id);

    if (p_node == NULL)
    {
        p_node = alloc_ShapeNode();
        if (p_node == NULL)
        {
            printf("could not allocate memory for shape.");
            quit_game_with_log_error("memory allocation error.", 1);
        }

        p_node->id = malloc(MAX_ID_LENGTH * sizeof(char));

        if (p_node->id == NULL)
        {

            printf("could not allocate memory for shape-id.");
            quit_game_with_log_error("memory allocation error.", 1);
        }

        printf("adding new node.\n");

        strncpy(p_node->id, p_id, MAX_ID_LENGTH);
        p_node->data = p_data;
        p_node->left = NULL;
        p_node->right = NULL;
    }
    else if (strcmp(p_node->id, p_id) == 0)
    {
        p_node->data = p_data;
    }
    else if (strcmp(p_node->id, p_id) < 0)
    {
        p_node->left = tree_add_shape(p_node->left, p_data, p_id);
    }
    else if (strcmp(p_node->id, p_id) > 0)
    {
        p_node->right = tree_add_shape(p_node->right, p_data, p_id);
    }

    return p_node;
}

/* description : search a ShapeNode, identified by an id-string in
 * the tree-map of shape-nodes.
 */
static ShapeNode_t *tree_search_shape(ShapeNode_t *p_node, char *p_id)
{

    printf("searching shape with id [%s]\n", p_id);

    ShapeNode_t *ret = NULL;

    if (p_node == NULL)
    {
        return NULL;
    }
    else if (strcmp(p_node->id, p_id) == 0)
    {
        printf("found.\n");

        ret = p_node;
    }
    else if (strcmp(p_node->id, p_id) < 0)
    {
        ret = tree_search_shape(p_node->left, p_id);
    }
    else if (strcmp(p_node->id, p_id) > 0)
    {
        ret = tree_search_shape(p_node->right, p_id);
    }

    return ret;
}

/* description : this function free's recursively the shape-nodes beginning from
 * a specific starting point p_node.
 */
static void tree_shapes_destroy(ShapeNode_t *p_node)
{

    if (p_node != NULL)
    {
        /* go recursiv through the tree... */
        tree_shapes_destroy(p_node->left);
        tree_shapes_destroy(p_node->right);

        printf("freeing shape node with id [%s]...\n", p_node->id);
        /* free the id */
        free(p_node->id);
        /* free the ShapeData */
        gal_destroy_shape(p_node->data);
        /* free the node itself */
        free(p_node);
    }
}

/* PUBLIC FUNCTION DEFINITIONS */
/* PROTOTYPES IN db_shapes.h */

/* description : allows the caller to add a shape to the internal
 * tree of shapes (e.g. ShapeNodes, which contain a pointer to ShapeData)
 */
ShapeNode_t *add_shape(char *p_id, ShapeData_t **p_data)
{
    ShapeNode_t *test_existance = search_shape(p_id);

    if (test_existance != NULL)
    {
        printf("There is already a shape with id '%s'.\nCan't add second shape with same id!\n", p_id);
        quit_game_with_log_error("data management error.", 1);
    }

    return root_shapes = tree_add_shape(root_shapes, p_data, p_id);
}


/* description : public method which allows the caller to search a
   specific ShapeNode, identified by an id-string, in the internal
 * tree of shapes (e.g. ShapeNodes, which contain a pointer to ShapeData)
 */

ShapeNode_t *search_shape(char *p_id)
{
    return tree_search_shape(root_shapes, p_id);
}

/* description : public method which allows the caller to release the memory of whole
 * tree of shapes (e.g. ShapeNodes, which contain a pointer to ShapeData)
 */
void destroy_shape_tree()
{
    tree_shapes_destroy(root_shapes);
}
