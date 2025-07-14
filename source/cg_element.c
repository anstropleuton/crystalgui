/// @file
///
/// @author    Anstro Pleuton
/// @copyright Copyright (c) 2025 Anstro Pleuton
///
/// Crystal GUI - A modern GUI framework for raylib.
///
/// This source file contains implementations for basic elements of
/// Crystal GUI.
///
/// This project is licensed under the terms of MIT license.

#include <raylib.h>
#include <raymath.h>
#include <string.h>

#include "crystalgui.h"

extern int    cguiNameCounter;
extern Shader cguiBoxShader;

CguiNode *CguiCreateTextElement(const char *text, Font font, float size, float spacing, float lineSpacing, Color color, bool strictWordWrap)
{
    CguiNode *node = CguiCreateNodePro(TextFormat("CguiTextElement #%d", ++cguiNameCounter), CGUI_ELEMENT_NODE_TYPE_TEXT, NULL, sizeof(CguiTextElementData));
    if (!node)
    {
        return NULL;
    }

    CguiTextElementData data = { .text = text, .font = font, .size = size, .spacing = spacing, .lineSpacing = lineSpacing, .color = color, .strictWordWrap = strictWordWrap };
    memcpy(node->data, &data, sizeof(CguiTextElementData));

    node->drawPre = CguiDrawPreTextElement;

    return node;
}

CguiNode *CguiCreateTextureElement(Texture texture, Rectangle source, Vector2 origin, float rotation, Color tint)
{
    CguiNode *node = CguiCreateNodePro(TextFormat("CguiTextureElement #%d", ++cguiNameCounter), CGUI_ELEMENT_NODE_TYPE_TEXTURE, NULL, sizeof(CguiTextureElementData));
    if (!node)
    {
        return NULL;
    }

    CguiTextureElementData data = { .texture = texture, .source = source, .origin = origin, .rotation = rotation, .tint = tint };
    memcpy(node->data, &data, sizeof(CguiTextureElementData));

    node->drawPre = CguiDrawPreTextureElement;

    return node;
}

CguiNode *CguiCreateBoxElement(Vector4 radii, Color color, Texture texture, float shadowDist, Vector2 shadowOffset, float shadowShrink, Color shadowColor, Texture shadowTexture, float borderThickness, Color borderColor, Texture borderTexture)
{
    CguiNode *node = CguiCreateNodePro(TextFormat("CguiBoxElement #%d", ++cguiNameCounter), CGUI_ELEMENT_NODE_TYPE_BOX, NULL, sizeof(CguiBoxElementData));
    if (!node)
    {
        return NULL;
    }

    CguiBoxElementData data = { .radii = radii, .color = color, .texture = texture, .shadowDist = shadowDist, .shadowOffset = shadowOffset, .shadowShrink = shadowShrink, .shadowColor = shadowColor, .shadowTexture = shadowTexture, .borderThickness = borderThickness, .borderColor = borderColor, .borderTexture = borderTexture };
    memcpy(node->data, &data, sizeof(CguiBoxElementData));

    node->drawPre = CguiDrawPreBoxElement;

    return node;
}

void CguiDrawPreTextElement(CguiNode *node)
{
    if (!node || node->type != CGUI_ELEMENT_NODE_TYPE_TEXT)
    {
        return;
    }

    CguiTextElementData data   = *(CguiTextElementData *) node->data;
    Rectangle           bounds = node->bounds;

    // This implementation is from raylib, though, modified to include basic word-wrapping.

    // Use default font if none provided (raylib behavior)
    if (data.font.texture.id == 0) data.font = GetFontDefault();

    float scaleFactor = data.size / data.font.baseSize;
    float wrapWidth   = bounds.width;

    float offsetX = 0.0f;
    float offsetY = 0.0f;

    const char *text    = data.text;
    int         textLen = TextLength(text);
    int         i       = 0;

    while (i < textLen)
    {
        // Track start of the word
        int   wordStart = i;
        float wordWidth = 0.0f;
        int   wordLen   = 0;

        // First, calculate word width
        while (i < textLen)
        {
            int codepointByteCount = 0;
            int codepoint          = GetCodepointNext(&text[i], &codepointByteCount);

            if (codepoint == ' ' || codepoint == '\t' || codepoint == '\n') break;

            int   index   = GetGlyphIndex(data.font, codepoint);
            float advance = (data.font.glyphs[index].advanceX > 0)
                                ? data.font.glyphs[index].advanceX
                                : data.font.recs[index].width;

            wordWidth += advance * scaleFactor + data.spacing;
            i += codepointByteCount;
            wordLen += codepointByteCount;
        }

        // If word is too wide and strict wrapping is enabled
        if (data.strictWordWrap && wordWidth > wrapWidth)
        {
            int   j            = 0;

            while (j < wordLen)
            {
                int codepointByteCount = 0;
                int codepoint          = GetCodepointNext(&text[wordStart + j], &codepointByteCount);
                int index              = GetGlyphIndex(data.font, codepoint);

                float advance = (data.font.glyphs[index].advanceX > 0)
                                    ? data.font.glyphs[index].advanceX
                                    : data.font.recs[index].width;

                float glyphWidth = advance * scaleFactor + data.spacing;

                // Wrap if the glyph would overflow
                if (offsetX + glyphWidth > wrapWidth)
                {
                    offsetX = 0.0f;
                    offsetY += data.size + data.lineSpacing;
                }

                DrawTextCodepoint(data.font, codepoint, (Vector2) { bounds.x + offsetX, bounds.y + offsetY }, data.size, data.color);

                offsetX += glyphWidth;
                j += codepointByteCount;
            }
        }
        else
        {
            // Word fits: wrap to next line if needed
            if (offsetX + wordWidth > wrapWidth)
            {
                offsetX = 0.0f;
                offsetY += data.size + data.lineSpacing;
            }

            int j = 0;
            while (j < wordLen)
            {
                int codepointByteCount = 0;
                int codepoint          = GetCodepointNext(&text[wordStart + j], &codepointByteCount);
                int index              = GetGlyphIndex(data.font, codepoint);

                float advance = (data.font.glyphs[index].advanceX > 0)
                                    ? data.font.glyphs[index].advanceX
                                    : data.font.recs[index].width;

                DrawTextCodepoint(data.font, codepoint, (Vector2) { bounds.x + offsetX, bounds.y + offsetY }, data.size, data.color);

                offsetX += advance * scaleFactor + data.spacing;
                j += codepointByteCount;
            }
        }

        // Process delimiter (space, tab, newline)
        if (i < textLen)
        {
            int codepointByteCount = 0;
            int codepoint          = GetCodepointNext(&text[i], &codepointByteCount);
            int index              = GetGlyphIndex(data.font, codepoint);

            if (codepoint == '\n')
            {
                offsetX = 0.0f;
                offsetY += data.size + data.lineSpacing;
            }
            else if (codepoint == ' ' || codepoint == '\t')
            {
                float advance = (data.font.glyphs[index].advanceX > 0)
                                    ? data.font.glyphs[index].advanceX
                                    : data.font.recs[index].width;

                float spaceWidth = advance * scaleFactor + data.spacing;

                if (offsetX + spaceWidth > wrapWidth)
                {
                    offsetX = 0.0f;
                    offsetY += data.size + data.lineSpacing;
                }
                else
                {
                    offsetX += spaceWidth;
                }
            }

            i += codepointByteCount;
        }
    }
}

