#include "raylib.h"
#include "rlights.h"
//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// ...

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
// ...

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
static int lightsCount = 0;    // Current amount of created lights

//----------------------------------------------------------------------------------
// Module specific Functions Declaration
//----------------------------------------------------------------------------------
// ...

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------

// Create a light and get shader locations
Light CreateLight(int type, Vector3 position, Vector3 target, Color color, Shader shader)
{
    Light light = { 0 };

    if (lightsCount < MAX_LIGHTS)
    {
        light.enabled = true;
        light.type = type;
        light.position = position;
        light.target = target;
        light.color = color;

        // NOTE: Lighting shader naming must be the provided ones
        light.enabledLoc = GetShaderLocation(shader, TextFormat("lights[%i].enabled", lightsCount));
        light.typeLoc = GetShaderLocation(shader, TextFormat("lights[%i].type", lightsCount));
        light.positionLoc = GetShaderLocation(shader, TextFormat("lights[%i].position", lightsCount));
        light.targetLoc = GetShaderLocation(shader, TextFormat("lights[%i].target", lightsCount));
        light.colorLoc = GetShaderLocation(shader, TextFormat("lights[%i].color", lightsCount));

        UpdateLightValues(shader, light);
        
        lightsCount++;
    }

    return light;
}

// Send light properties to shader
// NOTE: Light shader locations should be available 
void UpdateLightValues(Shader shader, Light light)
{
    // Send to shader light enabled state and type
    SetShaderValue(shader, light.enabledLoc, &light.enabled, SHADER_UNIFORM_INT);
    SetShaderValue(shader, light.typeLoc, &light.type, SHADER_UNIFORM_INT);

    // Send to shader light position values
    float position[3] = { light.position.x, light.position.y, light.position.z };
    SetShaderValue(shader, light.positionLoc, position, SHADER_UNIFORM_VEC3);

    // Send to shader light target position values
    float target[3] = { light.target.x, light.target.y, light.target.z };
    SetShaderValue(shader, light.targetLoc, target, SHADER_UNIFORM_VEC3);

    // Send to shader light color values
    float color[4] = { (float)light.color.r/(float)255, (float)light.color.g/(float)255, 
                       (float)light.color.b/(float)255, (float)light.color.a/(float)255 };
    SetShaderValue(shader, light.colorLoc, color, SHADER_UNIFORM_VEC4);
}
