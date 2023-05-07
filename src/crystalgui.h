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

/* -------------------------------------------------------------------------- */
/*                          Crystal GUI Definitions                           */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*            Misc functions useful for (and used by) Crystal GUI             */
/* -------------------------------------------------------------------------- */

#if defined(__cplusplus)
extern "C" {            // Prevents name mangling of functions
#endif
void DrawTextureFullscreen(Texture texture, bool isRenderTexture);              // Draw texture to fit the screen
Color ApplyHSVOnColor(Color color, Vector3 hsv);                                // Apply (add as shifts) HSV Values on the color
#if defined(__cplusplus)
}
#endif

/* -------------------------------------------------------------------------- */
/*                     Crystal GUI Core Enum Definitions                      */
/* -------------------------------------------------------------------------- */

// Blur shader location index
typedef enum CguiBlurShaderLocationIndex
{
    CRYSTALGUI_BLUR_SHADER_LOC_RESOLUTION,
    CRYSTALGUI_BLUR_SHADER_LOC_BLUR_RADIUS,
    CRYSTALGUI_BLUR_SHADER_LOC_MAX
} CguiBlurShaderLocationIndex;

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
} CguiShadowShaderLocationIndex;

// Rectangle shader location index
typedef enum CguiRectangleShaderLocationIndex
{
    CRYSTALGUI_RECTANGLE_SHADER_LOC_RESOLUTION,
    CRYSTALGUI_RECTANGLE_SHADER_LOC_RECTANGLE,
    CRYSTALGUI_RECTANGLE_SHADER_LOC_ROUNDNESS,
    CRYSTALGUI_RECTANGLE_SHADER_LOC_RECTANGLE_TINT,
    CRYSTALGUI_RECTANGLE_SHADER_LOC_MAX
} CguiRectangleShaderLocationIndex;

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
    // which is excluded from properties struct
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
        float rectangle[4];         // Excluded from properties struct
        float roundness;
        float shadowRadius;
        float shadowSize;
        float shadowOffset[2];
        float shadowColor[4];       // Excluded from properties struct
    } shadowShaderVariables;

    struct RectangleShaderVariables
    {
        float resolution[2];
        float rectangle[4];         // Excluded from properties struct
        float roundness;
        float rectangleTint[4];     // Excluded from properties struct
    } rectangleShaderVariables;
} CguiShaderVariables;

// Main theme colors
typedef struct CguiThemeColors
{
    Color accent;
    Color normal;
    Color disabled;
    Color alert;
    Color warning;
    Color danger;

    Vector3 focused;
    Vector3 pressed;

    Color shadowColor;
} CguiThemeColors;

// Main Crystal GUI Instance, used in most of the functions
typedef struct CguiInstance
{
    Shader blurShader;              // Blur shader
    Shader shadowShader;            // Shadow shader
    Shader rectangleShader;         // Shader to draw rounded rectangle

    CguiShaderVariables shaderVariables;    // All the default shader variables used by Crystal GUI

    int blurShaderLocs[CRYSTALGUI_BLUR_SHADER_LOC_MAX];             // Blur shader custom uniform locations
    int shadowShaderLocs[CRYSTALGUI_SHADOW_SHADER_LOC_MAX];         // Shadow shader custom uniform locations
    int rectangleShaderLocs[CRYSTALGUI_RECTANGLE_SHADER_LOC_MAX];   // Rectangle shader custom uniform locations

    RenderTexture blurInputRenderTexture;   // Input frame buffer to be blurred
    RenderTexture blurOutputRenderTexture;  // Blurred frame buffer
    RenderTexture textRenderTexture;        // Render texture used to temporarily store text to be blurred later

    CguiTextProperties textProperties;  // Font properties
    CguiThemeColors themeColors;        // All the colors

    float focusTimerSpeed;          // Focus Fade speed
    float colorTransparency;        // Transparency of color ranging from 0.0f to 1.0f
} CguiInstance;

