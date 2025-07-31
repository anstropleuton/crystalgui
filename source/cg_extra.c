/// @file
///
/// @author    Anstro Pleuton
/// @copyright Copyright (c) 2025 Anstro Pleuton
///
/// Crystal GUI - A GUI framework for raylib.
///
/// This source file contains implementations for raylib-like extra features.
///
/// This project is licensed under the terms of MIT license.

#include <math.h>
#include <string.h>

#include "crystalgui/crystalgui.h"
#include "raylib.h"
#include "raymath.h"

// Hotkey

bool CguiIsKeyRepeated(int key)
{
    return IsKeyPressed(key) || IsKeyPressedRepeat(key);
}

bool CguiIsHotkeyDown(int hotkey)
{
    bool isShiftDown   = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
    bool isControlDown = IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);
    bool isAltDown     = IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT);
    bool isSuperDown   = IsKeyDown(KEY_LEFT_SUPER) || IsKeyDown(KEY_RIGHT_SUPER);

    bool isShiftSpec   = (hotkey & CGUI_HOTKEY_SHIFT) == CGUI_HOTKEY_SHIFT;
    bool isControlSpec = (hotkey & CGUI_HOTKEY_CONTROL) == CGUI_HOTKEY_CONTROL;
    bool isAltSpec     = (hotkey & CGUI_HOTKEY_ALT) == CGUI_HOTKEY_ALT;
    bool isSuperSpec   = (hotkey & CGUI_HOTKEY_SUPER) == CGUI_HOTKEY_SUPER;

    return (isShiftDown == isShiftSpec) &&
           (isControlDown == isControlSpec) &&
           (isAltDown == isAltSpec) &&
           (isSuperDown == isSuperSpec);
}

bool CguiIsKeyComboUp(int hotkey, int key)
{
    return CguiIsHotkeyDown(hotkey) && IsKeyUp(key);
}

bool CguiIsKeyComboDown(int hotkey, int key)
{
    return CguiIsHotkeyDown(hotkey) && IsKeyDown(key);
}

bool CguiIsKeyComboPressed(int hotkey, int key)
{
    return CguiIsHotkeyDown(hotkey) && IsKeyPressed(key);
}

bool CguiIsKeyComboReleased(int hotkey, int key)
{
    return CguiIsHotkeyDown(hotkey) && IsKeyReleased(key);
}

// Texts

