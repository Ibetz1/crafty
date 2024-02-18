#include "block_textures.hpp"
#include "cvec3.hpp"

/*
    takes in distance from top block
*/
U8 block_tex_height_map(U64 y, U64 h) {
    
    // face blocks
    if (h - y == 1) {
        if (h < sand_level + 2) {
            return block_ids_SAND; // sand
        } else {
            return block_ids_GRASS; // grass
        }
    }
    else if (h - y < 8) {
        return block_ids_DIRT; // dirt
    } else {
        return block_ids_STONE; // stone
    }
}

/*
    returns a block based on height
*/
vec3_u8 block_tex_color_map(U8 tex) {
    if (tex == block_ids_GRASS) { // grass
        return {70, 160, 20};
    }

    if (tex == block_ids_DIRT) { // dirt
        return {130, 50, 16};
    }

    if (tex == block_ids_STONE) { // stone
        return {90, 80, 70};
    }

    if (tex == block_ids_SAND) {
        return {200, 180, 80};
    }

    if (tex == block_ids_WATER) {
        return {30, 70, 150};
    }

    return {255, 0, 0};
}

/*
    detmines block transparency
*/
U8 block_tex_alpha_map(U8 tex) {
    if (tex == block_ids_AIR) return 0;
    if (tex == block_ids_WATER) return 128;

    return 255;
}