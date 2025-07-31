/// @file
///
/// @author    Anstro Pleuton
/// @copyright Copyright (c) 2025 Anstro Pleuton
///
/// Crystal GUI - A GUI framework for raylib.
///
/// This source file contains implementations for core features.
///
/// This project is licensed under the terms of MIT license.

#include <stdlib.h>

#include "crystalgui/crystalgui.h"
#include "raylib.h"
#include "raymath.h"

int                                   cguiNameCounter                            = 0;
bool                                  cguiInited                                 = false;
Shader                                cguiBoxShader                              = (Shader) { 0 };
CguiTheme                            *cguiDefaultTheme                           = NULL;
CguiTheme                            *cguiActiveTheme                            = NULL;
CguiNode                             *cguiComponentTemplates[CGUI_COMPONENT_MAX] = { 0 };
CguiNode                             *cguiMouseButtonPressedNode                 = NULL;
struct CguiRegisteredTransitionChain *cguiRegisteredTransitionChains                  = NULL;

#define CGUI_GLSL_VERSION 330

void CguiInit(void)
{
    if (cguiInited)
    {
        return;
    }

    cguiBoxShader = LoadShader(NULL, TextFormat("resource/shaders/glsl%i/box.fs", CGUI_GLSL_VERSION));
    if (cguiBoxShader.id == 0)
    {
        CG_LOG_ERROR("Failed to load Box Shader. Are you missing \"resource\" folder in working directory?");
    }

    cguiDefaultTheme = CguiCreateCrystallineThemeDark();
    if (!cguiDefaultTheme)
    {
        CG_LOG_ERROR("Failed to load Crystalline Dark theme (default theme)");
    }
    CguiSetActiveTheme(cguiDefaultTheme);

    cguiInited = true;
}

void CguiClose(void)
{
    if (!cguiInited)
    {
        return;
    }

    CguiDeleteTheme(cguiDefaultTheme);

    UnloadShader(cguiBoxShader);

    cguiInited = false;
}

void CguiUpdate(CguiNode *root)
{
    for (int i = 0; i < CGUI_COMPONENT_MAX; i++)
    {
        CguiSyncHierarchy(cguiComponentTemplates[i]);
    }

    CguiSyncHierarchy(root);

    CguiUpdateRegisteredTransitions();

    CguiTransformNode(root, IsWindowResized());

    CguiDispatchEvents(root);

    CguiUpdateNode(root);
}

void CguiDraw(CguiNode *root, bool debugBounds)
{
    CguiDrawNode(root);
    if (debugBounds) CguiDebugDrawNode(root);
}
