/// @file
///
/// @author    Anstro Pleuton
/// @copyright Copyright (c) 2025 Anstro Pleuton
///
/// Crystal GUI - A GUI framework for raylib.
///
/// This source file contains implementations for elements.
///
/// This project is licensed under the terms of MIT license.

#include <string.h>

#include "crystalgui/crystalgui.h"
#include "raylib.h"
#include "raymath.h"

extern int    cguiNameCounter;
extern Shader cguiBoxShader;

CguiNode *CguiCreateTextElement(const char *text, Color color)
{
    return CguiCreateTextElementPro(text, GetFontDefault(), 18, 1.0f, 1.5f, color, CGUI_TEXT_JUSTIFY_BEGIN, CGUI_TEXT_JUSTIFY_BEGIN);
}

CguiNode *CguiCreateTextElementPro(const char *text, Font font, float fontSize, float spacing, float lineSpacing, Color color, int xJustify, int yJustify)
{
    CguiNode *node = CguiCreateNodePro(CguiTFillParent(), TextFormat("CguiTextElement #%d", ++cguiNameCounter), CGUI_ELEMENT_NODE_TYPE_TEXT, NULL, sizeof(CguiTextElementData));
    if (!node)
    {
        return NULL;
    }

    CguiTextElementData data = { .text = text, .font = font, .fontSize = fontSize, .spacing = spacing, .lineSpacing = lineSpacing, .color = color, .xJustify = xJustify, .yJustify = yJustify };
    memcpy(node->data, &data, sizeof(CguiTextElementData));

    node->drawPre = CguiDrawPreTextElement;

    return node;
}

void CguiDrawPreTextElement(CguiNode *node)
{
    if (!node || node->type != CGUI_ELEMENT_NODE_TYPE_TEXT || !node->data)
    {
        return;
    }

    CguiTextElementData *data   = node->data;
    Rectangle            bounds = node->bounds;

    CguiDrawTextPro(data->text, data->font, bounds, data->fontSize, data->spacing, data->lineSpacing, data->color, data->xJustify, data->yJustify);
}

bool CguiIsTextElementDataEqual(CguiTextElementData a, CguiTextElementData b)
{
    return a.text == b.text &&
           a.font.texture.id == b.font.texture.id &&
           a.fontSize == b.fontSize &&
           a.spacing == b.spacing &&
           a.lineSpacing == b.lineSpacing &&
           CguiIsColorEqual(a.color, b.color) &&
           a.xJustify == b.xJustify &&
           a.yJustify == b.yJustify;
}

CguiNode *CguiCreateTextureElement(Texture texture)
{
    return CguiCreateTextureElementPro(texture, CguiGetTextureSizeRec(texture), Vector2Zero(), 0.0f, WHITE);
}

CguiNode *CguiCreateTextureElementPro(Texture texture, Rectangle source, Vector2 origin, float rotation, Color tint)
{
    CguiNode *node = CguiCreateNodePro(CguiTFillParent(), TextFormat("CguiTextureElement #%d", ++cguiNameCounter), CGUI_ELEMENT_NODE_TYPE_TEXTURE, NULL, sizeof(CguiTextureElementData));
    if (!node)
    {
        return NULL;
    }

    CguiTextureElementData data = { .texture = texture, .source = source, .origin = origin, .rotation = rotation, .tint = tint };
    memcpy(node->data, &data, sizeof(CguiTextureElementData));

    node->drawPre = CguiDrawPreTextureElement;

    return node;
}

void CguiDrawPreTextureElement(CguiNode *node)
{
    if (!node || node->type != CGUI_ELEMENT_NODE_TYPE_TEXTURE || !node->data)
    {
        return;
    }

    CguiTextureElementData *data = node->data;

    DrawTexturePro(data->texture, data->source, (Rectangle) { node->bounds.x + data->origin.x, node->bounds.y + data->origin.y, node->bounds.width, node->bounds.height }, data->origin, data->rotation, data->tint);
}

bool CguiIsTextureElementDataEqual(CguiTextureElementData a, CguiTextureElementData b)
{
    return a.texture.id == b.texture.id &&
           CguiIsRectangleEqual(a.source, b.source) &&
           Vector2Equals(a.origin, b.origin) &&
           a.rotation == b.rotation &&
           CguiIsColorEqual(a.tint, b.tint);
}

CguiNode *CguiCreateBoxElement(float radius, Color color)
{
    return CguiCreateBoxElementPro((Vector4) { radius, radius, radius, radius }, color, (Texture) { 0 }, 0.0f, Vector2Zero(), 0.0f, BLANK, (Texture) { 0 }, 0.0f, BLANK, (Texture) { 0 });
}

CguiNode *CguiCreateBoxElementEx(float radius, Color color, float shadowDistance, Color shadowColor, float borderThickness, Color borderColor)
{
    return CguiCreateBoxElementPro((Vector4) { radius, radius, radius, radius }, color, (Texture) { 0 }, shadowDistance, Vector2Zero(), 0.0f, shadowColor, (Texture) { 0 }, borderThickness, borderColor, (Texture) { 0 });
}

