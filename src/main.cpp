#include "base_inc.h"
#include "raylib.h"
#include "raymath.h"

#include "shaders.hpp"
#include "chunk_render.hpp"
#include "chunk_loader.hpp"

extern "C" {
    #include "base_inc.c"
}
#include "cube_render.hpp"


#define CAMERA_MOUSE_MOVE_SENSITIVITY 0.03f     
#define CAMERA_MOVE_SPEED             0.09f
#define CAMERA_ROTATION_SPEED         0.03f

static World global_world;

static Camera global_camera = { 0 };

const int screen_width = 800;
const int screen_height = 450;

Model my_model;

// Generate a simple triangle mesh from code
static Mesh GenMeshCustom(void)
{
    Mesh mesh = { 0 };
    mesh.triangleCount = 0;

    vec3_f32 offset = {3, 0, 0};

    push_bot_face(&mesh, offset);
    push_top_face(&mesh, offset);
    push_back_face(&mesh, offset);
    push_front_face(&mesh, offset);
    push_left_face(&mesh, offset);
    push_right_face(&mesh, offset);

    // Upload mesh data from CPU (RAM) to GPU (VRAM) memory
    UploadMesh(&mesh, false);

    return mesh;
}

/*
    on runtime
*/
static void init() {

    //- NOTE(cabarger): camera default values...
    global_camera.position = (Vector3){ 0.2f, 0.4f, 0.2f }; 
    global_camera.target = (Vector3){ 0.185f, 0.4f, 0.0f }; // Looking at point
    global_camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };       // Up vector (rotation towards target)
    global_camera.fovy = 45.0f;                             // Field-of-view Y
    global_camera.projection = CAMERA_PERSPECTIVE;          // Projection type

    /*
        our shit down here
    */
    // ------- Lighting --------
    init_shaders();
    create_light(LIGHT_POINT, (Vector3){ -2, 1, -2 }, Vector3Zero(), YELLOW);
    create_light(LIGHT_DIRECTIONAL, (Vector3){ -2, 1, -2 }, Vector3Zero(), YELLOW);
    // ------- Lighting ---------
    init_chunk_render();

     my_model = LoadModelFromMesh(GenMeshCustom());
}

static void update_camera_and_movement() {
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
    pre-render pass
*/
static void update(F32 dt) {
    update_camera_and_movement();
    update_chunk_render(dt);
    update_shaders(&global_camera);
}

static void draw() {
    DrawGrid(100, 1);

    // draw_chunk_render();
    DrawModel(my_model, {0, 0, 0}, 1.0f, WHITE);
}

int main(void) {
    InitWindow(screen_width, screen_height, "AlgoCraft3D");
    SetTargetFPS(60);
    DisableCursor(); // NOTE(cabarger): Also locks the cursor

    init();
    while (!WindowShouldClose()) {
        update(GetFrameTime());

        BeginMode3D(global_camera);

        ClearBackground(BLACK);
        
        draw();

        EndMode3D();
        EndDrawing();
    }
    deload_shaders();
    CloseWindow();

    return 0;
}
