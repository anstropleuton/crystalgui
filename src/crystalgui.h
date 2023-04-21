/*
 * Crystal GUI - Modern-looking Crystal-clear GUI library for raylib.
 *
 * Copyright (c) 2023 Anstro Pleuton
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef CRYSTALGUI_H
#define CRYSTALGUI_H

#include "raylib.h"

#include <stdlib.h>

#ifndef CRYSTALGUI_RESOURCE_PATH
#define CRYSTALGUI_RESOURCE_PATH "res/"
#endif // CRYSTALGUI_RESOURCE_PATH

#ifndef CRYSTALGUI_SHADER_PATH
#define CRYSTALGUI_SHADER_PATH CRYSTALGUI_RESOURCE_PATH "shaders/"
#endif // CRYSTALGUI_SHADER_PATH

/* -------------------------------------------------------------------------- */
/*            Misc functions useful for (and used by) Crystal GUI             */
/* -------------------------------------------------------------------------- */

// Draw texture to fit the screen
static void DrawTextureFullscreen(Texture texture, bool isRenderTexture)
{
    DrawTexturePro(texture, (Rectangle)
    {
        0.0f, 0.0f, texture.width, texture.height * (isRenderTexture ? -1.0f : 1.0f)
    }, (Rectangle)
    {
        0.0f, 0.0f, GetScreenWidth(), GetScreenHeight()
    }, (Vector2)
    {
        0.0f, 0.0f
    }, 0.0f, WHITE);
}

/* -------------------------------------------------------------------------- */
/*                     Crystal GUI Core Enum Definitions                      */
/* -------------------------------------------------------------------------- */

// Blur shader location index
typedef enum CguiBlurShaderLocationIndex
{
    CRYSTALGUI_BLUR_SHADER_LOC_RESOLUTION,
    CRYSTALGUI_BLUR_SHADER_LOC_BLUR_RADIUS,
    CRYSTALGUI_BLUR_SHADER_LOC_MAX
};

// Shadow shader location index
typedef enum CguiShadowShaderLocationIndex
{
    CRYSTALGUI_SHADOW_SHADER_LOC_RESOLUTION,
    CRYSTALGUI_SHADOW_SHADER_LOC_RECTANGLE,
    CRYSTALGUI_SHADOW_SHADER_LOC_ROUNDNESS,
    CRYSTALGUI_SHADOW_SHADER_LOC_SHADOW_RADIUS,
    CRYSTALGUI_SHADOW_SHADER_LOC_SHADOW_SIZE,
    CRYSTALGUI_SHADOW_SHADER_LOC_SHADOW_OFFSET,
    CRYSTALGUI_SHADOW_SHADER_LOC_SHADOW_COLOR,
    CRYSTALGUI_SHADOW_SHADER_LOC_MAX
};

// Rectangle shader location index
typedef enum CguiRectangleShaderLocationIndex
{
    CRYSTALGUI_RECTANGLE_SHADER_LOC_RESOLUTION,
    CRYSTALGUI_RECTANGLE_SHADER_LOC_RECTANGLE,
    CRYSTALGUI_RECTANGLE_SHADER_LOC_ROUNDNESS,
    CRYSTALGUI_RECTANGLE_SHADER_LOC_RECTANGLE_TINT,
    CRYSTALGUI_RECTANGLE_SHADER_LOC_MAX
};

/* -------------------------------------------------------------------------- */
/*                    Crystal GUI Core Struct Definitions                     */
/* -------------------------------------------------------------------------- */

// Font properties for rendering text
typedef struct CguiTextProperties
{
    Font font;                      // Font to be used in text
    float size;                     // Size of the font
    float spacing;                  // Spacing for each letters in text
    Color color;                    // Color of the text

    Color shadowColor;              // Text shadow color (usually opposite color)
    float shadowBlurRadius;         // Blur radius (shadow diffusion) of text shadow, uses Blur Shader
    Vector2 shadowOffset;           // Position offset of text shadow
} CguiTextProperties;

