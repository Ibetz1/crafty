#include "raylib.h"
#include "raymath.h"

extern "C" {
    #include "base_inc.h"
    #include "base_inc.c"
}

#define CAMERA_MOUSE_MOVE_SENSITIVITY                   0.03f     
#define CAMERA_MOVE_SPEED                               0.09f
#define CAMERA_ROTATION_SPEED                           0.03f

global Camera global_camera = { 0 };

global Mesh global_DEBUG_block_mesh; 
global Material global_DEBUG_block_material;
global Model global_DEBUG_block_model;

/*
    on runtime
*/
internal void init() {
    const int screen_width = 800;
    const int screen_height = 450;
    InitWindow(screen_width, screen_height, "AlgoCraft3D");
    
    global_DEBUG_block_mesh = GenMeshCube(1.0f, 1.0f, 1.0f);                            
    global_DEBUG_block_material = LoadMaterialDefault();
    global_DEBUG_block_model = LoadModelFromMesh(global_DEBUG_block_mesh);
    
    //- NOTE(cabarger): camera default values...
    global_camera.position = (Vector3){ 0.2f, 0.4f, 0.2f }; 
    global_camera.target = (Vector3){ 0.185f, 0.4f, 0.0f }; // Looking at point
    global_camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };       // Up vector (rotation towards target)
    global_camera.fovy = 45.0f;                             // Field-of-view Y
    global_camera.projection = CAMERA_PERSPECTIVE;          // Projection type
    Vector3 position = { 0.0f, 0.0f, 0.0f };                // Set model position
}

/*
    pre-render pass
*/
internal void update(F32 dt) {
    Vector2 mouse_pos_delta = GetMouseDelta();
    Vector3 movement_this_frame = Vector3Zero();
    Vector3 rotation_this_frame = Vector3{
        .x = mouse_pos_delta.x * CAMERA_MOUSE_MOVE_SENSITIVITY,
        .y = mouse_pos_delta.y * CAMERA_MOUSE_MOVE_SENSITIVITY,
        .z = 0.0f
    };

    if (IsKeyDown(KEY_W)) movement_this_frame.x += CAMERA_MOVE_SPEED; // Forward
    if (IsKeyDown(KEY_S)) movement_this_frame.x += -CAMERA_MOVE_SPEED; // Backward
    if (IsKeyDown(KEY_A)) movement_this_frame.y += -CAMERA_MOVE_SPEED; // Left
    if (IsKeyDown(KEY_D)) movement_this_frame.y += CAMERA_MOVE_SPEED; // Right
    if (IsKeyDown(KEY_SPACE)) movement_this_frame.z += CAMERA_MOVE_SPEED; // Up
    if (IsKeyDown(KEY_LEFT_CONTROL)) movement_this_frame.z += -CAMERA_MOVE_SPEED; // Down 
   
    UpdateCameraPro(&global_camera,  movement_this_frame, rotation_this_frame, 0.0f);
}

/*
    render pass
*/
internal void draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    BeginMode3D(global_camera);

    DrawModel(
        global_DEBUG_block_model, 
        (Vector3){.x = 0.0f, .y = 0.0f, .z = 0.0f},  // Pos
        1.0f, // Scale
        WHITE // Tint
    ); 

    EndMode3D();
    EndDrawing();
}

int main(void) {
    init();
    SetTargetFPS(60);
    DisableCursor();                // Limit cursor to relative movement inside the window

    while (!WindowShouldClose()) {
        update(GetFrameTime());
        draw();
    }
    
    CloseWindow();

    return 0;
}
