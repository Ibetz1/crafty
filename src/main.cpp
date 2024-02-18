#include "base_inc.h"
#include "raylib.h"
#include "raymath.h"
#include "ui.hpp"
#include "shaders.hpp"
#include "chunk_render.hpp"
#include "chunk_loader.hpp"
#include "cube_render.hpp"

#include "base_inc.c"

enum GameState 
{
    GAME_RUNNING = 0,
    GAME_PAUSED,
};

static World global_world;
static Camera global_camera = { 0 };

static GameState game_state;

static S32 global_screen_width = 800;
static S32 global_screen_height = 450;

S32 game_paused = 0;

static void crafty_init() {
    //- NOTE(cabarger): Raylib boilerplate
    InitWindow(global_screen_width, global_screen_height, "Crafty3D");
    SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_TOPMOST);
    SetTargetFPS(60);
    DisableCursor(); // NOTE(cabarger): Also locks the cursor
    
    //- NOTE(cabarger): camera default values...
    global_camera.position = (Vector3){ 0.2f, 0.4f, 0.2f }; 
    global_camera.target = (Vector3){ 0.185f, 0.4f, 0.0f }; // Looking at point
    global_camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };       // Up vector (rotation towards target)
    global_camera.fovy = 45.0f;                             // Field-of-view Y
    global_camera.projection = CAMERA_PERSPECTIVE;          // Projection type

    /*
        our stuff down here
    */
    game_state = GAME_RUNNING;
    // ------- Lighting --------
    init_shaders();
    create_light(LIGHT_POINT, (Vector3){ -2, 1, -2 }, Vector3Zero(), YELLOW);
    create_light(LIGHT_DIRECTIONAL, (Vector3){ -2, 1, -2 }, Vector3Zero(), YELLOW);
    // ------- Lighting ---------
    create_ui(global_screen_width, global_screen_height);
    init_chunk_render();
}

static void handle_window_resize() {
    if (IsWindowResized()) {
        global_screen_width = GetScreenWidth();
        global_screen_height = GetScreenHeight();
    }
}

static void update_camera_and_movement() {

#define CAMERA_MOUSE_MOVE_SENSITIVITY 0.03f     
#define CAMERA_MOVE_SPEED             0.09f
#define CAMERA_ROTATION_SPEED         0.03f
   
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
static void crafty_update(F32 dt) {
    if(IsKeyPressed(KEY_P)) 
    {
        if (game_state == GAME_RUNNING)
            game_state = GAME_PAUSED;
        else if(game_state == GAME_PAUSED)
            game_state = GAME_RUNNING;
        // game_paused = !game_paused;
        // if (game_paused == 1) game_state = GAME_PAUSED;
        // else {
        //     game_state = GAME_RUNNING;
        // }
    }
    switch(game_state) {
        case GAME_RUNNING:
        {
            handle_window_resize();
            update_camera_and_movement();
            update_chunk_render(dt);
            update_shaders(&global_camera);
            update_hotbar(global_screen_width, global_screen_height);
            break;
        }
        case GAME_PAUSED:
        {
            break;
        }
    }
}

static void crafty_draw() {
    BeginDrawing();
    switch(game_state)
    {
        case GAME_RUNNING:
        {
            ClearBackground(BLACK);
            BeginMode3D(global_camera);
            
            DrawGrid(100, 1);

            draw_chunk_render();
                    EndMode3D();
            DrawFPS(0, 0);
            draw_hotbar(global_screen_width, global_screen_height);
            break;
        }
        case GAME_PAUSED:
        {
            Font my_font = LoadFont("resources/ComicMono-Bold.ttf");
            char * str = "Press 1 to exit, 2 to leave";
            Vector2 vec = {global_screen_width/2.0f - MeasureText(str, 30)/2.0f, global_screen_height/2.0f};

            DrawTextEx(my_font, str, vec, 30, 1, RED);
            if (IsKeyPressed(KEY_ONE))
            {
                exit(EXIT_SUCCESS);
            }
            else if (IsKeyPressed(KEY_TWO))
            {
                game_state = GAME_RUNNING;
            }
            break;
        }
    }
    EndDrawing();
}

int main(void) {
    crafty_init();
    while (!WindowShouldClose()) {
        crafty_update(GetFrameTime());
        crafty_draw();
    }
    CloseWindow();

    return 0;
}