// All the default shader variables used by Crystal GUI
typedef struct CguiShaderVariables
{
    // Shader properties contains shader variables that do not depend on the shader use
    // Example: "rectangle" is highly dependent on where and how the shader is used
    // which is excluded from properties list
    struct ShaderProperties
    {
        Vector2 resolution;
        float blurRadius;
        float roundness;
        float shadowRadius;
        float shadowSize;
        Vector2 shadowOffset;
    } shaderProperties;

    struct BlurShaderVariables
    {
        float resolution[2];
        float blurRadius;
    } blurShaderVariables;

    struct ShadowShaderVariables
    {
        float resolution[2];
        float rectangle[4];         // Excluded
        float roundness;
        float shadowRadius;
        float shadowSize;
        float shadowOffset[2];
        float shadowColor[4];       // Excluded
    } shadowShaderVariables;

    struct RectangleShaderVariables
    {
        float resolution[2];
        float rectangle[4];         // Excluded
        float roundness;
        float rectangleTint[4];     // Excluded
    } rectangleShaderVariables;
} CguiShaderVariables;

// Main Crystal GUI Instance
typedef struct CguiInstance
{
    Shader blurShader;              // Blur shader
    Shader shadowShader;            // Shadow shader
    Shader rectangleShader;         // Shader to draw rounded rectangle

    // All the default shader variables used by Crystal GUI
    CguiShaderVariables shaderVariables;

    int blurShaderLocs[CRYSTALGUI_BLUR_SHADER_LOC_MAX]; // Blur shader custom uniform locations
    int shadowShaderLocs[CRYSTALGUI_SHADOW_SHADER_LOC_MAX]; // Shadow shader custom uniform locations
    int rectangleShaderLocs[CRYSTALGUI_RECTANGLE_SHADER_LOC_MAX]; // Rectangle shader custom uniform locations

    RenderTexture blurInputRenderTexture; // Input frame buffer to be blurred
    RenderTexture blurOutputRenderTexture; // Blurred frame buffer
    RenderTexture textRenderTexture; // Render texture used to temporarily store text to be blurred later

    CguiTextProperties textProperties; // Font properties for instance
} CguiInstance;

/* -------------------------------------------------------------------------- */
/*                   Crystal GUI Core Function Definitions                    */
/* -------------------------------------------------------------------------- */

// Load default shaders
static void CguiLoadDefaultShaders(CguiInstance *instance)
{
    if (!instance) return;

    instance->blurShader = LoadShader(CRYSTALGUI_SHADER_PATH "blurShader.vs", CRYSTALGUI_SHADER_PATH "blurShader.fs");
    instance->shadowShader = LoadShader(CRYSTALGUI_SHADER_PATH "shadowShader.vs", CRYSTALGUI_SHADER_PATH "shadowShader.fs");
    instance->rectangleShader = LoadShader(CRYSTALGUI_SHADER_PATH "rectangleShader.vs", CRYSTALGUI_SHADER_PATH "rectangleShader.fs");
}

// Unload default shaders
static void CguiUnloadDefaultShaders(CguiInstance *instance)
{
    if (!instance) return;

    UnloadShader(instance->blurShader);
    UnloadShader(instance->shadowShader);
    UnloadShader(instance->rectangleShader);
}

// Set default shader property values
static void CguiSetDefaultShaderProperties(CguiInstance *instance)
{
    if (!instance) return;

    instance->shaderVariables.shaderProperties.resolution = (Vector2)
    {
        GetScreenWidth(), GetScreenHeight()
    };

    instance->shaderVariables.shaderProperties.blurRadius = 10.0f;
    instance->shaderVariables.shaderProperties.roundness = 5.0f;
    instance->shaderVariables.shaderProperties.shadowRadius = 30.0f;
    instance->shaderVariables.shaderProperties.shadowSize = -10.0f;

    instance->shaderVariables.shaderProperties.shadowOffset = (Vector2)
    {
        0.0f, -10.0f
    };
}

