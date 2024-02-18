#include "shaders.hpp"

Shader shaders[MAX_POSTPRO_SHADERS] = { 0 };
Light lights[MAX_LIGHT] = { 0 };
Shader lighting;

Material matInstances;
Material matDefault;
Matrix *transforms = (Matrix *)RL_CALLOC(MAX_INSTANCES, sizeof(Matrix));
Mesh cube;

int currentShader;
int totalLights = 0;

// To create shaders
// Set each material.shader = lighting;
// Create lights with create_light and pass in required parameters.

void init_shaders()
{
    // All post-processing shaders
    shaders[FX_BASE] = LoadShader(0, TextFormat("resources/shaders/glsl%i/base.fs", GLSL_VERSION));
    shaders[FX_GRAYSCALE] = LoadShader(0, TextFormat("resources/shaders/glsl%i/grayscale.fs", GLSL_VERSION));
    shaders[FX_POSTERIZATION] = LoadShader(0, TextFormat("resources/shaders/glsl%i/posterization.fs", GLSL_VERSION));
    shaders[FX_FISHEYE] = LoadShader(0, TextFormat("resources/shaders/glsl%i/fisheye.fs", GLSL_VERSION));
    shaders[FX_SOBEL] = LoadShader(0, TextFormat("resources/shaders/glsl%i/sobel.fs", GLSL_VERSION));
    shaders[FX_BLOOM] = LoadShader(0, TextFormat("resources/shaders/glsl%i/bloom.fs", GLSL_VERSION));
    shaders[FX_BLUR] = LoadShader(0, TextFormat("resources/shaders/glsl%i/blur.fs", GLSL_VERSION));

    lighting = LoadShader(TextFormat("resources/shaders/glsl%i/lighting_instancing.vs", GLSL_VERSION),
                            TextFormat("resources/shaders/glsl%i/lighting.fs", GLSL_VERSION));
    currentShader = FX_BASE;

    // Initalize ambient light and lighting shader
    lighting.locs[SHADER_LOC_MATRIX_MVP] = GetShaderLocation(lighting, "mvp");
    lighting.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocationAttrib(lighting, "instanceTransform");
    lighting.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(lighting, "viewPos");


    // Ambient light ( Can see it in pitch black )
    int ambientLoc = GetShaderLocation(lighting, "ambient");
    SetShaderValue(lighting, ambientLoc, (float[4]){ 0.1f, 0.1f, 0.1f, 1.0f }, SHADER_UNIFORM_VEC4);
    test_mesh_shaders();
}

// LIGHT_POINT, LIGHT_DIRECTIONAL
void create_light(int type, Vector3 position, Vector3 target, Color color)
{
    if (totalLights < MAX_LIGHT)
    {
        lights[totalLights] = CreateLight(type, position, target, color, lighting);
        totalLights++;
    }
}

void draw_lights()
{
    for (int i = 0; i < totalLights; i++)
    {
        if (lights[i].enabled) DrawSphereEx(lights[i].position, 0.2f, 8, 8, lights[i].color);
        else DrawSphereWires(lights[i].position, 0.2f, 8, 8, ColorAlpha(lights[i].color, 0.3f));
    }
}


void update_shaders(Camera * camera)
{
    SetShaderValue(lighting, lighting.locs[SHADER_LOC_VECTOR_VIEW], &camera->position.x, SHADER_UNIFORM_VEC3);
    // Lights
    if (IsKeyPressed(KEY_Y)) { lights[0].enabled = !lights[0].enabled; }
    if (IsKeyPressed(KEY_G)) { lights[1].enabled = !lights[1].enabled; }

    for (int i = 0; i < MAX_LIGHT; i++) UpdateLightValues(lighting, lights[i]);
    
    // Change post processing shader
    if (IsKeyPressed(KEY_J)) currentShader++;
    else if (IsKeyPressed(KEY_K)) currentShader--;

    if (currentShader >= MAX_POSTPRO_SHADERS) currentShader = 0;
    else if (currentShader < 0) currentShader = MAX_POSTPRO_SHADERS - 1;
}

void test_mesh_shaders()
{
    cube = GenMeshCube(1.0f, 1.0f, 1.0f);

    // Define transforms to be uploaded to GPU for instances
    // Translate and rotate cubes randomly
    for (int i = 0; i < MAX_INSTANCES; i++)
    {
        Matrix translation = MatrixTranslate((float)GetRandomValue(-50, 50), (float)GetRandomValue(-50, 50), (float)GetRandomValue(-50, 50));
        Vector3 axis = Vector3Normalize((Vector3){ (float)GetRandomValue(0, 360), (float)GetRandomValue(0, 360), (float)GetRandomValue(0, 360) });
        float angle = (float)GetRandomValue(0, 10)*DEG2RAD;
        Matrix rotation = MatrixRotate(axis, angle);
        
        transforms[i] = MatrixMultiply(rotation, translation);
    }
    
    matInstances = LoadMaterialDefault();
    matInstances.shader = lighting;
    matInstances.maps[MATERIAL_MAP_DIFFUSE].color = ORANGE;

    matDefault = LoadMaterialDefault();
    matDefault.maps[MATERIAL_MAP_DIFFUSE].color = BLUE;
}

void draw_shaders()
{
    draw_lights();
    // Draw cube mesh with default material (BLUE)
    DrawMesh(cube, matDefault, MatrixTranslate(-10.0f, 0.0f, 0.0f));

    // Draw meshes instanced using material containing instancing shader (RED + lighting),
    // updated in GPU every frame, so we can animate the different mesh instances
    DrawMeshInstanced(cube, matInstances, transforms, MAX_INSTANCES);

    // Draw cube mesh with default material (BLUE)
    DrawMesh(cube, matDefault, MatrixTranslate(10.0f, 0.0f, 0.0f));
}

// Shader Mode
void begin_shaders()
{
    BeginShaderMode(shaders[currentShader]);
}

void end_shaders()
{
    EndShaderMode();
}

void deload_shaders()
{
    for (int i = 0; i < MAX_POSTPRO_SHADERS; i++) UnloadShader(shaders[i]);
}

void draw_current_shader(int screen_width, int screen_height)
{
    DrawRectangle(0, 9, 580, 30, Fade(LIGHTGRAY, 0.7f));
    
    DrawText("(c) Church 3D model by Alberto Cano", screen_width - 200, screen_height - 20, 10, GRAY);
    DrawText("CURRENT POSTPRO SHADER:", 10, 15, 20, BLACK);
    DrawText(postproShaderText[get_shader_index()], 330, 15, 20, RED);
    DrawText("< >", 540, 10, 30, DARKBLUE);
}

// Auxiliary
Shader get_lighting_shader()
{
    return lighting;
}

// Takes in a value like "FX_BLUR"
Shader get_shader(int i)
{
    if (i > 0 && i < MAX_POSTPRO_SHADERS)
    return shaders[i];
    return get_current_shader();
}

Shader get_current_shader()
{
    return shaders[currentShader];
}

int get_shader_index()
{
    return currentShader;
}
