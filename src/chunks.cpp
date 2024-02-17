/*
    - define a maximum width of chunk rendering 
    (render distance containing radius of loaded chunks) (2*r = w) 
    
    - allocate enough memory for w^2 chunks such that the chunks can be unloaded/loaded
    into a fixed buffer

*/

extern "C" {
    #include "base_inc.h"
    #include "base_inc.c"
}

#include <stdio.h>
#include <math.h>
#include "chunks.hpp"



// compute size of chunks
inline U64 compute_chunk_size_bytes(U8 maxh, U8 maxw) {
    return maxw * maxw * maxh * sizeof(block);
}

// allocate chunks with the render distance as the radius
block* alloc_world_arena(U64 render_distance) {

    /*
        add 1 for center chunk
    */
    U64 chunk_count = pow((render_distance * 2), 2);

    U64 chunk_size_bytes = compute_chunk_size_bytes(CHUNK_H, CHUNK_W);

    return (block*) calloc(chunk_count, chunk_size_bytes);
}

block* get_block(block* world, U64 x, U64 y, U64 z, U64 render_distance) {

    /*
        bind coords to world
    */
    U64 world_width = render_distance * CHUNK_W;
    x = fmin(fmax(x, 0), world_width - 1);
    y = fmin(fmax(y, 0), world_width - 1);
    z = fmin(fmax(z, 0), CHUNK_H - 1);

    /*
        map x, y, z to linear coordinates
    */
    U64 index = x + (CHUNK_W) * (y + CHUNK_W * z);

    return world + index;
}

void test() {
    block* blocks = alloc_world_arena(1);

    block* b;
    for (U64 x = 0; x < CHUNK_W; x++) {
        for (U64 y = 0; y < CHUNK_W; y++) {
            for (U64 z = 0; z < CHUNK_H; z++) {
                b = get_block(blocks, x, y, z, 1);
                b->tex = x;
            }
            printf("%i ", b->tex);
        }
        printf("\n");
    }


    // int w = 16;
    // int h = 16;

    // int* base_ptr = (int*) calloc(w * h, sizeof(int));

    // // index = width * col + row;
    // for (int row = 0; row < h; row++) {
    //     for (int col = 0; col < w; col++) {
    //         int index = w * col + row;
    //         *(base_ptr + index) = index;
    //     }
    // }

    // for (int row = 0; row < h; row++) {
    //     for (int col = 0; col < w; col++) {
    //         int index = w * col + row;
    //         int v = *(base_ptr + index);

    //         printf("%i %i\n", index, v);
    //     }
    // }

}

// 2d array map:
// index = width * rol + col