void CguiDrawTextPro(const char *text, Font font, Rectangle bounds, float fontSize, float spacing, float lineSpacing, Color color, int xJustify, int yJustify)
{
    if (!text)
    {
        return;
    }

    if (font.texture.id == 0) font = GetFontDefault();

    const char *textPtr     = text;
    float       scaleFactor = fontSize / font.baseSize;

    // Pre-pass: count total number of lines and total text height
    int   totalLines      = 0;
    float totalTextHeight = 0;

    const char *measurePtr = textPtr;
    while (*measurePtr != '\0')
    {
        const char *lineStart       = measurePtr;
        const char *scanPtr         = lineStart;
        float       lineWidth       = 0;
        int         wordsCount      = 0;
        int         spaceGlyphIndex = GetGlyphIndex(font, ' ');
        float       spaceWidth      = font.glyphs[spaceGlyphIndex].advanceX * scaleFactor + spacing;

        const char *wordStart = NULL;
        float       wordWidth = 0;

        const char *lineEnd = lineStart;

        while (*scanPtr != '\0')
        {
            if (*scanPtr == '\n')
            {
                lineEnd = scanPtr;
                scanPtr++;
                break;
            }

            if (wordStart == NULL)
            {
                if (*scanPtr == ' ' || *scanPtr == '\t')
                {
                    scanPtr++;
                    continue;
                }
                else
                {
                    wordStart = scanPtr;
                    wordWidth = 0;
                }
            }

            int   cpByteCount  = 0;
            int   codepoint    = GetCodepointNext(scanPtr, &cpByteCount);
            int   glyphIndex   = GetGlyphIndex(font, codepoint);
            float glyphAdvance = (font.glyphs[glyphIndex].advanceX > 0 ? font.glyphs[glyphIndex].advanceX : font.recs[glyphIndex].width) * scaleFactor;

            const char *nextCharPtr      = scanPtr + cpByteCount;
            int         isLastCharInWord = (*nextCharPtr == '\0' || *nextCharPtr == ' ' || *nextCharPtr == '\t' || *nextCharPtr == '\n');
            float       charSpacing      = isLastCharInWord ? 0 : spacing;

            wordWidth += glyphAdvance + charSpacing;

            scanPtr += cpByteCount;

            if (isLastCharInWord)
            {
                float newLineWidth = (wordsCount == 0) ? wordWidth : lineWidth + spaceWidth + wordWidth;

                if (newLineWidth <= bounds.width)
                {
                    lineWidth = newLineWidth;
                    lineEnd   = scanPtr;
                    wordsCount++;
                    wordStart = NULL;
                }
                else
                {
                    if (wordsCount == 0)
                    {
                        // handle single long word char by char
                        scanPtr                      = wordStart;
                        float       partialWordWidth = 0;
                        const char *partialWordEnd   = scanPtr;

                        while (*scanPtr != '\0')
                        {
                            int   cpByteCount2  = 0;
                            int   cp2           = GetCodepointNext(scanPtr, &cpByteCount2);
                            int   glyphIndex2   = GetGlyphIndex(font, cp2);
                            float glyphAdvance2 = (font.glyphs[glyphIndex2].advanceX > 0 ? font.glyphs[glyphIndex2].advanceX : font.recs[glyphIndex2].width) * scaleFactor;

                            const char *nextPtr2     = scanPtr + cpByteCount2;
                            float       charSpacing2 = (*nextPtr2 == '\0' || *nextPtr2 == ' ' || *nextPtr2 == '\t' || *nextPtr2 == '\n') ? 0 : spacing;

                            float newWidth = partialWordWidth + glyphAdvance2 + charSpacing2;

                            if (newWidth <= bounds.width)
                            {
                                partialWordWidth = newWidth;
                                partialWordEnd   = nextPtr2;
                                scanPtr += cpByteCount2;
                            }
                            else
                            {
                                break;
                            }
                        }

                        lineWidth  = partialWordWidth;
                        lineEnd    = partialWordEnd;
                        wordsCount = 1;
                    }
                    else
                    {
                        // can't fit this word; stop line
                        break;
                    }
                }
            }
        }

        totalLines++;
        totalTextHeight += fontSize + lineSpacing;

        // Key fix: always advance measurePtr, even when line is full
        if (scanPtr > lineStart)
        {
            measurePtr = scanPtr;
        }
        else
        {
            // avoid infinite loop: move at least one byte
            measurePtr++;
        }

        if (*measurePtr == '\n') measurePtr++;
    }
    if (totalLines > 0)
    {
        totalTextHeight -= lineSpacing;
    }

    // Compute starting posY based on yJustify
    float posY = bounds.y;

    if (yJustify == CGUI_TEXT_JUSTIFY_CENTER)
    {
        posY += (bounds.height - totalTextHeight) / 2;
    }
    else if (yJustify == CGUI_TEXT_JUSTIFY_END)
    {
        posY += bounds.height - totalTextHeight;
    }
    // else BEGIN: posY = bounds.y

    // Second pass: actually draw text
    textPtr = text;
    while (*textPtr != '\0' && posY + fontSize <= bounds.y + bounds.height)
    {
        const char *lineStart       = textPtr;
        const char *scanPtr         = lineStart;
        float       lineWidth       = 0;
        int         wordsCount      = 0;
        int         spaceGlyphIndex = GetGlyphIndex(font, ' ');
        float       spaceWidth      = font.glyphs[spaceGlyphIndex].advanceX * scaleFactor + spacing;

        const char *wordStart = NULL;
        float       wordWidth = 0;

        const char *lineEnd = lineStart;

        while (*scanPtr != '\0')
        {
            if (*scanPtr == '\n')
            {
                lineEnd = scanPtr;
                scanPtr++;
                break;
            }

            if (wordStart == NULL)
            {
                if (*scanPtr == ' ' || *scanPtr == '\t')
                {
                    scanPtr++;
                    continue;
                }
                else
                {
                    wordStart = scanPtr;
                    wordWidth = 0;
                }
            }

            int   cpByteCount  = 0;
            int   codepoint    = GetCodepointNext(scanPtr, &cpByteCount);
            int   glyphIndex   = GetGlyphIndex(font, codepoint);
            float glyphAdvance = (font.glyphs[glyphIndex].advanceX > 0 ? font.glyphs[glyphIndex].advanceX : font.recs[glyphIndex].width) * scaleFactor;

            const char *nextCharPtr      = scanPtr + cpByteCount;
            int         isLastCharInWord = (*nextCharPtr == '\0' || *nextCharPtr == ' ' || *nextCharPtr == '\t' || *nextCharPtr == '\n');
            float       charSpacing      = isLastCharInWord ? 0 : spacing;

            wordWidth += glyphAdvance + charSpacing;

            scanPtr += cpByteCount;

            if (isLastCharInWord)
            {
                float newLineWidth = (wordsCount == 0) ? wordWidth : lineWidth + spaceWidth + wordWidth;

                if (newLineWidth <= bounds.width)
                {
                    lineWidth = newLineWidth;
                    lineEnd   = scanPtr;
                    wordsCount++;
                    wordStart = NULL;
                }
                else
                {
                    if (wordsCount == 0)
                    {
                        scanPtr                      = wordStart;
                        float       partialWordWidth = 0;
                        const char *partialWordEnd   = scanPtr;

                        while (*scanPtr != '\0')
                        {
                            int   cpByteCount2  = 0;
                            int   cp2           = GetCodepointNext(scanPtr, &cpByteCount2);
                            int   glyphIndex2   = GetGlyphIndex(font, cp2);
                            float glyphAdvance2 = (font.glyphs[glyphIndex2].advanceX > 0 ? font.glyphs[glyphIndex2].advanceX : font.recs[glyphIndex2].width) * scaleFactor;

                            const char *nextPtr2     = scanPtr + cpByteCount2;
                            float       charSpacing2 = (*nextPtr2 == '\0' || *nextPtr2 == ' ' || *nextPtr2 == '\t' || *nextPtr2 == '\n') ? 0 : spacing;

                            float newWidth = partialWordWidth + glyphAdvance2 + charSpacing2;

                            if (newWidth <= bounds.width)
                            {
                                partialWordWidth = newWidth;
                                partialWordEnd   = nextPtr2;
                                scanPtr += cpByteCount2;
                            }
                            else
                            {
                                break;
                            }
                        }

                        lineWidth  = partialWordWidth;
                        lineEnd    = partialWordEnd;
                        wordsCount = 1;
                    }
                    break;
                }
            }
        }

        // Draw the line
        float offsetX      = bounds.x;
        int   spacesInLine = (wordsCount > 1) ? wordsCount - 1 : 0;
        float extraSpace   = 0;

        if (xJustify == CGUI_TEXT_JUSTIFY_CENTER)
        {
            offsetX += (bounds.width - lineWidth) / 2;
        }
        else if (xJustify == CGUI_TEXT_JUSTIFY_END)
        {
            offsetX += bounds.width - lineWidth;
        }
        else if (xJustify == CGUI_TEXT_JUSTIFY_SPACE_BETWEEN && spacesInLine > 0)
        {
            extraSpace = (bounds.width - lineWidth) / (float) spacesInLine;
        }

        const char *drawPtr   = lineStart;
        int         wordIndex = 0;

        while (drawPtr < lineEnd)
        {
            if (*drawPtr == ' ' || *drawPtr == '\t')
            {
                drawPtr++;
                continue;
            }

            while (drawPtr < lineEnd && *drawPtr != ' ' && *drawPtr != '\t' && *drawPtr != '\n')
            {
                int   cpByteCount  = 0;
                int   codepoint    = GetCodepointNext(drawPtr, &cpByteCount);
                int   glyphIndex   = GetGlyphIndex(font, codepoint);
                float glyphAdvance = (font.glyphs[glyphIndex].advanceX > 0 ? font.glyphs[glyphIndex].advanceX : font.recs[glyphIndex].width) * scaleFactor;

                DrawTextCodepoint(font, codepoint, (Vector2) { offsetX, posY }, fontSize, color);

                offsetX += glyphAdvance + spacing;
                drawPtr += cpByteCount;
            }

            offsetX -= spacing;
            wordIndex++;

            if (xJustify == CGUI_TEXT_JUSTIFY_SPACE_BETWEEN && wordIndex < wordsCount)
                offsetX += spaceWidth + extraSpace;
            else if (wordIndex < wordsCount)
                offsetX += spaceWidth;
        }

        posY += fontSize + lineSpacing;

        textPtr = lineEnd;
        if (*textPtr == '\n') textPtr++;
    }
}