// Set property values to shader variables
static void CguiSetDefaultShaderVariables(CguiInstance *instance)
{
    if (!instance) return;

    instance->shaderVariables.blurShaderVariables.resolution[0] = instance->shaderVariables.shaderProperties.resolution.x;
    instance->shaderVariables.blurShaderVariables.resolution[1] = instance->shaderVariables.shaderProperties.resolution.y;
    instance->shaderVariables.shadowShaderVariables.resolution[0] = instance->shaderVariables.shaderProperties.resolution.x;
    instance->shaderVariables.shadowShaderVariables.resolution[1] = instance->shaderVariables.shaderProperties.resolution.y;
    instance->shaderVariables.rectangleShaderVariables.resolution[0] = instance->shaderVariables.shaderProperties.resolution.x;
    instance->shaderVariables.rectangleShaderVariables.resolution[1] = instance->shaderVariables.shaderProperties.resolution.y;
    instance->shaderVariables.blurShaderVariables.blurRadius = instance->shaderVariables.shaderProperties.blurRadius;
    instance->shaderVariables.shadowShaderVariables.roundness = instance->shaderVariables.shaderProperties.roundness;
    instance->shaderVariables.rectangleShaderVariables.roundness = instance->shaderVariables.shaderProperties.roundness;
    instance->shaderVariables.shadowShaderVariables.shadowRadius = instance->shaderVariables.shaderProperties.shadowRadius;
    instance->shaderVariables.shadowShaderVariables.shadowSize = instance->shaderVariables.shaderProperties.shadowSize;
    instance->shaderVariables.shadowShaderVariables.shadowOffset[0] = instance->shaderVariables.shaderProperties.shadowOffset.x;
    instance->shaderVariables.shadowShaderVariables.shadowOffset[1] = instance->shaderVariables.shaderProperties.shadowOffset.y;
}

// Get default shader locations
static void CguiSetDefaultShaderLocations(CguiInstance *instance)
{
    if (!instance) return;

    instance->blurShaderLocs[CRYSTALGUI_BLUR_SHADER_LOC_RESOLUTION] = GetShaderLocation(instance->blurShader, "resolution");
    instance->blurShaderLocs[CRYSTALGUI_BLUR_SHADER_LOC_BLUR_RADIUS] = GetShaderLocation(instance->blurShader, "blurRadius");

    instance->shadowShaderLocs[CRYSTALGUI_SHADOW_SHADER_LOC_RESOLUTION] = GetShaderLocation(instance->shadowShader, "resolution");
    instance->shadowShaderLocs[CRYSTALGUI_SHADOW_SHADER_LOC_RECTANGLE] = GetShaderLocation(instance->shadowShader, "rectangle");
    instance->shadowShaderLocs[CRYSTALGUI_SHADOW_SHADER_LOC_ROUNDNESS] = GetShaderLocation(instance->shadowShader, "roundness");
    instance->shadowShaderLocs[CRYSTALGUI_SHADOW_SHADER_LOC_SHADOW_RADIUS] = GetShaderLocation(instance->shadowShader, "shadowRadius");
    instance->shadowShaderLocs[CRYSTALGUI_SHADOW_SHADER_LOC_SHADOW_SIZE] = GetShaderLocation(instance->shadowShader, "shadowSize");
    instance->shadowShaderLocs[CRYSTALGUI_SHADOW_SHADER_LOC_SHADOW_OFFSET] = GetShaderLocation(instance->shadowShader, "shadowOffset");
    instance->shadowShaderLocs[CRYSTALGUI_SHADOW_SHADER_LOC_SHADOW_COLOR] = GetShaderLocation(instance->shadowShader, "shadowColor");

    instance->rectangleShaderLocs[CRYSTALGUI_RECTANGLE_SHADER_LOC_RESOLUTION] = GetShaderLocation(instance->rectangleShader, "resolution");
    instance->rectangleShaderLocs[CRYSTALGUI_RECTANGLE_SHADER_LOC_RECTANGLE] = GetShaderLocation(instance->rectangleShader, "rectangle");
    instance->rectangleShaderLocs[CRYSTALGUI_RECTANGLE_SHADER_LOC_ROUNDNESS] = GetShaderLocation(instance->rectangleShader, "roundness");
    instance->rectangleShaderLocs[CRYSTALGUI_RECTANGLE_SHADER_LOC_RECTANGLE_TINT] = GetShaderLocation(instance->rectangleShader, "rectangleTint");
}

