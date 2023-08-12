#include "crystalgui.h"
#include <stdlib.h>

#ifndef CRYSTALGUI_RESOURCE_PATH
#define CRYSTALGUI_RESOURCE_PATH "res/"
#endif // CRYSTALGUI_RESOURCE_PATH

#ifndef CRYSTALGUI_SHADER_PATH
#define CRYSTALGUI_SHADER_PATH CRYSTALGUI_RESOURCE_PATH "shaders/"
#endif // CRYSTALGUI_SHADER_PATH



/*******************************************************************************
 * Misc functions useful for (and used by) Crystal GUI.
 */

// Draw texture to fit the render size.
void DrawTextureFullscreen(Texture texture, bool isRenderTexture)
{
    DrawTexturePro(texture, (Rectangle) { 0.0f, 0.0f, texture.width, texture.height * (isRenderTexture ? -1.0f : 1.0f) }, (Rectangle) { 0.0f, 0.0f, GetScreenWidth(), GetScreenHeight() }, (Vector2) { 0.0f, 0.0f }, 0.0f, WHITE);
}

// Apply (add as shifts) HSV Values on the color.
Color ApplyHSVOnColor(Color color, Vector3 hsv)
{
    Vector3 colorHSV = ColorToHSV(color);

    colorHSV.x += hsv.x;
    colorHSV.y += hsv.y;
    colorHSV.z += hsv.z;

    if (colorHSV.y > 1.0f) { colorHSV.z -= colorHSV.y - 1.0f; colorHSV.y = 1.0f; }
    if (colorHSV.z > 1.0f) { colorHSV.y -= colorHSV.z - 1.0f; colorHSV.z = 1.0f; }
    if (colorHSV.y < 0.0f) { colorHSV.z += colorHSV.y + 1.0f; colorHSV.y = 0.0f; }
    if (colorHSV.z < 0.0f) { colorHSV.y += colorHSV.z + 1.0f; colorHSV.z = 0.0f; }

    Color finalColor = ColorFromHSV(colorHSV.x, colorHSV.y, colorHSV.z);
    finalColor.a = color.a;
    return finalColor;
}

// Clamp the floating point value between minimum and maximum.
float FloatClamp(float value, float minimum, float maximum)
{
    if (value < minimum) value = minimum;
    if (value > maximum) value = maximum;
    return value;
}



/*******************************************************************************
 * Crystal GUI Function Definitions.
 */

// Load default shaders.
void CguiLoadDefaultShaders(CguiInstance *instance)
{
    if (!instance) return;

    instance->blurShader = LoadShader(CRYSTALGUI_SHADER_PATH "blurShader.vs", CRYSTALGUI_SHADER_PATH "blurShader.fs");
    instance->shadowShader = LoadShader(CRYSTALGUI_SHADER_PATH "shadowShader.vs", CRYSTALGUI_SHADER_PATH "shadowShader.fs");
    instance->rectangleShader = LoadShader(CRYSTALGUI_SHADER_PATH "rectangleShader.vs", CRYSTALGUI_SHADER_PATH "rectangleShader.fs");
}

// Unload default shaders.
void CguiUnloadDefaultShaders(CguiInstance *instance)
{
    if (!instance) return;

    UnloadShader(instance->blurShader);
    UnloadShader(instance->shadowShader);
    UnloadShader(instance->rectangleShader);
}

// Set default shader property values.
void CguiSetDefaultShaderProperties(CguiInstance *instance)
{
    if (!instance) return;

    instance->shaderVariables.shaderProperties.resolution = (Vector2) { GetScreenWidth(), GetScreenHeight() };
    instance->shaderVariables.shaderProperties.blurRadius = 10.0f;
    instance->shaderVariables.shaderProperties.roundness = 5.0f;
    instance->shaderVariables.shaderProperties.shadowRadius = 30.0f;
    instance->shaderVariables.shaderProperties.shadowSize = -10.0f;
    instance->shaderVariables.shaderProperties.shadowOffset = (Vector2) { 0.0f, -10.0f };
}

// Set property values to shader variables.
void CguiSetDefaultShaderVariables(CguiInstance *instance)
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

// Set default shader locations.
void CguiSetDefaultShaderLocations(CguiInstance *instance)
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

// Apply shader variables to shader.
void CguiApplyShaderVariables(CguiInstance *instance)
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