// Textures

void CguiDrawTextureDest(Texture texture, Rectangle dest, Color tint)
{
    DrawTexturePro(texture, CguiGetTextureSizeRec(texture), dest, Vector2Zero(), 0.0f, tint);
}

void CguiDrawTextureFullscreen(Texture texture, Color tint)
{
    DrawTexturePro(texture, CguiGetTextureSizeRec(texture), CguiGetAppSizeRec(), Vector2Zero(), 0.0f, tint);
}

void CguiDrawTextureFullscreenEx(Texture texture, Rectangle src, Color tint)
{
    DrawTexturePro(texture, src, CguiGetAppSizeRec(), Vector2Zero(), 0.0f, tint);
}

void CguiDrawRenderTextureDest(RenderTexture renderTexture, Rectangle dest, Color tint)
{
    DrawTexturePro(renderTexture.texture, CguiFlipRectangleY(CguiGetRenderTextureSizeRec(renderTexture), 0.0f), dest, Vector2Zero(), 0.0f, tint);
}

void CguiDrawRenderTextureFullscreen(RenderTexture renderTexture, Color tint)
{
    DrawTexturePro(renderTexture.texture, CguiFlipRectangleY(CguiGetRenderTextureSizeRec(renderTexture), 0.0f), CguiGetAppSizeRec(), Vector2Zero(), 0.0f, tint);
}

