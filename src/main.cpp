#include "raylib.h"

/*
    on runtime
*/
void init() {

}

<<<<<<< HEAD
/*
    pre-render pass
*/
void update(float dt) {
    
}

/*
    render pass
*/
void draw() {

}

int main(void) {
    init();

=======
// Test co

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
>>>>>>> 502bbe3 (test co)
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(60);
    init();

    while (!WindowShouldClose()) {
        update(GetFrameTime());
        BeginDrawing();
            ClearBackground(RAYWHITE);
            draw();
        EndDrawing();
    }
    
    CloseWindow();

    return 0;
}