// Load render textures.
void CguiLoadRenderTextures(CguiInstance *instance)
{
    if (!instance) return;

    instance->blurInputRenderTexture = LoadRenderTexture(instance->shaderVariables.shaderProperties.resolution.x, instance->shaderVariables.shaderProperties.resolution.y);
    instance->blurOutputRenderTexture = LoadRenderTexture(instance->shaderVariables.shaderProperties.resolution.x, instance->shaderVariables.shaderProperties.resolution.y);
    instance->tempTextRenderTexture = LoadRenderTexture(instance->shaderVariables.shaderProperties.resolution.x, instance->shaderVariables.shaderProperties.resolution.y);

    // Clear all the render textures.
    BeginTextureMode(instance->blurInputRenderTexture);
    ClearBackground(BLANK);
    EndTextureMode();
    BeginTextureMode(instance->blurOutputRenderTexture);
    ClearBackground(BLANK);
    EndTextureMode();
    BeginTextureMode(instance->tempTextRenderTexture);
    ClearBackground(BLANK);
    EndTextureMode();
}

// Unload render texture.
void CguiUnloadRenderTextures(CguiInstance *instance)
{
    if (!instance) return;

    UnloadRenderTexture(instance->blurInputRenderTexture);
    UnloadRenderTexture(instance->blurOutputRenderTexture);
    UnloadRenderTexture(instance->tempTextRenderTexture);
}

// Set default font property values.
void CguiSetDefaultFontProperties(CguiInstance *instance)
{
    if (!instance) return;

    instance->textProperties.size = 20.0f;
    instance->textProperties.spacing = 1.0f;
    instance->textProperties.color = WHITE;
    instance->textProperties.font = LoadFontEx(CRYSTALGUI_RESOURCE_PATH "Roboto-Regular.ttf", instance->textProperties.size, NULL, 0);

    instance->textProperties.shadowBlurRadius = 2.0f;
    instance->textProperties.shadowOffset = (Vector2) { 0.0f, -2.0f };
}

// Set the default theme colors.
void CguiSetDefaultThemeColors(CguiInstance *instance)
{
    if (!instance) return;

    instance->themeColors.accent = (Color) { 0, 170, 255, 255 };
    instance->themeColors.regular = (Color) { 0, 0, 0, 255 };
    instance->themeColors.disabled = (Color) { 127, 127, 127, 255 };
    instance->themeColors.warning = (Color) { 255, 255, 0, 255 };
    instance->themeColors.alert = (Color) { 255, 127, 0, 255 };
    instance->themeColors.destructive = (Color) { 255, 0, 0, 255 };

    instance->themeColors.focused = (Vector3) { 0, 0, 0.062666 };
    instance->themeColors.pressed = (Vector3) { 0, 0, 0.125222 };

    instance->themeColors.shadowColor = (Color) { 0, 0, 0, 127 };
    instance->textProperties.color = (Color) { 255, 255, 255, 255 };
}

// Create a new instance of Crystal GUI.
CguiInstance *CguiCreateInstance(void)
{
    CguiInstance *instance = (CguiInstance *)malloc(sizeof(CguiInstance));
    if (!instance) return NULL;

    instance->shaderVariables.shaderProperties.resolution = (Vector2) { GetScreenWidth(), GetScreenHeight() };

    CguiLoadRenderTextures(instance);

    // Load default shader.
    CguiLoadDefaultShaders(instance);
    CguiSetDefaultShaderProperties(instance);
    CguiSetDefaultShaderVariables(instance);
    CguiSetDefaultShaderLocations(instance);
    CguiApplyShaderVariables(instance);

    CguiSetDefaultFontProperties(instance);
    CguiSetDefaultThemeColors(instance);

    instance->focusTimerSpeed = 10.0f;
    instance->animationTimerSpeed = 10.0f;
    instance->colorTransparency = 0.5f;

    return instance;
}

// Delete the created Crystal GUI instance.
void CguiDeleteInstance(CguiInstance *instance)
{
    if (!instance) return;

    CguiUnloadRenderTextures(instance);
    CguiUnloadDefaultShaders(instance);
    UnloadFont(instance->textProperties.font);
    free(instance);
}

