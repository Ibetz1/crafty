#include "raylib.h"

/*
    on runtime
*/
void init() {

}

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
`   init();

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
