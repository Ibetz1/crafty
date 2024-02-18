#include "raylib.h"
#include "raymath.h"

#include "shaders.h"
#include "chunks.hpp"

extern "C" {
    #include "base_inc.h"
    #include "base_inc.c"
}

//#include "shaders.cpp"

#define CAMERA_MOUSE_MOVE_SENSITIVITY 0.03f
#define CAMERA_MOVE_SPEED             0.09f
#define CAMERA_ROTATION_SPEED         0.03f

global World global_world;

global Camera global_camera = { 0 };

global Mesh global_DEBUG_block_mesh; 
global Material global_DEBUG_block_material;
global Model global_DEBUG_block_model;

global Model church;
global Texture2D churchTexture;
global RenderTexture2D target;
global Material matInstances;
global Material matDefault;

// Test Mesh for Shaders
//global Mesh cube;
//global Matrix *transforms;   // Pre-multiplied transformations passed to rlgl

const int screen_width = 800;
const int screen_height = 450;
/*
    on runtime
*/
internal void init() {
    InitWindow(screen_width, screen_height, "AlgoCraft3D");
    SetTargetFPS(60);
    DisableCursor(); // NOTE(cabarger): Also locks the cursor.
    init_shaders(matInstances, matDefault);

    // Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
    // Matrix *transforms = (Matrix *)RL_CALLOC(MAX_INSTANCES, sizeof(Matrix));
    // //Test Matrix
    // for (int i = 0; i < MAX_INSTANCES; i++)
    // {
    //     Matrix translation = MatrixTranslate((float)GetRandomValue(-50, 50), (float)GetRandomValue(-50, 50), (float)GetRandomValue(-50, 50));
    //     Vector3 axis = Vector3Normalize((Vector3){ (float)GetRandomValue(0, 360), (float)GetRandomValue(0, 360), (float)GetRandomValue(0, 360) });
    //     float angle = (float)GetRandomValue(0, 10)*DEG2RAD;
    //     Matrix rotation = MatrixRotate(axis, angle);
        
    //     transforms[i] = MatrixMultiply(rotation, translation);
    // }
    
    // Test church model
    target = LoadRenderTexture(screen_width, screen_height);

    church = LoadModel("./resources/church.obj");                 // Load OBJ model
    churchTexture = LoadTexture("./resources/church_diffuse.png"); // Load model texture (diffuse map)
    church.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = churchTexture;        // Set model diffuse texture
    //

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

    //- cabarger: Init world
    global_world = World::alloc_chunks();
    
    //- cabarger: Test fill blocks from Ian's code
    Chunk chunk = World::chunk_at_chunk_cor(&global_world, 0, 0);
    
    for (U8 block_x=0; block_x < CHUNK_W; ++block_x) {
        for (U8 block_y=0; block_y < CHUNK_W; ++block_y) {
            for (U8 block_z=0; block_z < CHUNK_H; ++block_z) {
                if (block_z == 10 || block_z == 11 || block_z == 12) {
                    // Block* block_p = 
                        // Chunk::block_at(&chunk, block_x, block_y, block_z);
                    // Block::set_value(block_p, 1);
                }
            }
        }
    }
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
    float cameraPos[3] = { global_camera.position.x, global_camera.position.y, global_camera.position.z };
    update_shaders(cameraPos);

}

#define SAM_DRAW 1
#if SAM_DRAW 
internal void draw() {
    BeginTextureMode(target);  
    ClearBackground(RAYWHITE);

    BeginMode3D(global_camera);

    // DrawMesh(cube, matDefault, MatrixTranslate(-10.0f, 0.0f, 0.0f));
    // DrawMeshInstanced(cube, matInstances, transforms, MAX_INSTANCES);
    // DrawMesh(cube, matDefault, MatrixTranslate(10.0f, 0.0f, 0.0f));
    // Church
    // DrawModel(
    //     global_DEBUG_block_model, 
    //     (Vector3){.x = 0.0f, .y = 0.0f, .z = 0.0f},  // Pos
    //     1.0f, // Scale
    //     BLUE // Tint
    // ); 
    DrawModel(church, { 0.0f, 0.0f, 0.0f }, 0.1f, WHITE);   // Draw 3d model with texture
    DrawGrid(10, 1.0f); 

    EndMode3D();
    EndTextureMode();

    BeginDrawing();
    ClearBackground(RAYWHITE);

    begin_shaders();
        DrawTextureRec(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2){ 200, 400 }, WHITE);
        DrawTextureRec(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2){ 0, 0 }, WHITE);
        DrawTextureRec(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2){ 0, 0 }, WHITE);
    end_shaders();

    // DRAW TEXT over 2d shapes and drawn texture
    DrawRectangle(0, 9, 580, 30, Fade(LIGHTGRAY, 0.7f));
    
    DrawText("(c) Church 3D model by Alberto Cano", screen_width - 200, screen_height - 20, 10, GRAY);
    DrawText("CURRENT POSTPRO SHADER:", 10, 15, 20, BLACK);
    DrawText(postproShaderText[get_shader_index()], 330, 15, 20, RED);
    DrawText("< >", 540, 10, 30, DARKBLUE);
    
    EndDrawing();
}
#else
#include "render.cpp"
#endif

int main(void) {
    
    init();
    while (!WindowShouldClose()) {
        update(GetFrameTime());
        draw();
    }
    UnloadTexture(churchTexture);        
    UnloadModel(church);             
    UnloadRenderTexture(target);    
    deload_shaders();
    // RL_FREE(transforms);
    CloseWindow();

    return 0;
}