void CguiDrawPreTextureElement(CguiNode *node)
{
    if (!node || node->type != CGUI_ELEMENT_NODE_TYPE_TEXTURE)
    {
        return;
    }

    CguiTextureElementData data = *(CguiTextureElementData *) node->data;

    DrawTexturePro(data.texture, data.source, (Rectangle) { node->bounds.x + data.origin.x, node->bounds.y + data.origin.y, node->bounds.width, node->bounds.height }, data.origin, data.rotation, data.tint);
}

void CguiDrawPreBoxElement(CguiNode *node)
{
    if (!node || node->type != CGUI_ELEMENT_NODE_TYPE_BOX)
    {
        return;
    }

    CguiBoxElementData data = *(CguiBoxElementData *) node->data;

    // Set additional textures
    // Raylib doesn't have SHADER_UNIFORM_BOOL ... ???

    SetShaderValue(cguiBoxShader, GetShaderLocation(cguiBoxShader, "useTexture"), (int[1]) { IsTextureValid(data.texture) }, SHADER_UNIFORM_INT);

    if (IsTextureValid(data.shadowTexture))
    {
        SetShaderValueTexture(cguiBoxShader, GetShaderLocation(cguiBoxShader, "texture1"), data.shadowTexture);
        SetShaderValue(cguiBoxShader, GetShaderLocation(cguiBoxShader, "useShadowTexture"), (int[1]) { 1 }, SHADER_UNIFORM_INT);
    }
    else
    {
        SetShaderValue(cguiBoxShader, GetShaderLocation(cguiBoxShader, "useShadowTexture"), (int[1]) { 0 }, SHADER_UNIFORM_INT);
    }
    if (IsTextureValid(data.borderTexture))
    {
        SetShaderValueTexture(cguiBoxShader, GetShaderLocation(cguiBoxShader, "texture2"), data.borderTexture);
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
    SetShaderValue(cguiBoxShader, GetShaderLocation(cguiBoxShader, "radii"), &data.radii, SHADER_UNIFORM_VEC4);

    Vector4 colorN = ColorNormalize(data.color);
    SetShaderValue(cguiBoxShader, GetShaderLocation(cguiBoxShader, "color"), &colorN, SHADER_UNIFORM_VEC4);

    // Set shadow values
    SetShaderValue(cguiBoxShader, GetShaderLocation(cguiBoxShader, "shadowDist"), &data.shadowDist, SHADER_UNIFORM_FLOAT);
    SetShaderValue(cguiBoxShader, GetShaderLocation(cguiBoxShader, "shadowOffset"), &data.shadowOffset, SHADER_UNIFORM_VEC2);
    SetShaderValue(cguiBoxShader, GetShaderLocation(cguiBoxShader, "shadowShrink"), &data.shadowShrink, SHADER_UNIFORM_FLOAT);

    Vector4 shadowColorN = ColorNormalize(data.shadowColor);
    SetShaderValue(cguiBoxShader, GetShaderLocation(cguiBoxShader, "shadowColor"), &shadowColorN, SHADER_UNIFORM_VEC4);

    // Set border values
    SetShaderValue(cguiBoxShader, GetShaderLocation(cguiBoxShader, "borderThickness"), &data.borderThickness, SHADER_UNIFORM_FLOAT);

    Vector4 borderColorN = ColorNormalize(data.borderColor);
    SetShaderValue(cguiBoxShader, GetShaderLocation(cguiBoxShader, "borderColor"), &borderColorN, SHADER_UNIFORM_VEC4);

    // Draw rectangle
    BeginShaderMode(cguiBoxShader);
    if (IsTextureValid(data.texture))
        CguiDrawTextureFullscreen(data.texture, WHITE);
    else
        DrawRectangle(0, 0, CguiGetAppWidth(), CguiGetAppHeight(), WHITE);
    EndShaderMode();

    // Draw shadow
}
