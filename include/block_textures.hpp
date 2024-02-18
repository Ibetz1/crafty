#ifndef _BLOCK_TEX_HPP
#define _BLOCK_TEX_HPP

#include "base_types.h"
#include "cvec3.hpp"

static U64 water_level = 13;
static U64 sand_level = water_level + 1;

typedef U8 block_ids;
enum {
    block_ids_AIR    = 0,
    block_ids_GRASS  = 1,
    block_ids_DIRT   = 2,
    block_ids_STONE  = 3,
    block_ids_SAND   = 4,
    block_ids_WATER  = 5,
};

/*
    takes in distance from top block
*/
U8 block_tex_height_map(U64 y, U64 h);

/*
    returns a block based on height
*/
vec3_u8 block_tex_color_map(U8 tex);

/*
    detmines block transparency
*/
U8 block_tex_alpha_map(U8 tex);


#endif