// Apply shader variables to shader
static void CguiApplyShaderVariables(CguiInstance *instance)
{
    if (!instance) return;

    SetShaderValue(instance->blurShader, instance->blurShaderLocs[CRYSTALGUI_BLUR_SHADER_LOC_RESOLUTION], instance->shaderVariables.blurShaderVariables.resolution, SHADER_UNIFORM_VEC2);
    SetShaderValue(instance->blurShader, instance->blurShaderLocs[CRYSTALGUI_BLUR_SHADER_LOC_BLUR_RADIUS], &instance->shaderVariables.blurShaderVariables.blurRadius, SHADER_UNIFORM_FLOAT);

    SetShaderValue(instance->shadowShader, instance->shadowShaderLocs[CRYSTALGUI_SHADOW_SHADER_LOC_RESOLUTION], instance->shaderVariables.shadowShaderVariables.resolution, SHADER_UNIFORM_VEC2);
    SetShaderValue(instance->shadowShader, instance->shadowShaderLocs[CRYSTALGUI_SHADOW_SHADER_LOC_ROUNDNESS], &instance->shaderVariables.shadowShaderVariables.roundness, SHADER_UNIFORM_FLOAT);
    SetShaderValue(instance->shadowShader, instance->shadowShaderLocs[CRYSTALGUI_SHADOW_SHADER_LOC_SHADOW_RADIUS], &instance->shaderVariables.shadowShaderVariables.shadowRadius, SHADER_UNIFORM_FLOAT);
    SetShaderValue(instance->shadowShader, instance->shadowShaderLocs[CRYSTALGUI_SHADOW_SHADER_LOC_SHADOW_SIZE], &instance->shaderVariables.shadowShaderVariables.shadowSize, SHADER_UNIFORM_FLOAT);
    SetShaderValue(instance->shadowShader, instance->shadowShaderLocs[CRYSTALGUI_SHADOW_SHADER_LOC_SHADOW_OFFSET], instance->shaderVariables.shadowShaderVariables.shadowOffset, SHADER_UNIFORM_VEC2);

    SetShaderValue(instance->rectangleShader, instance->rectangleShaderLocs[CRYSTALGUI_RECTANGLE_SHADER_LOC_RESOLUTION], instance->shaderVariables.rectangleShaderVariables.resolution, SHADER_UNIFORM_VEC2);
    SetShaderValue(instance->rectangleShader, instance->rectangleShaderLocs[CRYSTALGUI_RECTANGLE_SHADER_LOC_ROUNDNESS], &instance->shaderVariables.rectangleShaderVariables.roundness, SHADER_UNIFORM_FLOAT);
}

// Load render textures
static void CguiLoadRenderTextures(CguiInstance *instance)
{
    if (!instance) return NULL;

    instance->blurInputRenderTexture = LoadRenderTexture(instance->shaderVariables.shaderProperties.resolution.x, instance->shaderVariables.shaderProperties.resolution.y);
    instance->blurOutputRenderTexture = LoadRenderTexture(instance->shaderVariables.shaderProperties.resolution.x, instance->shaderVariables.shaderProperties.resolution.y);
    instance->textRenderTexture = LoadRenderTexture(instance->shaderVariables.shaderProperties.resolution.x, instance->shaderVariables.shaderProperties.resolution.y);

    // Clear all the render textures
    BeginTextureMode(instance->blurInputRenderTexture);
    ClearBackground(BLANK);
    EndTextureMode();
    BeginTextureMode(instance->blurOutputRenderTexture);
    ClearBackground(BLANK);
    EndTextureMode();
    BeginTextureMode(instance->textRenderTexture);
    ClearBackground(BLANK);
    EndTextureMode();
}

// Unload render texture
static void CguiUnloadRenderTextures(CguiInstance *instance)
{
    if (!instance) return;

    UnloadRenderTexture(instance->blurInputRenderTexture);
    UnloadRenderTexture(instance->blurOutputRenderTexture);
    UnloadRenderTexture(instance->textRenderTexture);
}

