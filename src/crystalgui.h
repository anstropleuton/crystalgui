/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Crystal GUI - Modern-looking Crystal-clear GUI library for raylib.
 *
 * Copyright (c) 2023 Anstro Pleuton
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a
 *   copy of this software and associated documentation files (the "Software"),
 *   to deal in the Software without restriction, including without limitation
 *   the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *   and/or sell copies of the Software, and to permit persons to whom the
 *   Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *   DEALINGS IN THE SOFTWARE.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * The fundamentals
 *
 *   Component: Everything interactable in Crystal GUI is a component, such as
 *   the button that you press, check box that you tick, etc. There exists two
 *   uninteractable component called "Label" and "Icon".
 *
 *   Element: An Element is made of zero or components. You can think it as a
 *   group of components. Every element draws a shader-processed rectangle. If
 *   an element consists of one interactable component, the entire element (the
 *   boundries of the shader-processed rectangle) is interactable.
 *
 *   All together in a diagram for element:
 *
 *      /-------------------\
 *      |  (X)  Use GUI     |   --- The rectangle is an element.
 *      \---|------|--------/
 *          |      |
 *          |      \--------------- Text Readonly Component.
 *          \---------------------- Toggle Checkbox Component.
 *
 *   This is not chemistry, folks!
 *
 * Usage
 *
 *   // WIP, TODO: Add usage guides
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * How to make your own component?
 *
 *   Create a structure and use `CguiComponentBasic component;` as your first
 *   member of the structure. This is because a pointer to a struct actually
 *   points to the first member of the struct and so your custom component is
 *   internally casted to `CguiComponentBasic` (without changing any data) and
 *   called the update/draw functions in `CguiComponentBasic`
 *
 *   (recommended) Create a creation function. (example: `CreateMyComponent`)
 *   (recommended) Create a destruction function. (example: `DeleteMyComponent`)
 *   (recommended) Create an update function. (example: `UpdateMyComponent`)
 *   (recommended) Create a drawing function. (example: `DrawMyComponent`)
 *
 *   In the creation function, assign `CguiComponentBasic component;` with
 *   the function call for `CguiCreateComponentBasic()`. It will allocate
 *   memory for `CguiComponentBasic component;`
 *
 *   In the destruction function, use `CguiDeleteComponentBasic()` to free the
 *   memory allocated when creation.
 *
 *   In the update function, call `CguiUpdateComponentBasic()` before updating
 *   your component.
 *
 *   In the draw function, call `CguiDrawComponentBasic()` before drawing
 *   your component.
 *
 *   The update function pointer in `CguiComponentBasic` is called when the
 *   update funciton in the element is called.
 *   The draw function pointer in `CguiComponentBasic` is called when the
 *   draw function in the element is called.
 *
 *   You can have NULL for update and/or draw function pointer in
 *   `CguiComponentBasic` to skip it. They can do almost anything with the
 *   exception of "Drawing-related functions" from raylib.
 *
 *   Example: ```c
 *     typedef struct MyComponent { CguiComponentBasic *component; };
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * How component and element come together in love?
 *
 *   In the update element function,
 */

#ifndef CRYSTALGUI_H
#define CRYSTALGUI_H

#include "raylib.h"
#include "raymath.h"
#include "rllist.h"

#if defined(__cplusplus)
extern "C" {            // Prevents name mangling of functions.
#endif



/*******************************************************************************
 * Misc functions useful for (and used by) Crystal GUI.
 */

void DrawTextureFullscreen(Texture texture, bool isRenderTexture);              // Draw texture to fit the render size.
Color ApplyHSVOnColor(Color color, Vector3 hsv);                                // Apply (add as shifts) HSV Values on the color.



/*******************************************************************************
 * Crystal GUI Macro Definitions.
 */



/*******************************************************************************
 * Crystal GUI Enum Definitions.
 */

// Blur shader location index.
typedef enum CguiBlurShaderLocationIndex
{
    CRYSTALGUI_BLUR_SHADER_LOC_RESOLUTION,
    CRYSTALGUI_BLUR_SHADER_LOC_BLUR_RADIUS,
    CRYSTALGUI_BLUR_SHADER_LOC_MAX
} CguiBlurShaderLocationIndex;

// Shadow shader location index.
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

// Rectangle shader location index.
typedef enum CguiRectangleShaderLocationIndex
{
    CRYSTALGUI_RECTANGLE_SHADER_LOC_RESOLUTION,
    CRYSTALGUI_RECTANGLE_SHADER_LOC_RECTANGLE,
    CRYSTALGUI_RECTANGLE_SHADER_LOC_ROUNDNESS,
    CRYSTALGUI_RECTANGLE_SHADER_LOC_RECTANGLE_TINT,
    CRYSTALGUI_RECTANGLE_SHADER_LOC_MAX
} CguiRectangleShaderLocationIndex;



/*******************************************************************************
 * Crystal GUI Struct Definitions.
 */

// Font properties for rendering text.
typedef struct CguiTextProperties
{
    Font font;                      // Font to be used in text.
    float size;                     // Size of the font.
    float spacing;                  // Spacing for each letters in text.
    Color color;                    // Color of the text.

    Color shadowColor;              // Text shadow color (usually opposite color).
    float shadowBlurRadius;         // Blur radius (shadow diffusion) of text shadow, uses Blur Shader.
    Vector2 shadowOffset;           // Position offset of text shadow.
} CguiTextProperties;

// All the default shader variables used by Crystal GUI.
typedef struct CguiShaderVariables
{
    // Shader properties contains shader variables that do not depend on the shader use.
    // Example: "rectangle" is highly dependent on where and how the shader is used,
    // which is excluded from properties struct.
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
        float rectangle[4];         // Excluded from properties struct.
        float roundness;
        float shadowRadius;
        float shadowSize;
        float shadowOffset[2];
        float shadowColor[4];       // Excluded from properties struct.
    } shadowShaderVariables;

    struct RectangleShaderVariables
    {
        float resolution[2];
        float rectangle[4];         // Excluded from properties struct.
        float roundness;
        float rectangleTint[4];     // Excluded from properties struct.
    } rectangleShaderVariables;
} CguiShaderVariables;

// Main theme colors.
typedef struct CguiThemeColors
{
    Color accent;
    Color regular;
    Color disabled;
    Color alert;
    Color warning;
    Color destructive;

    Vector3 focused;
    Vector3 pressed;

    Color shadowColor;
} CguiThemeColors;

// Pre-processed resources used as cached. (INCOMPLETE and UNUSED yet)
typedef struct CguiPreprocessedResources
{
    RenderTexture shadowTexture;
    RenderTexture rectangleTexture;
} CguiPreprocessedResources;

// Main Crystal GUI Instance, used in most of the functions.
typedef struct CguiInstance
{
    Shader blurShader;              // Blur shader.
    Shader shadowShader;            // Shadow shader.
    Shader rectangleShader;         // Shader to draw rounded rectangle.

    CguiShaderVariables shaderVariables;    // All the default shader variables used by Crystal GUI.

    int blurShaderLocs[CRYSTALGUI_BLUR_SHADER_LOC_MAX];             // Blur shader custom uniform locations.
    int shadowShaderLocs[CRYSTALGUI_SHADOW_SHADER_LOC_MAX];         // Shadow shader custom uniform locations.
    int rectangleShaderLocs[CRYSTALGUI_RECTANGLE_SHADER_LOC_MAX];   // Rectangle shader custom uniform locations.

    RenderTexture blurInputRenderTexture;   // Input frame buffer to be blurred.
    RenderTexture blurOutputRenderTexture;  // Blurred frame buffer.
    RenderTexture textRenderTexture;        // Render texture used to temporarily store text to be blurred later.

    CguiTextProperties textProperties;  // Font properties.
    CguiThemeColors themeColors;        // All the colors.

    float focusTimerSpeed;          // Focus Fade speed.
    float animationTimerSpeed;      // Animation speed.
    float colorTransparency;        // Transparency of color ranging from 0.0f to 1.0f .
} CguiInstance;



/*******************************************************************************
 * Crystal GUI Function Definitions.
 */

void CguiLoadDefaultShaders(CguiInstance *instance);                            // Load default shaders.
void CguiUnloadDefaultShaders(CguiInstance *instance);                          // Unload default shaders.
void CguiSetDefaultShaderProperties(CguiInstance *instance);                    // Set default shader property values.
void CguiSetDefaultShaderVariables(CguiInstance *instance);                     // Set property values to shader variables.
void CguiSetDefaultShaderLocations(CguiInstance *instance);                     // Get default shader locations.
void CguiApplyShaderVariables(CguiInstance *instance);                          // Apply shader variables to shader.
void CguiLoadRenderTextures(CguiInstance *instance);                            // Load render textures.
void CguiUnloadRenderTextures(CguiInstance *instance);                          // Unload render texture.
void CguiSetDefaultFontProperties(CguiInstance *instance);                      // Set default font property values.
void CguiSetDefaultThemeColors(CguiInstance *instance);                         // Set the default theme colors.
CguiInstance *CguiCreateInstance(void);                                         // Create a new instance of Crystal GUI.
void CguiDeleteInstance(CguiInstance *instance);                                // Delete the created Crystal GUI instance.
void CguiUpdateResolution(CguiInstance *instance);                              // Update shader and render texture resolution. (reloads render texture)
void CguiBlurRenderTexture(CguiInstance *instance);                             // Apply blur effect to the input frame.



/*******************************************************************************
 * Crystal GUI Basic Component.
 */

// Placement of the component in an element.
// An element cah have propertis to control the usage of placement.
// It can choose to use onlh left center and right placement and clamp everything.
// Placement also acts as an anchor when an element gets resized.
typedef enum CguiPlacement
{
    CRYSTALGUI_PLACEMENT_NONE,
    CRYSTALGUI_PLACEMENT_CENTER,
    CRYSTALGUI_PLACEMENT_TOP,
    CRYSTALGUI_PLACEMENT_BOTTOM,
    CRYSTALGUI_PLACEMENT_LEFT,
    CRYSTALGUI_PLACEMENT_RIGHT,
    CRYSTALGUI_PLACEMENT_TOP_LEFT,
    CRYSTALGUI_PLACEMENT_TOP_RIGHT,
    CRYSTALGUI_PLACEMENT_BOTTOM_LEFT,
    CRYSTALGUI_PLACEMENT_BOTTOM_RIGHT,
    CRYSTALGUI_PLACEMENT_MAX
} CguiAnchor;



/*******************************************************************************
 * WARNING!
 * Do not directly initialize the components and always use the creation functions.
 * If you know what you are doing, go ahead!
 */

// Basic component
typedef struct CguiComponentBasic
{
    void (*update)(void *component);    // Function to update the component.
    void (*draw)(void *component);  // Function to draw the component.
    int placement;                  // Placement of the component in element.
    Rectangle minimumBounds;        // Minimum bounds required by the component.
    void *elementPointer;           // Nullable pointer to the element (automatially set)
} CguiComponentBasic;

CguiComponentBasic *CguiCreateComponentBasic(CguiInstance *instance, int placement, Rectangle minimumBounds);                           // Create basic component with no features.
void CguiDeleteComponentBasic(CguiInstance *instance, CguiComponentBasic *component);                                                   // Delete the created basic component.
void CguiUpdateComponentBasic(CguiInstance *instance, CguiComponentBasic *component);                                                   // Update the created basic component.
void CguiDrawComponentBasic(CguiInstance *instance, CguiComponentBasic *component);                                                     // Draw the created basic component.



/*******************************************************************************
 * Crystal GUI Non-Basic Components.
 */

// Display the text (with markdown)... that's it.
typedef struct CguiComponentReadonlyText
{
    CguiComponentBasic *component;  // All component must have these elements.
    const char *text;               // Supports few markdown formats. :O
    bool isRawText;                 // Prevents markdown formatting.
} CguiComponentReadonlyText;

CguiComponentReadonlyText *CguiCreateComponentReadonlyText(CguiInstance *instance, int placement, Rectangle minimumBounds, const char *text, bool isRawText);   // Create basic component with no features.
void CguiDeleteComponentReadonlyText(CguiInstance *instance, CguiComponentReadonlyText *component);                                     // Delete the created basic component.
void CguiUpdateComponentReadonlyText(CguiInstance *instance, CguiComponentReadonlyText *component);                                     // Update the created basic component.
void CguiDrawComponentReadonlyText(CguiInstance *instance, CguiComponentReadonlyText *component);                                       // Draw the created basic component.

// Display a texture (icon)... that's it.
typedef struct CguiComponentIconTexture
{
    CguiComponentBasic *component;  // All component must have these elements.
    Texture texture;                // Texture to draw.
    Color tint;                     // Tint color for texture.
} CguiComponentIconTexture;

CguiComponentIconTexture *CguiCreateComponentIconTexture(CguiInstance *instance, int placement, Rectangle minimumBounds);               // Create basic component with no features.
void CguiDeleteComponentIconTexture(CguiInstance *instance, CguiComponentIconTexture *component);                                       // Delete the created basic component.
void CguiUpdateComponentIconTexture(CguiInstance *instance, CguiComponentIconTexture *component);                                       // Update the created basic component.
void CguiDrawComponentIconTexture(CguiInstance *instance, CguiComponentIconTexture *component);                                         // Draw the created basic component.



/*******************************************************************************
 * Crystal GUI Element.
 */

typedef struct CguiElement
{
    List *components;               // List of component pointers.
    Rectangle bounds;               // Boundery of the element.
} CguiElement;

CguiElement *CguiCreateElement(CguiInstance *instance);                         //
void CguiDeleteElement(CguiInstance *instance, CguiElement *element);           //
void CguiUpdateElement(CguiInstance *instance, CguiElement *element);           //
void CguiDrawElement(CguiInstance *instance, CguiElement *element);             //

#if defined(__cplusplus)
}
#endif

#endif // CRYSTALGUI_H
