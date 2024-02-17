internal void draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawFPS(0, 0);
    
    BeginMode3D(global_camera);

#if 0 // NOTE(cabarger): draw chunk benchmark
    for (U8 chunk_x=0; chunk_x < 1; ++chunk_x) {
        //- cabarger: draw a chunk
        for (U8 block_x=0; block_x < 16; ++block_x) {
            for (U8 block_y=0; block_y < 16; ++block_y) {
                for (U8 block_z=0; block_z < 16; ++block_z) {
#if 0
                    DrawModel(
                        global_DEBUG_block_model, 
                        (Vector3){.x = (F32)(block_x + chunk_x * 16), .y = (F32)(block_y) , .z = (F32)(block_z) },  // Pos
                        1.0f, // Scale
                        BLUE // Tint
                    ); 
#else 
                    DrawModelWires(
                        global_DEBUG_block_model, 
                        (Vector3){.x = (F32)(block_x + chunk_x * 16), .y = (F32)(block_y) , .z = (F32)(block_z) },  // Pos
                        1.0f, // Scale
                        BLUE // Tint
                    ); 
#endif
                }
            }
        }
    }
#endif

    //- cabarger: Test draw chunk from Ian's code
    Chunk chunk = World::chunk_at_chunk_cor(&global_world, 0, 0);
    
    for (U8 block_x = 0; block_x < CHUNK_W; ++block_x) {
        for (U8 block_y = 0; block_y < CHUNK_H; ++block_y) {
            for (U8 block_z = 0; block_z < CHUNK_W; ++block_z) {
                Block* block = Chunk::block_at(&chunk, block_x, block_y, block_z);

                Vector3 vpos = {
                    .x = (F32) block_x,
                    .y = (F32) block_y,
                    .z = (F32) block_z
                };

                if (block_y == 11) {
                    DrawModelWires(
                        global_DEBUG_block_model, 
                        vpos,
                        1.0f, // Scale
                        BLUE // Tint
                    ); 
                }
            }
        }
    }
    
    EndMode3D();
    EndDrawing();
}