// Set default font property values
static void CguiSetDefaultFontProperties(CguiInstance *instance)
{
    if (!instance) return;

    instance->textProperties.size = 20.0f;
    instance->textProperties.spacing = 1.0f;
    instance->textProperties.color = WHITE;
    instance->textProperties.font = LoadFontEx(CRYSTALGUI_RESOURCE_PATH "Roboto-Regular.ttf", instance->textProperties.size, NULL, 0);

    instance->textProperties.shadowColor = ColorAlpha(BLACK, 1.0f);
    instance->textProperties.shadowBlurRadius = 2.0f;
    instance->textProperties.shadowOffset = CLITERAL(Vector2){ 0.0f, -2.0f };
}

// Create a new instance of Crystal GUI
static CguiInstance *CguiCreateInstanceInstance(void)
{
    CguiInstance *instance = (CguiInstance *)malloc(sizeof(CguiInstance));
    if (!instance) return NULL;

    instance->shaderVariables.shaderProperties.resolution = (Vector2)
    {
        GetScreenWidth(), GetScreenHeight()
    };

    CguiLoadRenderTextures(instance);

    // Load default shader
    CguiLoadDefaultShaders(instance);
    CguiSetDefaultShaderProperties(instance);
    CguiSetDefaultShaderVariables(instance);
    CguiSetDefaultShaderLocations(instance);
    CguiApplyShaderVariables(instance);

    CguiSetDefaultFontProperties(instance);

    return instance;
}

// Delete the created Crystal GUI instance
static void CguiDeleteInstance(CguiInstance *instance)
{
    if (!instance) return;

    CguiUnloadRenderTextures(instance);
    CguiUnloadDefaultShaders(instance);
    UnloadFont(instance->textProperties.font);
    free(instance);
}

// Update shader and render texture resolution (reloads render texture)
static void CguiUpdateResolution(CguiInstance *instance)
{
    if (!instance) return;

    instance->shaderVariables.shaderProperties.resolution = (Vector2)
    {
        GetScreenWidth(), GetScreenHeight()
    };

    CguiUnloadRenderTextures(instance);
    CguiLoadRenderTextures(instance);

    instance->shaderVariables.blurShaderVariables.resolution[0] = instance->shaderVariables.shaderProperties.resolution.x;
    instance->shaderVariables.blurShaderVariables.resolution[1] = instance->shaderVariables.shaderProperties.resolution.y;
    instance->shaderVariables.shadowShaderVariables.resolution[0] = instance->shaderVariables.shaderProperties.resolution.x;
    instance->shaderVariables.shadowShaderVariables.resolution[1] = instance->shaderVariables.shaderProperties.resolution.y;
    instance->shaderVariables.rectangleShaderVariables.resolution[0] = instance->shaderVariables.shaderProperties.resolution.x;
    instance->shaderVariables.rectangleShaderVariables.resolution[1] = instance->shaderVariables.shaderProperties.resolution.y;

    SetShaderValue(instance->blurShader, instance->blurShaderLocs[CRYSTALGUI_BLUR_SHADER_LOC_RESOLUTION], instance->shaderVariables.blurShaderVariables.resolution, SHADER_UNIFORM_VEC2);
    SetShaderValue(instance->shadowShader, instance->shadowShaderLocs[CRYSTALGUI_SHADOW_SHADER_LOC_RESOLUTION], instance->shaderVariables.shadowShaderVariables.resolution, SHADER_UNIFORM_VEC2);
    SetShaderValue(instance->rectangleShader, instance->rectangleShaderLocs[CRYSTALGUI_RECTANGLE_SHADER_LOC_RESOLUTION], instance->shaderVariables.rectangleShaderVariables.resolution, SHADER_UNIFORM_VEC2);
}

// Apply blur effect to the input frame
static void CguiBlurRenderTexture(CguiInstance *instance)
{
    if (!instance) return;

    BeginTextureMode(instance->blurOutputRenderTexture);
    BeginShaderMode(instance->blurShader);
    DrawTextureFullscreen(instance->blurInputRenderTexture.texture, true);
    EndShaderMode();
    EndTextureMode();
}

/* -------------------------------------------------------------------------- */
/*                     Crystal GUI Base Enum Definitions                      */
/* -------------------------------------------------------------------------- */
// It's quite lonely here...