// Update shader and render texture resolution. (reloads render texture)
void CguiUpdateResolution(CguiInstance *instance)
{
    if (!instance) return;

    instance->shaderVariables.shaderProperties.resolution = (Vector2) { GetScreenWidth(), GetScreenHeight() };

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

// Apply blur effect to the input blur render texture.
void CguiBlurRenderTexture(CguiInstance *instance)
{
    if (!instance) return;

    BeginTextureMode(instance->blurOutputRenderTexture);
    BeginShaderMode(instance->blurShader);
    DrawTextureFullscreen(instance->blurInputRenderTexture.texture, true);
    EndShaderMode();
    EndTextureMode();
}

// Draw a rectangle using default shaders.
void CguiDrawShaderRectangle(CguiInstance *instance, Rectangle rectangle, Color rectangleColor, Color shadowColor)
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

// Draw text using default shaders.
void CguiDrawShaderText(CguiInstance *instance, const char *text, Rectangle rectangle)
{
    if (!instance) return;

    Vector2 measuredText = MeasureTextEx(instance->textProperties.font, text, instance->textProperties.size, instance->textProperties.spacing);
    Vector2 position = { rectangle.x + (rectangle.width - measuredText.x) / 2.0f, rectangle.y + (rectangle.height - measuredText.y) / 2.0f };

    position.x -= instance->textProperties.shadowOffset.x;
    position.y -= instance->textProperties.shadowOffset.y;
    BeginTextureMode(instance->tempTextRenderTexture);
    ClearBackground(BLANK);
    DrawTextEx(instance->textProperties.font, text, position, instance->textProperties.size, instance->textProperties.spacing, instance->textProperties.shadowColor);
    EndTextureMode();
    position.x += instance->textProperties.shadowOffset.x;
    position.y += instance->textProperties.shadowOffset.y;

    // Temporarily set font properties as blur shader variables.
    SetShaderValue(instance->blurShader, instance->blurShaderLocs[CRYSTALGUI_BLUR_SHADER_LOC_BLUR_RADIUS], &instance->textProperties.shadowBlurRadius, SHADER_UNIFORM_FLOAT);

    BeginShaderMode(instance->blurShader);
    DrawTextureFullscreen(instance->tempTextRenderTexture.texture, true);
    EndShaderMode();

    // Reset blur shader variables.
    SetShaderValue(instance->blurShader, instance->blurShaderLocs[CRYSTALGUI_BLUR_SHADER_LOC_BLUR_RADIUS], &instance->shaderVariables.blurShaderVariables.blurRadius, SHADER_UNIFORM_FLOAT);

    DrawTextEx(instance->textProperties.font, text, position, instance->textProperties.size, instance->textProperties.spacing, instance->textProperties.color);
}

// Draw a rectangle with ticking animation.
void CguiDrawShaderCheckbox(CguiInstance *instance, Rectangle rectangle, Color tickedColor, Color untickedColor, Color shadowColor, float tickThickness, float animationTime)
{
    if (!instance) return;

    float firstHalfAnimationTime = FloatClamp(animationTime * 2.0f, 0.0f, 1.0f);
    float thirdQuaterAnimationTime = FloatClamp(animationTime * 4.0f - 2.0f, 0.0f, 1.0f);
    float fourthQuaterAnimationTime = FloatClamp(animationTime * 4.0f - 3.0f, 0.0f, 1.0f);

    Color rectangleColor = { tickedColor.r * firstHalfAnimationTime + untickedColor.r * (1.0f - firstHalfAnimationTime),
                             tickedColor.g * firstHalfAnimationTime + untickedColor.g * (1.0f - firstHalfAnimationTime),
                             tickedColor.b * firstHalfAnimationTime + untickedColor.b * (1.0f - firstHalfAnimationTime),
                             tickedColor.a * firstHalfAnimationTime + untickedColor.a * (1.0f - firstHalfAnimationTime)
                           };
    Color tickColor = instance->textProperties.color;

    CguiDrawShaderRectangle(instance, rectangle, rectangleColor, shadowColor);

    float lineThickness = instance->shaderVariables.shaderProperties.roundness * tickThickness;
    if (lineThickness < 1.0f) lineThickness = 1.0f;

    Vector2 leftCheckLineStart = { rectangle.x + rectangle.width * 0.296875f, rectangle.y + rectangle.height * 0.509375f };
    Vector2 leftCheckLineEndOffset = { rectangle.width * 0.1625f * thirdQuaterAnimationTime, rectangle.height * 0.15f * thirdQuaterAnimationTime };
    Vector2 leftCheckLineEnd = { leftCheckLineStart.x + leftCheckLineEndOffset.x, leftCheckLineStart.y + leftCheckLineEndOffset.y };

    Vector2 rightCheckLineStart = leftCheckLineEnd;
    Vector2 rightCheckLineEndOffset = { rectangle.width * 0.265625f * fourthQuaterAnimationTime, rectangle.height * -0.315625f * fourthQuaterAnimationTime };
    Vector2 rightCheckLineEnd = { rightCheckLineStart.x + rightCheckLineEndOffset.x, rightCheckLineStart.y + rightCheckLineEndOffset.y };

    DrawLineEx(leftCheckLineStart, leftCheckLineEnd, lineThickness, tickColor);
    DrawLineEx(rightCheckLineStart, rightCheckLineEnd, lineThickness, tickColor);
}



/*******************************************************************************
 * Crystal GUI Basic Component.
 */

// Create basic component with no features.
CguiComponentBasic *CguiCreateComponentBasic(CguiInstance *instance, int placement, Rectangle minimumBounds)
{
    if (!instance) return NULL;
    CguiComponentBasic *component = (CguiComponentBasic *)malloc(sizeof(CguiComponentBasic));
    if (!component) return NULL;

    component->placement = placement;
    component->minimumBounds = minimumBounds;
    component->update = CguiUpdateComponentBasic;
    component->draw = CguiDrawComponentBasic;

    component->elementPointer = NULL;

    return component;
}

// Delete the created basic component.
void CguiDeleteComponentBasic(CguiInstance *instance, CguiComponentBasic *component)
{
    if (!instance || !component) return;
    free(component);
}

// Update the created basic component.
void CguiUpdateComponentBasic(CguiInstance *instance, CguiComponentBasic *component)
{
    if (!instance || !component) return;

    // Casting back to non-basic component (when casted to basic component) works fine.
    if (component->update) component->update(component);
}

// Draw the created basic component.
void CguiDrawComponentBasic(CguiInstance *instance, CguiComponentBasic *component)
{
    if (!instance || !component) return;

    // Casting back to non-basic component (when casted to basic component) works fine.
    if (component->draw) component->draw(component);
}



/*******************************************************************************
 * Crystal GUI Non-Basic Components.
 */

// Create basic component with no features.
CguiComponentReadonlyText *CguiCreateComponentReadonlyText(CguiInstance *instance, int placement, Rectangle minimumBounds, const char *text, bool isRawText)
{
    if (!instance) return NULL;
    CguiComponentReadonlyText *component = (CguiComponentReadonlyText *)malloc(sizeof(CguiComponentReadonlyText));
    if (!component) return NULL;

    component->component = CguiCreateComponentBasic(instance, placement, minimumBounds);
    component->component->update = CguiUpdateComponentReadonlyText;
    component->component->draw = CguiDrawComponentReadonlyText;
    component->text = text;
    component->isRawText = isRawText;
}

// Delete the created basic component.
void CguiDeleteComponentReadonlyText(CguiInstance *instance, CguiComponentReadonlyText *component)
{
    if (!instance || !component) return;
    CguiDeleteComponentBasic(instance, component->component);
    free(component);
}

// Update the created basic component.
void CguiUpdateComponentReadonlyText(CguiInstance *instance, CguiComponentReadonlyText *component)
{
    if (!instance || !component) return;
    CguiUpdateComponentBasic(instance, component->component);
}

// Draw the created basic component.
void CguiDrawComponentReadonlyText(CguiInstance *instance, CguiComponentReadonlyText *component)
{
    if (!instance || !component) return;
    CguiDrawComponentBasic(instance, component->component);
}

// Create basic component with no features.
CguiComponentIconTexture *CguiCreateComponentIconTexture(CguiInstance *instance, int placement, Rectangle minimumBounds, Texture texture, Color tint)
{
    if (!instance) return NULL;
    CguiComponentIconTexture *component = (CguiComponentIconTexture *)malloc(sizeof(CguiComponentIconTexture));
    if (!component) return NULL;

    component->component = CguiCreateComponentBasic(instance, placement, minimumBounds);
    component->texture = texture;
    component->tint = tint;
}

// Delete the created basic component.
void CguiDeleteComponentIconTexture(CguiInstance *instance, CguiComponentIconTexture *component)
{
    if (!instance || !component) return;
    CguiDeleteComponentBasic(instance, component->component);
    free(component);
}

// Update the created basic component.
void CguiUpdateComponentIconTexture(CguiInstance *instance, CguiComponentIconTexture *component)
{
    if (!instance || !component) return;
    CguiUpdateComponentBasic(instance, component->component);
}

// Draw the created basic component.
void CguiDrawComponentIconTexture(CguiInstance *instance, CguiComponentIconTexture *component)
{
    if (!instance || !component) return;
    CguiDrawComponentBasic(instance, component->component);
}

// Create element.
CguiElement *CguiCreateElement(CguiInstance *instance)
{
    if (!instance) return NULL;
    CguiElement *element = (CguiElement *)malloc(sizeof(CguiElement));
    if (!element) return NULL;
}

// Delete the created element.
void CguiDeleteElement(CguiInstance *instance, CguiElement *element)
{
    if (!instance || !element) return;
}

// Update the created element.
void CguiUpdateElement(CguiInstance *instance, CguiElement *element)
{
    if (!instance || !element) return;
}

// Draw the created element.
void CguiDrawElement(CguiInstance *instance, CguiElement *element)
{
    if (!instance || !element) return;
}
