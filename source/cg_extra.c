/// @file
///
/// @author    Anstro Pleuton
/// @copyright Copyright (c) 2025 Anstro Pleuton
///
/// Crystal GUI - A modern GUI framework for raylib.
///
/// This source file contains implementations for raylib-like extra features in
/// Crystal GUI.
///
/// This project is licensed under the terms of MIT license.

#include "crystalgui.h"
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
