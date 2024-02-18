#include "base_inc.h"
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include "ui.hpp"
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
static BoundingBox global_camera_bb = { 0 };

static Model global_skybox_model = { 0 };
static Shader global_shdr_cubemap = { 0 };

static GameState game_state;
static Font comic_mono_font;

static S32 global_screen_width = 800;
static S32 global_screen_height = 450;

S32 game_paused = 0;

static void crafty_init() {
    //- NOTE(cabarger): Raylib boilerplate
    InitWindow(global_screen_width, global_screen_height, "Crafty3D");
    SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_TOPMOST);
    SetTargetFPS(60);
    DisableCursor(); // NOTE(cabarger): Also locks the cursor

    Mesh skybox_cube_mesh = GenMeshCube(1.0f, 1.0f, 1.0f);
    global_skybox_model = LoadModelFromMesh(skybox_cube_mesh);

    global_skybox_model.materials[0].shader = LoadShader(TextFormat("resources/shaders/glsl%i/skybox.vs", 330),
                                        TextFormat("resources/shaders/glsl%i/skybox.fs", 330));

    SetShaderValue(global_skybox_model.materials[0].shader, GetShaderLocation(global_skybox_model.materials[0].shader, "environmentMap"), (int[1]){ MATERIAL_MAP_CUBEMAP }, SHADER_UNIFORM_INT);
    SetShaderValue(global_skybox_model.materials[0].shader, GetShaderLocation(global_skybox_model.materials[0].shader, "doGamma"), (int[1]) { 0 }, SHADER_UNIFORM_INT);
    SetShaderValue(global_skybox_model.materials[0].shader, GetShaderLocation(global_skybox_model.materials[0].shader, "vflipped"), (int[1]){ 0 }, SHADER_UNIFORM_INT);

    global_shdr_cubemap = LoadShader(TextFormat("resources/shaders/glsl%i/cubemap.vs", 330),
                                TextFormat("resources/shaders/glsl%i/cubemap.fs", 330));

    SetShaderValue(global_shdr_cubemap, GetShaderLocation(global_shdr_cubemap, "equirectangularMap"), (int[1]){ 0 }, SHADER_UNIFORM_INT);

    char skybox_filename[256] = { 0 };

    Image skybox_img = LoadImage("resources/skybox.png");
    global_skybox_model.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(skybox_img, CUBEMAP_LAYOUT_AUTO_DETECT);     
    UnloadImage(skybox_img);
        
    //- NOTE(cabarger): camera default values...
    global_camera.position = (Vector3){ 0.2f, CHUNK_W*3, 0.2f }; 
    global_camera.target = (Vector3){ 0.185f, 0.4f, 0.0f }; // Looking at point
    global_camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };       // Up vector (rotation towards target)
    global_camera.fovy = 45.0f;                             // Field-of-view Y
    global_camera.projection = CAMERA_PERSPECTIVE;          // Projection type

    game_state = GAME_RUNNING;
    create_ui();
    comic_mono_font = LoadFont("resources/ComicMono-Bold.ttf");
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
#define CAMERA_DIM                    0.25f

    Vector2 mouse_pos_delta = GetMouseDelta();
    Vector3 movement_this_frame = Vector3Zero();
    Vector3 rotation_this_frame = Vector3{
        .x = mouse_pos_delta.x * CAMERA_MOUSE_MOVE_SENSITIVITY,
        .y = mouse_pos_delta.y * CAMERA_MOUSE_MOVE_SENSITIVITY,
        .z = 0.0f
    };

    Vector3 last_camera_p = global_camera.position; //- cabarger: We'll save the position 
                                                    // in the case of a collision.
    if (IsKeyDown(KEY_W)) movement_this_frame.x += CAMERA_MOVE_SPEED; // Forward
    if (IsKeyDown(KEY_S)) movement_this_frame.x += -CAMERA_MOVE_SPEED; // Backward
    if (IsKeyDown(KEY_A)) movement_this_frame.y += -CAMERA_MOVE_SPEED; // Left
    if (IsKeyDown(KEY_D)) movement_this_frame.y += CAMERA_MOVE_SPEED; // Right
    if (IsKeyDown(KEY_SPACE)) movement_this_frame.z += CAMERA_MOVE_SPEED; // Up
    if (IsKeyDown(KEY_LEFT_CONTROL)) movement_this_frame.z += -CAMERA_MOVE_SPEED; // Down 
   
    UpdateCameraPro(&global_camera,  movement_this_frame, rotation_this_frame, 0.0f);