void CguiDrawRenderTextureFullscreenEx(RenderTexture renderTexture, Rectangle src, Color tint)
{
    DrawTexturePro(renderTexture.texture, CguiFlipRectangleY(src, 0.0f), CguiGetAppSizeRec(), Vector2Zero(), 0.0f, tint);
}

Vector2 CguiGetTextureSizeV(Texture texture)
{
    return (Vector2) { (float) texture.width, (float) texture.height };
}

Rectangle CguiGetTextureSizeRec(Texture texture)
{
    return (Rectangle) { 0.0f, 0.0f, (float) texture.width, (float) texture.height };
}

Vector2 CguiGetRenderTextureSizeV(RenderTexture renderTexture)
{
    return CguiGetTextureSizeV(renderTexture.texture);
}

Rectangle CguiGetRenderTextureSizeRec(RenderTexture renderTexture)
{
    return CguiGetTextureSizeRec(renderTexture.texture);
}

// Sizes

Vector2 CguiGetScreenSizeV(void)
{
    return (Vector2) { (float) GetScreenWidth(), (float) GetScreenHeight() };
}

Rectangle CguiGetScreenSizeRec(void)
{
    return (Rectangle) { 0.0f, 0.0f, (float) GetScreenWidth(), (float) GetScreenHeight() };
}

Vector2 CguiGetMonitorSizeV(int monitor)
{
    return (Vector2) { (float) GetMonitorWidth(monitor), (float) GetMonitorHeight(monitor) };
}

Rectangle CguiGetMonitorSizeRec(int monitor)
{
    return (Rectangle) { 0.0f, 0.0f, (float) GetMonitorWidth(monitor), (float) GetMonitorHeight(monitor) };
}

int CguiGetAppWidth(void)
{
    if (IsWindowFullscreen())
        return GetMonitorWidth(GetCurrentMonitor());
    else
        return GetScreenWidth();
}

int CguiGetAppHeight(void)
{
    if (IsWindowFullscreen())
        return GetMonitorHeight(GetCurrentMonitor());
    else
        return GetScreenHeight();
}

Vector2 CguiGetAppSizeV(void)
{
    return (Vector2) { (float) CguiGetAppWidth(), (float) CguiGetAppHeight() };
}

Rectangle CguiGetAppSizeRec(void)
{
    return (Rectangle) { 0.0f, 0.0f, (float) CguiGetAppWidth(), (float) CguiGetAppHeight() };
}

// Misc.

Vector2 CguiRotatePoint(Vector2 point, Vector2 origin, float angle)
{
    Vector2 po      = Vector2Subtract(point, origin);
    Vector2 rotated = Vector2Rotate(po, angle);
    Vector2 ro      = Vector2Add(rotated, origin);
    return ro;
}

Rectangle CguiRecZero(void)
{
    return (Rectangle) { 0.0f, 0.0f, 0.0f, 0.0f };
}

bool CguiIsRectangleEqual(Rectangle a, Rectangle b)
{
    return a.x == b.x && a.y == b.y && a.width == b.width && a.height == b.height;
}

bool CguiIsColorEqual(Color a, Color b)
{
    return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}

Texture CguiLoadTextureFromRenderTexture(RenderTexture renderTexture)
{
    Image image = LoadImageFromTexture(renderTexture.texture);
    ImageFlipVertical(&image);
    Texture texture = LoadTextureFromImage(image);
    UnloadImage(image);
    return texture;
}

float CguiGetMinRoundness(Rectangle rec, float roundness)
{
    return rec.width < rec.height ? roundness / rec.width : roundness / rec.height;
}

Color CguiColorMultiply(Color color, Vector4 values)
{
    return (Color) {
        (unsigned char) (color.r * values.x),
        (unsigned char) (color.g * values.y),
        (unsigned char) (color.b * values.z),
        (unsigned char) (color.a * values.w)
    };
}

Color CguiColorFromHSL(float hue, float saturation, float lightness)
{
    return CguiColorFromHSLA(hue, saturation, lightness, 1.0f);
}

