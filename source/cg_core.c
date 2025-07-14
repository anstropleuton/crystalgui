/// @file
///
/// @author    Anstro Pleuton
/// @copyright Copyright (c) 2025 Anstro Pleuton
///
/// Crystal GUI - A modern GUI framework for raylib.
///
/// This source file contains implementations for core features of Crystal GUI.
///
/// This project is licensed under the terms of MIT license.

#include <stdlib.h>

#include "crystalgui.h"
#include "raylib.h"

bool   cguiInited    = false;
Shader cguiBoxShader = (Shader) { 0 };

#define CGUI_GLSL_VERSION 330

void CguiInit(void)
{
    if (cguiInited)
    {
        return;
    }

    cguiBoxShader = LoadShader(NULL, TextFormat("resources/shaders/glsl%i/box.fs", CGUI_GLSL_VERSION));
    if (cguiBoxShader.id == 0)
    {
        CG_LOG_ERROR("Failed to load Box Shader. Are you missing \"resources\" folder in working directory?");
    }

    cguiInited = true;
}

void CguiClose(void)
{
    if (!cguiInited)
    {
        return;
    }

    UnloadShader(cguiBoxShader);

    cguiInited = false;
}
