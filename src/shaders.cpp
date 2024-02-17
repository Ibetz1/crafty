#include "shaders.h"

namespace{
    Shader shaders[MAX_POSTPRO_SHADERS] = { 0 };
    int currentShader;
}

// void InitMesh()
// {
//     Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);

//     // Define transforms to be uploaded to GPU for instances
//     Matrix *transforms = (Matrix *)RL_CALLOC(MAX_INSTANCES, sizeof(Matrix));   // Pre-multiplied transformations passed to rlgl

//     // Translate and rotate cubes randomly
//     for (int i = 0; i < MAX_INSTANCES; i++)
//     {
//         Matrix translation = MatrixTranslate((float)GetRandomValue(-50, 50), (float)GetRandomValue(-50, 50), (float)GetRandomValue(-50, 50));
//         Vector3 axis = Vector3Normalize((Vector3){ (float)GetRandomValue(0, 360), (float)GetRandomValue(0, 360), (float)GetRandomValue(0, 360) });
//         float angle = (float)GetRandomValue(0, 10)*DEG2RAD;
//         Matrix rotation = MatrixRotate(axis, angle);
        
//         transforms[i] = MatrixMultiply(rotation, translation);
//     }
// }

void InitShaders()
{
    // Load all postpro shaders
    // NOTE 1: All postpro shader use the base vertex shader (DEFAULT_VERTEX_SHADER)
    // NOTE 2: We load the correct shader depending on GLSL version

    // NOTE: Defining 0 (NULL) for vertex shader forces usage of internal default vertex shader
    shaders[FX_GRAYSCALE] = LoadShader(0, TextFormat("resources/shaders/glsl%i/grayscale.fs", GLSL_VERSION));
    shaders[FX_POSTERIZATION] = LoadShader(0, TextFormat("resources/shaders/glsl%i/posterization.fs", GLSL_VERSION));
    shaders[FX_DREAM_VISION] = LoadShader(0, TextFormat("resources/shaders/glsl%i/dream_vision.fs", GLSL_VERSION));
    shaders[FX_PIXELIZER] = LoadShader(0, TextFormat("resources/shaders/glsl%i/pixelizer.fs", GLSL_VERSION));
    shaders[FX_CROSS_HATCHING] = LoadShader(0, TextFormat("resources/shaders/glsl%i/cross_hatching.fs", GLSL_VERSION));
    shaders[FX_CROSS_STITCHING] = LoadShader(0, TextFormat("resources/shaders/glsl%i/cross_stitching.fs", GLSL_VERSION));
    shaders[FX_PREDATOR_VIEW] = LoadShader(0, TextFormat("resources/shaders/glsl%i/predator.fs", GLSL_VERSION));
    shaders[FX_SCANLINES] = LoadShader(0, TextFormat("resources/shaders/glsl%i/scanlines.fs", GLSL_VERSION));
    shaders[FX_FISHEYE] = LoadShader(0, TextFormat("resources/shaders/glsl%i/fisheye.fs", GLSL_VERSION));
    shaders[FX_SOBEL] = LoadShader(0, TextFormat("resources/shaders/glsl%i/sobel.fs", GLSL_VERSION));
    shaders[FX_BLOOM] = LoadShader(0, TextFormat("resources/shaders/glsl%i/bloom.fs", GLSL_VERSION));
    shaders[FX_BLUR] = LoadShader(0, TextFormat("resources/shaders/glsl%i/blur.fs", GLSL_VERSION));

    currentShader = FX_GRAYSCALE;

    // Shader shader = LoadShader(TextFormat("resources/shaders/glsl%i/lighting_instancing.vs", GLSL_VERSION),
    //                            TextFormat("resources/shaders/glsl%i/lighting.fs", GLSL_VERSION));
    // // Get shader locations
    
    // shader.locs[SHADER_LOC_MATRIX_MVP] = GetShaderLocation(shader, "mvp");
    // shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
    // shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocationAttrib(shader, "instanceTransform");

    // // Set shader value: ambient light level
    // int ambientLoc = GetShaderLocation(shader, "ambient");
    // SetShaderValue(shader, ambientLoc, (float[4]){ 0.2f, 0.2f, 0.2f, 1.0f }, SHADER_UNIFORM_VEC4);

}
void UpdateShader()
{
    if (IsKeyPressed(KEY_J)) currentShader++;
    else if (IsKeyPressed(KEY_K)) currentShader--;

    if (currentShader >= MAX_POSTPRO_SHADERS) currentShader = 0;
    else if (currentShader < 0) currentShader = MAX_POSTPRO_SHADERS - 1;
}

int getCurrentShader()
{
    return currentShader;
}

void BeginShaders()
{
    BeginShaderMode(shaders[currentShader]);
}

void EndShaders()
{
    EndShaderMode();
}

void DeloadShaders()
{
    for (int i = 0; i < MAX_POSTPRO_SHADERS; i++) UnloadShader(shaders[i]);
    //UnloadShader(shade);
}
// void CreateShaders()
// {
//     CreateLight(LIGHT_DIRECTIONAL, (Vector3){ 50.0f, 50.0f, 0.0f }, Vector3Zero(), WHITE, shader);
// }