Color CguiColorFromHSLA(float hue, float saturation, float lightness, float alpha)
{
    float h = fmodf(hue, 360.0f);
    if (h < 0) h += 360.0f;
    float s = Clamp(saturation, 0.0f, 1.0f);
    float l = Clamp(lightness, 0.0f, 1.0f);
    float a = Clamp(alpha, 0.0f, 1.0f);

    float c       = (1.0f - fabsf(2.0f * l - 1.0f)) * s;
    float h_prime = h / 60.0f;
    float x       = c * (1.0f - fabsf(fmodf(h_prime, 2.0f) - 1.0f));
    float r1, g1, b1;

    if (h_prime >= 0 && h_prime < 1)
    {
        r1 = c;
        g1 = x;
        b1 = 0;
    }
    else if (h_prime < 2)
    {
        r1 = x;
        g1 = c;
        b1 = 0;
    }
    else if (h_prime < 3)
    {
        r1 = 0;
        g1 = c;
        b1 = x;
    }
    else if (h_prime < 4)
    {
        r1 = 0;
        g1 = x;
        b1 = c;
    }
    else if (h_prime < 5)
    {
        r1 = x;
        g1 = 0;
        b1 = c;
    }
    else
    {
        r1 = c;
        g1 = 0;
        b1 = x;
    }

    float m = l - c / 2.0f;
    float r = Clamp(r1 + m, 0.0f, 1.0f);
    float g = Clamp(g1 + m, 0.0f, 1.0f);
    float b = Clamp(b1 + m, 0.0f, 1.0f);

    Color color = {
        (unsigned char) (r * 255),
        (unsigned char) (g * 255),
        (unsigned char) (b * 255),
        (unsigned char) (a * 255)
    };
    return color;
}

Color CguiColorFromVecHSL(Vector3 hsl)
{
    return CguiColorFromHSLA(hsl.x, hsl.y, hsl.z, 1.0f);
}

Color CguiColorFromVecHSLA(Vector4 hsla)
{
    return CguiColorFromHSLA(hsla.x, hsla.y, hsla.z, hsla.w);
}

Vector3 CguiColorToHSLA(Color color)
{
    float r = color.r / 255.0f;
    float g = color.g / 255.0f;
    float b = color.b / 255.0f;

    float max   = fmaxf(r, fmaxf(g, b));
    float min   = fminf(r, fminf(g, b));
    float delta = max - min;

    float h = 0.0f;
    float s = 0.0f;
    float l = (max + min) / 2.0f;

    if (delta == 0)
    {
        h = 0.0f;
        s = 0.0f;
    }
    else
    {
        s = (l < 0.5f) ? (delta / (max + min)) : (delta / (2.0f - max - min));

        if (max == r)
            h = 60.0f * fmodf(((g - b) / delta), 6.0f);
        else if (max == g)
            h = 60.0f * (((b - r) / delta) + 2.0f);
        else
            h = 60.0f * (((r - g) / delta) + 4.0f);

        if (h < 0) h += 360.0f;
    }

    Vector3 hsl = { h, s, l };
    return hsl;
}

Vector4 CguiColorToVecHSLA(Color color)
{
    Vector3 hsl  = CguiColorToHSLA(color);
    Vector4 hsla = { hsl.x, hsl.y, hsl.z, color.a / 255.0f };
    return hsla;
}

Vector4 CguiColorToValues(Color color)
{
    return (Vector4) {
        color.r / 255.0f,
        color.g / 255.0f,
        color.b / 255.0f,
        color.a / 255.0f
    };
}

Color CguiColorFromValues(Vector4 color)
{
    return (Color) {
        (unsigned char) (color.x * 255.0f),
        (unsigned char) (color.y * 255.0f),
        (unsigned char) (color.z * 255.0f),
        (unsigned char) (color.w * 255.0f)
    };
}

void CguiBeginScissorModeRec(Rectangle area)
{
    BeginScissorMode(area.x, area.y, area.width, area.height);
}

Rectangle CguiFlipRectangleX(Rectangle rec, float axis)
{
    rec.x -= axis;
    rec.x *= -1;
    rec.x -= rec.width;
    rec.x += axis;
    return rec;
}

Rectangle CguiFlipRectangleY(Rectangle rec, float axis)
{
    rec.y -= axis;
    rec.y *= -1;
    rec.y -= rec.height;
    rec.y += axis;
    return rec;
}

Rectangle CguiFlipRectangleXY(Rectangle rec, Vector2 axis)
{
    return CguiFlipRectangleX(CguiFlipRectangleY(rec, axis.y), axis.x);
}