#if 0
    ///////////////////////////////
    //~ cabarger: Nieve collision checking. 

    global_camera_bb = (BoundingBox){
        .min = (Vector3){
            .x = global_camera.position.x - (F32)CAMERA_DIM / 2.0f, 
            .y = global_camera.position.y - (F32)CAMERA_DIM / 2.0f,
            .z = global_camera.position.z - (F32)CAMERA_DIM / 2.0f,
        },
        .max = (Vector3){
            .x = global_camera.position.x + (F32)CAMERA_DIM / 2.0f, 
            .y = global_camera.position.y + (F32)CAMERA_DIM / 2.0f,
            .z = global_camera.position.z + (F32)CAMERA_DIM / 2.0f,
        },
    };
    
    //- FIX(cabarger): Rounding???? 
    U64 block_start_x = (U64)clamp<S64>((S64)global_camera_bb.min.x * 1.1, 0, 0x0FFFF);
    U64 block_start_z = (U64)clamp<S64>((S64)global_camera_bb.min.z * 1.1, 0, 0x0FFFF);
    
    Chunk camera_chunk = World::chunk_at_chunk_cor(
        &global_world, (vec2_u64){
        .x = (block_start_x / CHUNK_W), 
        .y = (block_start_z / CHUNK_W),
    });
    
    Model* chunk_model = static_model_from_chunk(&camera_chunk);
    BoundingBox chunk_bb = GetModelBoundingBox(*chunk_model);
    
    if (CheckCollisionBoxes(global_camera_bb, chunk_bb))
        global_camera.position = last_camera_p;
#endif
}

static void crafty_update(F32 dt) {
    if (IsKeyPressed(KEY_P)) game_state = GameState(((U8)game_state + 1) % 2);
    switch(game_state) {
        case GAME_RUNNING: {
            handle_window_resize();
            update_camera_and_movement();
            update_hotbar(global_screen_width, global_screen_height);
        } break;
        case GAME_PAUSED: break;
    }
}

bool chunks_drawn = false;
static void crafty_draw() {

    /*
        loading screen
    */
    if (!chunks_drawn) {
        BeginDrawing();

        ClearBackground(BLACK);
        DrawTextEx(comic_mono_font, "loading", {0, 0}, 30, 1, RED);

        EndDrawing();

        init_chunk_render();
        chunks_drawn = true;
    }

    BeginDrawing();

    switch(game_state) {
        case GAME_RUNNING: {
            
            ClearBackground(BLACK);
            BeginMode3D(global_camera);

            rlDisableBackfaceCulling();
            rlDisableDepthMask();

            DrawModel(global_skybox_model, (Vector3){0, 0, 0}, 1.0f, WHITE);

            rlEnableBackfaceCulling();
            rlEnableDepthMask();
          
            
            DrawGrid(100, 1);

            draw_chunk_render();
                    EndMode3D();
            DrawFPS(0, 0);
            draw_hotbar(global_screen_width, global_screen_height);

        } break;
        case GAME_PAUSED: {
            const char* str = "Press 1 to exit game, 2 to exit this menu.";
            Vector2 vec = {global_screen_width/2.0f - MeasureText(str, 30)/2.0f, global_screen_height/2.0f};

            DrawTextEx(comic_mono_font, str, vec, 30, 1, RED);
            if (IsKeyPressed(KEY_ONE)) {
                exit(EXIT_SUCCESS);
            }
            else if (IsKeyPressed(KEY_TWO)) {
                game_state = GAME_RUNNING;
            }
        } break;
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
