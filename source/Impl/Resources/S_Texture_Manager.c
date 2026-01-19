#include "S_Texture_Manager.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "D_Common.h"
#include "S_Quit.h"
#include "S_SDL3_Rendering.h"

static SDL_Renderer *renderer;

static MyGameTexture_t *root_textures = NULL;

static MyGameTexture_t *mt_in_alloc_texture(void);
static MyGameTexture_t *mt_in_add_texture(MyGameTexture_t *p_node, char p_id[], SDL_Texture *p_tex, int p_width, int p_height);
static MyGameTexture_t *mt_in_search_texture(MyGameTexture_t *p_node, char p_id[]);
static void mt_in_tree_textures_destroy(MyGameTexture_t *p_node);
static int mt_in_load_texture(char *p_filename, MyGameTexture_t **p_hook);

void mt_init() {
	renderer=sdla_get_renderer();
}

static int mt_in_load_texture(char *p_filename, MyGameTexture_t **p_hook)
{
    SDL_Surface *surface = NULL;
    char *bmp_path = NULL;

    /* Textures are pixel data that we upload to the video hardware for fast drawing. Lots of 2D
       engines refer to these as "sprites." We'll do a static MyGameTexture_t (upload once, draw many
       times) with data from a bitmap file. */

    /* SDL_Surface is pixel data the CPU can access. SDL_Texture is pixel data the GPU can access.
       Load a .bmp into a surface, move it to a MyGameTexture_t from there. */
    SDL_asprintf(&bmp_path, "%s%s%s", SDL_GetBasePath(), "../data/images/", p_filename); /* allocate a string of the full file path */

    surface = IMG_Load(bmp_path);
    if (!surface)
    {
        printf("Couldn't load image: %s\n", SDL_GetError());
        SDL_Log("Couldn't load image: %s", SDL_GetError());
        quit_game_with_log_error("data management error.\n", 1);
    }
    SDL_free(bmp_path); /* done with this, the file is loaded. */

    if (SDL_SetSurfaceColorKey(surface, true, SDL_MapSurfaceRGB(surface, 0xFF, 0xFF, 0xFF)) == false)
    {
        printf("Unable to color key! SDL error: %s\n", SDL_GetError());
        SDL_Log("Unable to color key! SDL error: %s", SDL_GetError());
        quit_game_with_log_error("data management error.\n", 1);
    }

    SDL_Texture *tmp_texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (tmp_texture == NULL)
    {
        printf("Couldn't convert surface to texture : %s\n", SDL_GetError());
        SDL_Log("Couldn't convert surface to texture : %s", SDL_GetError());
        quit_game_with_log_error("data management error.\n", 1);
    }

    mt_add_texture(p_filename, tmp_texture, tmp_texture->w, tmp_texture->h);

    /* SDL_DestroySurface(surface); */ /* done with this, the MyGameTexture_t has a copy of the pixels now. */

    printf("hooking texture for %s ... \n", p_filename);
    if (p_hook != NULL)
    {
        *p_hook = mt_search_texture(p_filename);

        if (*p_hook == NULL)
        {
            printf("could not find %s in the tree! exit.\n", p_filename);
            quit_game_with_log_error("data management error.\n", 1);
        }
        printf("adress hook[%s] : [%p]\n", p_filename, *p_hook);
    }

    return TRUE;
}

static MyGameTexture_t *mt_in_alloc_texture()
{
    return (MyGameTexture_t *)malloc(sizeof(MyGameTexture_t));
}

static MyGameTexture_t *mt_in_add_texture(MyGameTexture_t *p_node, char p_id[], SDL_Texture *p_tex, int p_width, int p_height)
{
    if (p_node == NULL)
    {
        p_node = mt_in_alloc_texture();
        if (p_node == NULL)
        {
            printf("Could not allocate mem for MyGameTexture_t with id %s.\n", p_id);
            quit_game_with_log_error("data management error.", 1);
        }

        p_node->id = malloc(MAX_ID_LENGTH * sizeof(char));

        if (p_node->id == NULL)
        {

            printf("Could not allocate mem for id '%s'.\n", p_id);
            quit_game_with_log_error("data management error.", 1);
        }

        p_node->texture = p_tex;
        p_node->width = p_width;
        p_node->height = p_height;
        strncpy(p_node->id, p_id, MAX_ID_LENGTH);
        p_node->left = NULL;
        p_node->right = NULL;
    }
    else if (strcmp(p_node->id, p_id) == 0)
    {
        p_node->texture = p_tex;
        p_node->width = p_width;
        p_node->height = p_height;
    }
    else if (strcmp(p_node->id, p_id) < 0)
    {
        p_node->left = mt_in_add_texture(p_node->left, p_id, p_tex, p_width, p_height);
    }
    else if (strcmp(p_node->id, p_id) > 0)
    {
        p_node->right = mt_in_add_texture(p_node->right, p_id, p_tex, p_width, p_height);
    }

    return p_node;
}

static MyGameTexture_t *mt_in_search_texture(MyGameTexture_t *p_node, char p_id[])
{

    MyGameTexture_t *ret = NULL;

    if (p_node == NULL)
    {
        return NULL;
    }
    else if (strcmp(p_node->id, p_id) == 0)
    {

        ret = p_node;
    }
    else if (strcmp(p_node->id, p_id) < 0)
    {
        ret = mt_in_search_texture(p_node->left, p_id);
    }
    else if (strcmp(p_node->id, p_id) > 0)
    {
        ret = mt_in_search_texture(p_node->right, p_id);
    }

    return ret;
}



/* description : this function free's recursively the shape-nodes beginning from
 * a specific starting point p_node.
 */
static void mt_in_tree_textures_destroy(MyGameTexture_t *p_node)
{

    if (p_node != NULL)
    {
        /* go recursiv through the tree... */
        mt_in_tree_textures_destroy(p_node->left);
        mt_in_tree_textures_destroy(p_node->right);

        printf("free MyGameTexture_t node with id [%s]...\n", p_node->id);
        /* free the id */
        free(p_node->id);
        /* free the MyGameTexture_t */
        SDL_DestroyTexture(p_node->texture);
        /* free the node itself */
        free(p_node);
    }
}

MyGameTexture_t *mt_add_texture(char p_id[], SDL_Texture *p_tex, int p_width, int p_height)
{
    MyGameTexture_t *test_existance = mt_search_texture(p_id);
    MyGameTexture_t *ret = NULL;

    if (test_existance != NULL)
    {
        printf("There is already a texture with id '%s'.\nCan't add second texture with same id!\n", p_id);
        quit_game_with_log_error("data management error.", 1);
    }
    else if (p_tex == NULL)
    {

        printf("Could not find %s, trying to load from disk...\n", p_id);
        mt_in_load_texture(p_id, &ret);

        if (ret == NULL)
        {
            printf("Could not load file '%s'.\n", p_id);
            quit_game_with_log_error("data management error.", 1);
        }

        return ret;
    }

    return root_textures = mt_in_add_texture(root_textures, p_id, p_tex, p_width, p_height);
}

MyGameTexture_t *mt_search_texture(char p_id[])
{
    return mt_in_search_texture(root_textures, p_id);
}

void mt_destroy_texture_tree()
{
    mt_in_tree_textures_destroy(root_textures);
}
