/// @file
///
/// @author    Anstro Pleuton
/// @copyright Copyright (c) 2025 Anstro Pleuton
///
/// Crystal GUI - A GUI framework for raylib.
///
/// This source file contains implementations for theming functionality.
///
/// This project is licensed under the terms of MIT license.

#include <string.h>

#include "crystalgui/crystalgui.h"
#include "raylib.h"

extern int        cguiNameCounter;
extern CguiTheme *cguiDefaultTheme;

extern CguiTheme *cguiActiveTheme;
extern CguiNode  *cguiComponentTemplates[];

CguiTheme *CguiCreateTheme(const char *themeName)
{
    if (!themeName)
    {
        themeName = TextFormat("CguiUnnamedTheme #%d", ++cguiNameCounter);
    }

    CguiTheme *theme = CG_MALLOC_NULL(sizeof(CguiTheme));
    if (!theme)
    {
        return NULL;
    }

    theme->themeName = CG_MALLOC_NULL(strlen(themeName) + 1);
    if (!theme->themeName)
    {
        CG_FREE_NULL(theme);
        return NULL;
    }

    strcpy(theme->themeName, themeName);

    theme->templates = CG_MALLOC_NULL(sizeof(CguiNode *) * CGUI_COMPONENT_MAX);
    if (!theme->templates)
    {
        CG_FREE_NULL(theme->themeName);
        CG_FREE_NULL(theme);
        return NULL;
    }

    return theme;
}

void CguiDeleteTheme(CguiTheme *theme)
{
    if (!theme)
    {
        return;
    }

    if (theme->deleteThemeData)
    {
        theme->deleteThemeData(theme);
    }

    CG_FREE_NULL(theme->themeName);

    CG_FREE_NULL(theme->themeData);
    theme->themeDataSize = 0;

    if (theme->templates)
    {
        for (int i = 0; i < CGUI_COMPONENT_MAX; i++)
        {
            CguiNode *templateNode = theme->templates[i];
            if (templateNode) CguiDeleteNode(templateNode);
        }
    }

    CG_FREE_NULL(theme->templates);
    CG_FREE_NULL(theme);
}

void CguiSetActiveTheme(CguiTheme *theme)
{
    if (!theme)
    {
        return;
    }

    cguiActiveTheme = theme;

    for (int i = 0; i < CGUI_COMPONENT_MAX; i++)
    {
        if (!cguiActiveTheme->templates[i])
        {
            continue;
        }

        const char *newName = TextFormat("%s (CguiThemeTemplate #%d)", cguiActiveTheme->templates[i]->name, ++cguiNameCounter);

        if (!cguiComponentTemplates[i])
        {
            cguiComponentTemplates[i] = CguiCloneNode(cguiActiveTheme->templates[i]);
        }

        if (!CguiRenameNode(cguiComponentTemplates[i], newName))
        {
            continue;
        }

        if (!CguiCopyNodeNoTi(cguiActiveTheme->templates[i], cguiComponentTemplates[i]))
        {
            continue;
        }

        CguiApplyTemplateResync(cguiComponentTemplates[i]);
    }
}

CguiTheme *CguiGetActiveTheme(void)
{
    return cguiActiveTheme;
}

CguiTheme *CguiGetDefaultTheme(void)
{
    return cguiDefaultTheme;
}

CguiNode *CguiGetComponentTemplate(CguiTheme *theme, int componentType)
{
    if (!theme || componentType < 0 || componentType >= CGUI_COMPONENT_MAX)
    {
        return NULL;
    }

    CguiNode *template = theme->templates[componentType];
    return template;
}

bool CguiSetComponentTemplate(CguiTheme *theme, CguiNode *node)
{
    if (!theme || !node)
    {
        return false;
    }

    int componentType = node->type - CGUI_COMPONENT_NODE_TYPE_ROOT;
    if (componentType < 0 || componentType >= CGUI_COMPONENT_MAX)
    {
        return false;
    }

    theme->templates[componentType] = node;
    return true;
}
