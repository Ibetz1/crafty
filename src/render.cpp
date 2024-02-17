internal void draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode3D(global_camera);
    DrawGrid(10, 1.0f); 

    //- NOTE(cabarger): Draw a cube
    DrawModel(
        global_DEBUG_block_model, 
        (Vector3){.x = 0.0f, .y = 0.0f, .z = 0.0f},  // Pos
        1.0f, // Scale
        BLUE // Tint
    ); 

    EndMode3D();
    EndDrawing();
}
