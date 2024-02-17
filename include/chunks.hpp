#ifndef _CHUNK_HPP
#define _CHUNK_HPP

#define CHUNK_W 16
#define CHUNK_H 255

#include <math.h>


// contains block data
struct block {
    U8 tex = 0;
};

// handles chunk pointers
struct chunk {
    U64 x_cor; // x coord in chunk space
    U64 y_cor; // y coord in chunk space
    block* base_ptr; // ptr to top left block
};

block* alloc_world_arena(size_t render_distance);
void test();


#endif