/* -------------------------------------------------------------------------- */
/*                   Crystal GUI Core Function Definitions                    */
/* -------------------------------------------------------------------------- */

#if defined(__cplusplus)
extern "C" {            // Prevents name mangling of functions
#endif
void CguiLoadDefaultShaders(CguiInstance *instance);                            // Load default shaders
void CguiUnloadDefaultShaders(CguiInstance *instance);                          // Unload default shaders
void CguiSetDefaultShaderProperties(CguiInstance *instance);                    // Set default shader property values
void CguiSetDefaultShaderVariables(CguiInstance *instance);                     // Set property values to shader variables
void CguiSetDefaultShaderLocations(CguiInstance *instance);                     // Get default shader locations
void CguiApplyShaderVariables(CguiInstance *instance);                          // Apply shader variables to shader
void CguiLoadRenderTextures(CguiInstance *instance);                            // Load render textures
void CguiUnloadRenderTextures(CguiInstance *instance);                          // Unload render texture
void CguiSetDefaultFontProperties(CguiInstance *instance);                      // Set default font property values
void CguiSetDefaultThemeColors(CguiInstance *instance);                         // Set the default theme colors
CguiInstance *CguiCreateInstance(void);                                         // Create a new instance of Crystal GUI
void CguiDeleteInstance(CguiInstance *instance);                                // Delete the created Crystal GUI instance
void CguiUpdateResolution(CguiInstance *instance);                              // Update shader and render texture resolution (reloads render texture)
void CguiBlurRenderTexture(CguiInstance *instance);                             // Apply blur effect to the input frame
#if defined(__cplusplus)
}
#endif

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

#if defined(__cplusplus)
extern "C" {            // Prevents name mangling of functions
#endif
void CguiDrawShaderRectangle(CguiInstance *instance, Rectangle rectangle, Color rectangleColor, Color shadowColor); // Draw a rectangle using default shaders
void CguiDrawShaderText(CguiInstance *instance, const char *text, Rectangle rectangle);                             // Draw text using default shaders
#if defined(__cplusplus)
}
#endif

/* -------------------------------------------------------------------------- */
/*                             Crystal GUI Main                               */
/* -------------------------------------------------------------------------- */

// Type of the button
typedef enum CguiButtonType
{
    CGUI_BUTTON_TYPE_NORMAL,
    CGUI_BUTTON_TYPE_DISABLED,
    CGUI_BUTTON_TYPE_RECOMMENDED,
    CGUI_BUTTON_TYPE_ALERT,
    CGUI_BUTTON_TYPE_WARNING,
    CGUI_BUTTON_TYPE_DANGEROUS,
    CGUI_BUTTON_TYPE_MAX
} CguiButtonType;

// State of the button (internal)
typedef enum CguiButtonState
{
    CGUI_BUTTON_STATE_NORMAL,
    CGUI_BUTTON_STATE_FOCUSED,
    CGUI_BUTTON_STATE_PRESSED,
    CGUI_BUTTON_STATE_MAX
} CguiButtonState;

// Button structure (Members started from two underscores (__) is used internally)
typedef struct CguiButton
{
    Rectangle bounds;
    const char *text;
    int type;
    int __state;
    float __focusTimer;
} CguiButton;

#if defined(__cplusplus)
extern "C" {            // Prevents name mangling of functions
#endif
CguiButton *CguiCreateButton(CguiInstance *instance, Rectangle bounds, const char *text, int type); // Create a new button
void CguiDeleteButton(CguiInstance *instance, CguiButton *button);                                  // Delete the created button
bool CguiUpdateButton(CguiInstance *instance, CguiButton *button);                                  // Update the button, returns true when clicked
void CguiDrawButton(CguiInstance *instance, CguiButton *button);                                    // Draw the button using Crystal GUI Base
#if defined(__cplusplus)
}
#endif

#endif // CRYSTALGUI_H