CguiNode *CguiCreateBoxElementPro(Vector4 radii, Color color, Texture texture, float shadowDistance, Vector2 shadowOffset, float shadowShrink, Color shadowColor, Texture shadowTexture, float borderThickness, Color borderColor, Texture borderTexture)
{
    CguiNode *node = CguiCreateNodePro(CguiTFillParent(), TextFormat("CguiBoxElement #%d", ++cguiNameCounter), CGUI_ELEMENT_NODE_TYPE_BOX, NULL, sizeof(CguiBoxElementData));
    if (!node)
    {
        return NULL;
    }

    CguiBoxElementData data = { .radii = radii, .color = color, .texture = texture, .shadowDistance = shadowDistance, .shadowOffset = shadowOffset, .shadowShrink = shadowShrink, .shadowColor = shadowColor, .shadowTexture = shadowTexture, .borderThickness = borderThickness, .borderColor = borderColor, .borderTexture = borderTexture };
    memcpy(node->data, &data, sizeof(CguiBoxElementData));

    node->drawPre = CguiDrawPreBoxElement;

    return node;
}

void CguiDrawPreBoxElement(CguiNode *node)
{
    if (!node || node->type != CGUI_ELEMENT_NODE_TYPE_BOX || !node->data)
    {
        return;
    }

    CguiBoxElementData *data = node->data;

    // Set additional textures
    // Raylib doesn't have SHADER_UNIFORM_BOOL ... ???

    SetShaderValue(cguiBoxShader, GetShaderLocation(cguiBoxShader, "useTexture"), (int[1]) { IsTextureValid(data->texture) }, SHADER_UNIFORM_INT);

    if (IsTextureValid(data->shadowTexture))
    {
        SetShaderValueTexture(cguiBoxShader, GetShaderLocation(cguiBoxShader, "texture1"), data->shadowTexture);
        SetShaderValue(cguiBoxShader, GetShaderLocation(cguiBoxShader, "useShadowTexture"), (int[1]) { 1 }, SHADER_UNIFORM_INT);
    }
    else
    {
        SetShaderValue(cguiBoxShader, GetShaderLocation(cguiBoxShader, "useShadowTexture"), (int[1]) { 0 }, SHADER_UNIFORM_INT);
    }
    if (IsTextureValid(data->borderTexture))
    {
        SetShaderValueTexture(cguiBoxShader, GetShaderLocation(cguiBoxShader, "texture2"), data->borderTexture);
        SetShaderValue(cguiBoxShader, GetShaderLocation(cguiBoxShader, "useBorderTexture"), (int[1]) { 1 }, SHADER_UNIFORM_INT);
    }
    else
    {
        SetShaderValue(cguiBoxShader, GetShaderLocation(cguiBoxShader, "useBorderTexture"), (int[1]) { 0 }, SHADER_UNIFORM_INT);
    }

    // Flip Y for shader rectangle
    Rectangle bounds = node->bounds;
    bounds           = CguiFlipRectangleY(bounds, GetScreenHeight() / 2.0f);

    SetShaderValue(cguiBoxShader, GetShaderLocation(cguiBoxShader, "rectangle"), &bounds, SHADER_UNIFORM_VEC4);

    // Set other node valeus
    SetShaderValue(cguiBoxShader, GetShaderLocation(cguiBoxShader, "radii"), &data->radii, SHADER_UNIFORM_VEC4);

    Vector4 colorN = ColorNormalize(data->color);
    SetShaderValue(cguiBoxShader, GetShaderLocation(cguiBoxShader, "color"), &colorN, SHADER_UNIFORM_VEC4);

    // Set shadow values
    SetShaderValue(cguiBoxShader, GetShaderLocation(cguiBoxShader, "shadowDistance"), &data->shadowDistance, SHADER_UNIFORM_FLOAT);

    Vector2 shadowOffset = { data->shadowOffset.x, -data->shadowOffset.y };
    SetShaderValue(cguiBoxShader, GetShaderLocation(cguiBoxShader, "shadowOffset"), &shadowOffset, SHADER_UNIFORM_VEC2);
    SetShaderValue(cguiBoxShader, GetShaderLocation(cguiBoxShader, "shadowShrink"), &data->shadowShrink, SHADER_UNIFORM_FLOAT);

    Vector4 shadowColorN = ColorNormalize(data->shadowColor);
    SetShaderValue(cguiBoxShader, GetShaderLocation(cguiBoxShader, "shadowColor"), &shadowColorN, SHADER_UNIFORM_VEC4);

    // Set border values
    SetShaderValue(cguiBoxShader, GetShaderLocation(cguiBoxShader, "borderThickness"), &data->borderThickness, SHADER_UNIFORM_FLOAT);

    Vector4 borderColorN = ColorNormalize(data->borderColor);
    SetShaderValue(cguiBoxShader, GetShaderLocation(cguiBoxShader, "borderColor"), &borderColorN, SHADER_UNIFORM_VEC4);

    // Draw rectangle
    BeginShaderMode(cguiBoxShader);
    if (IsTextureValid(data->texture))
        CguiDrawTextureFullscreen(data->texture, WHITE);
    else
        DrawRectangle(0, 0, CguiGetAppWidth(), CguiGetAppHeight(), WHITE);
    EndShaderMode();
}

bool CguiIsBoxElementDataEqual(CguiBoxElementData a, CguiBoxElementData b)
{
    return Vector4Equals(a.radii, b.radii) &&
           CguiIsColorEqual(a.color, b.color) &&
           a.texture.id == b.texture.id &&
           a.shadowDistance == b.shadowDistance &&
           Vector2Equals(a.shadowOffset, b.shadowOffset) &&
           a.shadowShrink == b.shadowShrink &&
           CguiIsColorEqual(a.shadowColor, b.shadowColor) &&
           a.shadowTexture.id == b.shadowTexture.id &&
           a.borderThickness == b.borderThickness &&
           CguiIsColorEqual(a.borderColor, b.borderColor) &&
           a.borderTexture.id == b.borderTexture.id;
}