/* -------------------------------------------------------------------------- */
/*                    Crystal GUI Base Struct Definitions                     */
/* -------------------------------------------------------------------------- */
// It's quite lonely here as well...

/* -------------------------------------------------------------------------- */
/*                        Crystal GUI Base Functions                          */
/* -------------------------------------------------------------------------- */

// Draw a rectangle using default shaders
static void CguiDrawShaderRectangle(CguiInstance *instance, Rectangle rectangle, Color rectangleColor, Color shadowColor)
{
    if (!instance) return;

    float shader_shadowColor[4] = { shadowColor.r / 255.0f, shadowColor.g / 255.0f, shadowColor.b / 255.0f, shadowColor.a / 255.0f };
    SetShaderValue(instance->shadowShader, instance->shadowShaderLocs[CRYSTALGUI_SHADOW_SHADER_LOC_SHADOW_COLOR], shader_shadowColor, SHADER_UNIFORM_VEC4);

    float shader_rectangleColor[4] = { rectangleColor.r / 255.0f, rectangleColor.g / 255.0f, rectangleColor.b / 255.0f, rectangleColor.a / 255.0f };
    SetShaderValue(instance->rectangleShader, instance->rectangleShaderLocs[CRYSTALGUI_RECTANGLE_SHADER_LOC_RECTANGLE_TINT], shader_rectangleColor, SHADER_UNIFORM_VEC4);

    float shader_rectangle[4] = { rectangle.x, instance->shaderVariables.shaderProperties.resolution.y - rectangle.y - rectangle.height, rectangle.width, rectangle.height };
    SetShaderValue(instance->shadowShader, instance->shadowShaderLocs[CRYSTALGUI_SHADOW_SHADER_LOC_RECTANGLE], shader_rectangle, SHADER_UNIFORM_VEC4);
    SetShaderValue(instance->rectangleShader, instance->rectangleShaderLocs[CRYSTALGUI_RECTANGLE_SHADER_LOC_RECTANGLE], shader_rectangle, SHADER_UNIFORM_VEC4);

    BeginShaderMode(instance->shadowShader);
    DrawTextureFullscreen(instance->blurOutputRenderTexture.texture, true);
    EndShaderMode();

    BeginShaderMode(instance->rectangleShader);
    DrawTextureFullscreen(instance->blurOutputRenderTexture.texture, true);
    EndShaderMode();
}

// Draw text using default shaders
static void CguiDrawShaderText(CguiInstance *instance, const char *text, Rectangle rectangle)
{
    if (!instance) return;

    Vector2 measuredText = MeasureTextEx(instance->textProperties.font, text, instance->textProperties.size, instance->textProperties.spacing);
    Vector2 position = { rectangle.x + (rectangle.width - measuredText.x) / 2.0f, rectangle.y + (rectangle.height - measuredText.y) / 2.0f };

    position.x -= instance->textProperties.shadowOffset.x;
    position.y -= instance->textProperties.shadowOffset.y;
    BeginTextureMode(instance->textRenderTexture);
    DrawTextEx(instance->textProperties.font, text, position, instance->textProperties.size, instance->textProperties.spacing, instance->textProperties.shadowColor);
    EndTextureMode();
    position.x += instance->textProperties.shadowOffset.x;
    position.y += instance->textProperties.shadowOffset.y;

    // Temporarily set font properties as blur shader variables
    SetShaderValue(instance->blurShader, instance->blurShaderLocs[CRYSTALGUI_BLUR_SHADER_LOC_BLUR_RADIUS], &instance->textProperties.shadowBlurRadius, SHADER_UNIFORM_FLOAT);

    BeginShaderMode(instance->blurShader);
    DrawTextureFullscreen(instance->textRenderTexture.texture, true);
    EndShaderMode();

    // Reset blur shader variables
    SetShaderValue(instance->blurShader, instance->blurShaderLocs[CRYSTALGUI_BLUR_SHADER_LOC_BLUR_RADIUS], &instance->shaderVariables.blurShaderVariables.blurRadius, SHADER_UNIFORM_FLOAT);

    DrawTextEx(instance->textProperties.font, text, position, instance->textProperties.size, instance->textProperties.spacing, instance->textProperties.color);
}

#endif // CRYSTALGUI_H
