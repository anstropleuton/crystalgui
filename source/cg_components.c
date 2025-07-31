/// @file
///
/// @author    Anstro Pleuton
/// @copyright Copyright (c) 2025 Anstro Pleuton
///
/// Crystal GUI - A GUI framework for raylib.
///
/// This source file contains implementations for components.
///
/// This project is licensed under the terms of MIT license.

#include <string.h>

#include "crystalgui/crystalgui.h"
#include "raylib.h"
#include "raymath.h"

extern CguiNode *cguiComponentTemplates[];

void CguiApplyOverrides(CguiNode *node, CguiCommonOverrides overrides)
{
    if (!node)
    {
        return;
    }

    if (overrides.fields & CGUI_COMMON_OVERRIDE_FIELD_ENABLED)
    {
        node->enabled = overrides.enabled;
    }

    if (overrides.fields & CGUI_COMMON_OVERRIDE_FIELD_NAME)
    {
        CguiRenameNode(node, overrides.name);
    }

    if (overrides.fields & CGUI_COMMON_OVERRIDE_FIELD_TRANSFORMATION)
    {
        node->transformation = overrides.transformation;
        node->rebound        = true;
    }
}

void CguiOverrideEnabled(CguiNode *node, CguiCommonOverrides *overrides, bool enabled)
{
    if (!overrides)
    {
        return;
    }

    overrides->fields |= CGUI_COMMON_OVERRIDE_FIELD_ENABLED;
    overrides->enabled = enabled;

    CguiApplyOverrides(node, *overrides);
}

void CguiOverrideName(CguiNode *node, CguiCommonOverrides *overrides, const char *name)
{
    if (!overrides)
    {
        return;
    }

    overrides->fields |= CGUI_COMMON_OVERRIDE_FIELD_NAME;
    overrides->name = name;

    CguiApplyOverrides(node, *overrides);
}

void CguiOverrideTransformation(CguiNode *node, CguiCommonOverrides *overrides, CguiTransformation transformation)
{
    if (!overrides)
    {
        return;
    }

    overrides->fields |= CGUI_COMMON_OVERRIDE_FIELD_TRANSFORMATION;
    overrides->transformation = transformation;

    CguiApplyOverrides(node, *overrides);
}

CguiNode *CguiCreateRoot(void)
{
    CguiNode *node = CguiCreateInstance(cguiComponentTemplates[CGUI_COMPONENT_ROOT]);
    if (!node)
    {
        return NULL;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_ROOT || !node->data || !node->instanceData)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    CguiRootData         *data  = node->data;
    CguiRootInstanceData *iData = node->instanceData;

    node->override  = CguiOverrideRoot;
    node->updatePre = CguiPreUpdateRoot;
    node->drawPre   = CguiPreDrawRoot;

    iData->targetBackgroundColor = data->backgroundColor;
    iData->transitionChain       = CguiCreateTransitionChain();

    if (!iData->transitionChain)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    CguiTransition *transition = CguiTransitColor(&iData->currentBackgroundColor, &iData->targetBackgroundColor, &iData->transitioningBackgroundColor, data->transitionEasing, data->transitionDuration);
    if (!transition)
    {
        CguiDeleteTransitionChain(iData->transitionChain);
        CguiDeleteNode(node);
        return NULL;
    }

    if (!CguiInsertTransition(iData->transitionChain, transition))
    {
        CguiDeleteTransition(transition);
        CguiDeleteTransitionChain(iData->transitionChain);
        CguiDeleteNode(node);
        return NULL;
    }

    CguiOverrideTransformation(node, &iData->overrides, CguiTFillParent());

    return node;
}

void CguiPreUpdateRoot(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_ROOT || !node->data || !node->instanceData)
    {
        return;
    }

    CguiRootData         *data  = node->data;
    CguiRootInstanceData *iData = node->instanceData;

    if (!CguiIsColorEqual(iData->targetBackgroundColor, data->backgroundColor))
    {
        iData->targetBackgroundColor  = data->backgroundColor;
        iData->currentBackgroundColor = iData->transitioningBackgroundColor;

        CguiTransition *transition         = iData->transitionChain->first;
        iData->transitionChain->active     = transition;
        iData->transitionChain->activeTime = 0.0f;
        iData->transitionChain->finished   = false;
    }

    CguiUpdateTransitionChain(iData->transitionChain);
}

void CguiPreDrawRoot(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_ROOT || !node->data || !node->instanceData)
    {
        return;
    }

    CguiRootData         *data  = node->data;
    CguiRootInstanceData *iData = node->instanceData;

    DrawRectangleRec(node->bounds, iData->transitioningBackgroundColor);
}

void CguiOverrideRoot(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_ROOT || !node->data || !node->instanceData)
    {
        return;
    }

    CguiRootData         *data  = node->data;
    CguiRootInstanceData *iData = node->instanceData;

    node->updatePre = CguiPreUpdateRoot;
    node->drawPre   = CguiPreDrawRoot;

    CguiApplyOverrides(node, iData->overrides);
}

CguiNode *CguiCreateLayer(CguiTransformation transformation, int type)
{
    CguiNode *node = CguiCreateInstance(cguiComponentTemplates[CGUI_COMPONENT_LAYER]);
    if (!node)
    {
        return NULL;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_LAYER || !node->data || !node->instanceData)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    if (node->childrenCount == 0)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    CguiLayerData         *data       = node->data;
    CguiLayerInstanceData *iData      = node->instanceData;
    CguiNode              *boxNodeRef = node->children[CGUI_LAYER_COMPOSITION_BOX_NODE];
    if (boxNodeRef->type != CGUI_ELEMENT_NODE_TYPE_BOX || !boxNodeRef->data)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    CguiTextElementData *boxNodeData = boxNodeRef->data;

    node->updatePre = CguiPreUpdateLayer;
    node->override  = CguiOverrideLayer;

    iData->type = type;

    if (iData->type >= 0 && iData->type < CGUI_LAYER_TYPE_MAX)
    {
        iData->targetBoxData = data->boxDatas[iData->type];
    }

    iData->transitionChain = CguiCreateTransitionChain();

    if (!iData->transitionChain)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    CguiTransition *transition = CguiTransitBoxElementData(&iData->currentBoxData, &iData->targetBoxData, &iData->transitioningBoxData, data->transitionEasing, data->transitionDuration);
    if (!transition)
    {
        CguiDeleteTransitionChain(iData->transitionChain);
        CguiDeleteNode(node);
        return NULL;
    }

    if (!CguiInsertTransition(iData->transitionChain, transition))
    {
        CguiDeleteTransition(transition);
        CguiDeleteTransitionChain(iData->transitionChain);
        CguiDeleteNode(node);
        return NULL;
    }

    CguiOverrideTransformation(node, &iData->overrides, transformation);

    return node;
}

void CguiPreUpdateLayer(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_LAYER || !node->data || !node->instanceData)
    {
        return;
    }

    if (node->childrenCount == 0)
    {
        return;
    }

    CguiLayerData         *data       = node->data;
    CguiLayerInstanceData *iData      = node->instanceData;
    CguiNode              *boxNodeRef = node->children[CGUI_LAYER_COMPOSITION_BOX_NODE];
    if (boxNodeRef->type != CGUI_ELEMENT_NODE_TYPE_BOX || !boxNodeRef->data)
    {
        return;
    }

    CguiBoxElementData *boxNodeData = boxNodeRef->data;

    if (iData->type < 0 || iData->type >= CGUI_LAYER_TYPE_MAX)
    {
        return;
    }

    CguiBoxElementData boxData = data->boxDatas[iData->type];

    if (!CguiIsBoxElementDataEqual(iData->targetBoxData, boxData))
    {
        iData->targetBoxData  = boxData;
        iData->currentBoxData = iData->transitioningBoxData;

        CguiTransition *transition         = iData->transitionChain->first;
        iData->transitionChain->active     = transition;
        iData->transitionChain->activeTime = 0.0f;
        iData->transitionChain->finished   = false;
    }

    CguiUpdateTransitionChain(iData->transitionChain);

    *boxNodeData = iData->transitioningBoxData;
}

void CguiOverrideLayer(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_LAYER || !node->data || !node->instanceData)
    {
        return;
    }

    if (node->childrenCount == 0)
    {
        return;
    }

    CguiLayerData         *data       = node->data;
    CguiLayerInstanceData *iData      = node->instanceData;
    CguiNode              *boxNodeRef = node->children[CGUI_LAYER_COMPOSITION_BOX_NODE];
    if (boxNodeRef->type != CGUI_ELEMENT_NODE_TYPE_BOX || !boxNodeRef->data)
    {
        return;
    }

    CguiTextElementData *boxNodeData = boxNodeRef->data;

    node->updatePre = CguiPreUpdateLayer;

    CguiApplyOverrides(node, iData->overrides);
}

CguiNode *CguiCreateLabel(CguiTransformation transformation, const char *text, int type, bool disabled, int xJustify, int yJustify)
{
    CguiNode *node = CguiCreateInstance(cguiComponentTemplates[CGUI_COMPONENT_LABEL]);
    if (!node)
    {
        return NULL;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_LABEL || !node->data || !node->instanceData)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    if (node->childrenCount == 0)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    CguiLabelData         *data        = node->data;
    CguiLabelInstanceData *iData       = node->instanceData;
    CguiNode              *textNodeRef = node->children[CGUI_LABEL_COMPOSITION_TEXT_NODE];
    if (textNodeRef->type != CGUI_ELEMENT_NODE_TYPE_TEXT || !textNodeRef->data)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    CguiTextElementData *textNodeData = textNodeRef->data;

    if (iData->type >= 0 && iData->type < CGUI_LAYER_TYPE_MAX)
    {
        iData->targetTextData = data->textDatas[iData->type];
    }

    iData->transitionChain = CguiCreateTransitionChain();

    if (!iData->transitionChain)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    CguiTransition *transition = CguiTransitTextElementData(&iData->currentTextData, &iData->targetTextData, &iData->transitioningTextData, data->transitionEasing, data->transitionDuration);
    if (!transition)
    {
        CguiDeleteTransitionChain(iData->transitionChain);
        CguiDeleteNode(node);
        return NULL;
    }

    if (!CguiInsertTransition(iData->transitionChain, transition))
    {
        CguiDeleteTransition(transition);
        CguiDeleteTransitionChain(iData->transitionChain);
        CguiDeleteNode(node);
        return NULL;
    }

    node->override  = CguiOverrideLabel;
    node->updatePre = CguiPreUpdateLabel;

    iData->text     = text;
    iData->type     = type;
    iData->disabled = disabled;
    iData->xJustify = xJustify;
    iData->yJustify = yJustify;

    CguiOverrideTransformation(node, &iData->overrides, transformation);

    return node;
}

void CguiPreUpdateLabel(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_LABEL || !node->data || !node->instanceData)
    {
        return;
    }

    if (node->childrenCount == 0)
    {
        return;
    }

    CguiLabelData         *data        = node->data;
    CguiLabelInstanceData *iData       = node->instanceData;
    CguiNode              *textNodeRef = node->children[CGUI_LABEL_COMPOSITION_TEXT_NODE];
    if (textNodeRef->type != CGUI_ELEMENT_NODE_TYPE_TEXT || !textNodeRef->data)
    {
        return;
    }

    CguiTextElementData *textNodeData = textNodeRef->data;

    if (iData->type < 0 || iData->type >= CGUI_LABEL_TYPE_MAX)
    {
        return;
    }

    CguiTextElementData textData = data->textDatas[iData->type];

    if (iData->disabled)
    {
        textData = data->disabledTextDatas[iData->type];
    }

    if (!CguiIsTextElementDataEqual(iData->targetTextData, textData))
    {
        iData->targetTextData  = textData;
        iData->currentTextData = iData->transitioningTextData;

        CguiTransition *transition         = iData->transitionChain->first;
        iData->transitionChain->active     = transition;
        iData->transitionChain->activeTime = 0.0f;
        iData->transitionChain->finished   = false;
    }

    CguiUpdateTransitionChain(iData->transitionChain);

    *textNodeData = iData->transitioningTextData;

    textNodeData->text     = iData->text;
    textNodeData->xJustify = iData->xJustify;
    textNodeData->yJustify = iData->yJustify;
}

void CguiOverrideLabel(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_LABEL || !node->data || !node->instanceData)
    {
        return;
    }

    if (node->childrenCount == 0)
    {
        return;
    }

    CguiLabelData         *data        = node->data;
    CguiLabelInstanceData *iData       = node->instanceData;
    CguiNode              *textNodeRef = node->children[CGUI_LABEL_COMPOSITION_TEXT_NODE];
    if (textNodeRef->type != CGUI_ELEMENT_NODE_TYPE_TEXT || !textNodeRef->data)
    {
        return;
    }

    CguiTextElementData *textNodeData = textNodeRef->data;

    node->updatePre = CguiPreUpdateLabel;

    CguiApplyOverrides(node, iData->overrides);
}

CguiNode *CguiCreateButton(CguiTransformation transformation, int type, CguiButtonPressCallback pressCallback, bool disabled)
{
    CguiNode *node = CguiCreateInstance(cguiComponentTemplates[CGUI_COMPONENT_BUTTON]);
    if (!node)
    {
        return NULL;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_BUTTON || !node->data || !node->instanceData)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    if (node->childrenCount == 0)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    CguiButtonData         *data       = node->data;
    CguiButtonInstanceData *iData      = node->instanceData;
    CguiNode               *boxNodeRef = node->children[CGUI_BUTTON_COMPOSITION_BOX_NODE];
    if (boxNodeRef->type != CGUI_ELEMENT_NODE_TYPE_BOX || !boxNodeRef->data)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    CguiTextElementData *boxNodeData = boxNodeRef->data;

    iData->transitionChain = CguiCreateTransitionChain();

    if (!iData->transitionChain)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    CguiTransition *transition = CguiTransitBoxElementData(&iData->currentBoxData, &iData->targetBoxData, &iData->transitioningBoxData, data->transitionEasing, data->transitionDuration);
    if (!transition)
    {
        CguiDeleteTransitionChain(iData->transitionChain);
        CguiDeleteNode(node);
        return NULL;
    }

    if (!CguiInsertTransition(iData->transitionChain, transition))
    {
        CguiDeleteTransition(transition);
        CguiDeleteTransitionChain(iData->transitionChain);
        CguiDeleteNode(node);
        return NULL;
    }

    node->override             = CguiOverrideButton;
    node->updatePre            = CguiPreUpdateButton;
    node->canHandleMouseEvents = true;
    node->handleEvent          = CguiHandleButtonEvents;

    iData->mouseButton   = MOUSE_BUTTON_LEFT;
    iData->type          = type;
    iData->pressCallback = pressCallback;
    iData->disabled      = disabled;

    CguiOverrideTransformation(node, &iData->overrides, transformation);

    return node;
}

void CguiPreUpdateButton(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_BUTTON || !node->data || !node->instanceData)
    {
        return;
    }

    if (node->childrenCount == 0)
    {
        return;
    }

    CguiButtonData         *data       = node->data;
    CguiButtonInstanceData *iData      = node->instanceData;
    CguiNode               *boxNodeRef = node->children[CGUI_BUTTON_COMPOSITION_BOX_NODE];
    if (boxNodeRef->type != CGUI_ELEMENT_NODE_TYPE_BOX || !boxNodeRef->data)
    {
        return;
    }

    CguiBoxElementData *boxNodeData = boxNodeRef->data;

    if (!iData->held) iData->active = false; // Keep active only for one frame

    if (iData->type < 0 || iData->type >= CGUI_BUTTON_TYPE_MAX)
    {
        return;
    }

    CguiBoxElementData boxData = data->boxDatas[iData->type];

    if (iData->hovered)
    {
        boxData = data->hoveredBoxDatas[iData->type];
    }

    if (iData->held)
    {
        boxData = data->heldBoxDatas[iData->type];
    }

    if (iData->disabled)
    {
        boxData = data->disabledBoxDatas[iData->type];
    }

    if (!CguiIsBoxElementDataEqual(iData->targetBoxData, boxData))
    {
        iData->targetBoxData  = boxData;
        iData->currentBoxData = iData->transitioningBoxData;

        CguiTransition *transition         = iData->transitionChain->first;
        iData->transitionChain->active     = transition;
        iData->transitionChain->activeTime = 0.0f;
        iData->transitionChain->finished   = false;
    }

    CguiUpdateTransitionChain(iData->transitionChain);

    *boxNodeData = iData->transitioningBoxData;
}

void CguiOverrideButton(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_BUTTON || !node->data || !node->instanceData)
    {
        return;
    }

    if (node->childrenCount == 0)
    {
        return;
    }

    CguiButtonData         *data       = node->data;
    CguiButtonInstanceData *iData      = node->instanceData;
    CguiNode               *boxNodeRef = node->children[CGUI_BUTTON_COMPOSITION_BOX_NODE];
    if (boxNodeRef->type != CGUI_ELEMENT_NODE_TYPE_BOX || !boxNodeRef->data)
    {
        return;
    }

    CguiBoxElementData *boxNodeData = boxNodeRef->data;

    node->updatePre            = CguiPreUpdateButton;
    node->canHandleMouseEvents = true;
    node->handleEvent          = CguiHandleButtonEvents;

    CguiApplyOverrides(node, iData->overrides);
}

bool CguiHandleButtonEvents(CguiNode *node, CguiEvent *event)
{
    if (!node || !event)
    {
        return false;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_BUTTON || !node->data || !node->instanceData)
    {
        return false;
    }

    if (node->childrenCount == 0)
    {
        return false;
    }

    CguiButtonData         *data       = node->data;
    CguiButtonInstanceData *iData      = node->instanceData;
    CguiNode               *boxNodeRef = node->children[CGUI_BUTTON_COMPOSITION_BOX_NODE];
    if (boxNodeRef->type != CGUI_ELEMENT_NODE_TYPE_BOX || !boxNodeRef->data)
    {
        return false;
    }

    CguiBoxElementData *boxNodeData = boxNodeRef->data;

    if (iData->disabled)
    {
        return false;
    }

    switch (event->eventType)
    {
        case CGUI_EVENT_TYPE_MOUSE_CURSOR_MOVE: {
            CguiMouseCursorMoveEvent *cursorMoveEvent = (CguiMouseCursorMoveEvent *) event;

            if (cursorMoveEvent->moveIn)
            {
                iData->hovered = true;
            }
            if (cursorMoveEvent->moveOut)
            {
                iData->hovered = false;
            }

            return true;
        }

        case CGUI_EVENT_TYPE_MOUSE_BUTTON_PRESS: {
            CguiMouseButtonPressEvent *buttonPressEvent = (CguiMouseButtonPressEvent *) event;

            if (buttonPressEvent->button == iData->mouseButton)
            {
                iData->held = true;
            }

            return true;
        }

        case CGUI_EVENT_TYPE_MOUSE_BUTTON_RELEASE: {
            CguiMouseButtonReleaseEvent *buttonReleaseEvent = (CguiMouseButtonReleaseEvent *) event;

            if (buttonReleaseEvent->button == iData->mouseButton)
            {
                iData->held = false;
                if (iData->hovered)
                {
                    iData->active = true;
                    if (iData->pressCallback) iData->pressCallback(node);
                }
            }

            return true;
        }
    }

    return true;
}

CguiNode *CguiCreateToggle(CguiTransformation transformation, bool active, CguiTogglePressCallback pressCallback, bool disabled)
{
    CguiNode *node = CguiCreateInstance(cguiComponentTemplates[CGUI_COMPONENT_TOGGLE]);
    if (!node)
    {
        return NULL;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_TOGGLE || !node->data || !node->instanceData)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    if (node->childrenCount == 0)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    CguiToggleData         *data       = node->data;
    CguiToggleInstanceData *iData      = node->instanceData;
    CguiNode               *boxNodeRef = node->children[CGUI_TOGGLE_COMPOSITION_BOX_NODE];
    if (boxNodeRef->type != CGUI_ELEMENT_NODE_TYPE_BOX || !boxNodeRef->data)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    CguiTextElementData *boxNodeData = boxNodeRef->data;

    iData->transitionChain = CguiCreateTransitionChain();

    if (!iData->transitionChain)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    CguiTransition *transition = CguiTransitBoxElementData(&iData->currentBoxData, &iData->targetBoxData, &iData->transitioningBoxData, data->transitionEasing, data->transitionDuration);
    if (!transition)
    {
        CguiDeleteTransitionChain(iData->transitionChain);
        CguiDeleteNode(node);
        return NULL;
    }

    if (!CguiInsertTransition(iData->transitionChain, transition))
    {
        CguiDeleteTransition(transition);
        CguiDeleteTransitionChain(iData->transitionChain);
        CguiDeleteNode(node);
        return NULL;
    }

    node->override             = CguiOverrideToggle;
    node->updatePre            = CguiPreUpdateToggle;
    node->canHandleMouseEvents = true;
    node->handleEvent          = CguiHandleToggleEvents;

    iData->mouseToggle   = MOUSE_BUTTON_LEFT;
    iData->pressCallback = pressCallback;
    iData->disabled      = disabled;
    iData->active        = active;

    CguiOverrideTransformation(node, &iData->overrides, transformation);

    return node;
}

void CguiPreUpdateToggle(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_TOGGLE || !node->data || !node->instanceData)
    {
        return;
    }

    if (node->childrenCount == 0)
    {
        return;
    }

    CguiToggleData         *data       = node->data;
    CguiToggleInstanceData *iData      = node->instanceData;
    CguiNode               *boxNodeRef = node->children[CGUI_TOGGLE_COMPOSITION_BOX_NODE];
    if (boxNodeRef->type != CGUI_ELEMENT_NODE_TYPE_BOX || !boxNodeRef->data)
    {
        return;
    }

    CguiBoxElementData *boxNodeData = boxNodeRef->data;

    CguiBoxElementData boxData = data->boxData;

    if (iData->active)
    {
        boxData = data->activeBoxData;

        if (iData->hovered)
        {
            boxData = data->activeHoveredBoxData;
        }

        if (iData->held)
        {
            boxData = data->activeHeldBoxData;
        }

        if (iData->disabled)
        {
            boxData = data->activeDisabledBoxData;
        }
    }
    else
    {
        boxData = data->boxData;

        if (iData->hovered)
        {
            boxData = data->hoveredBoxData;
        }

        if (iData->held)
        {
            boxData = data->heldBoxData;
        }

        if (iData->disabled)
        {
            boxData = data->disabledBoxData;
        }
    }

    if (!CguiIsBoxElementDataEqual(iData->targetBoxData, boxData))
    {
        iData->targetBoxData  = boxData;
        iData->currentBoxData = iData->transitioningBoxData;

        CguiTransition *transition         = iData->transitionChain->first;
        iData->transitionChain->active     = transition;
        iData->transitionChain->activeTime = 0.0f;
        iData->transitionChain->finished   = false;
    }

    CguiUpdateTransitionChain(iData->transitionChain);

    *boxNodeData = iData->transitioningBoxData;
}

void CguiOverrideToggle(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_TOGGLE || !node->data || !node->instanceData)
    {
        return;
    }

    if (node->childrenCount == 0)
    {
        return;
    }

    CguiToggleData         *data       = node->data;
    CguiToggleInstanceData *iData      = node->instanceData;
    CguiNode               *boxNodeRef = node->children[CGUI_TOGGLE_COMPOSITION_BOX_NODE];
    if (boxNodeRef->type != CGUI_ELEMENT_NODE_TYPE_BOX || !boxNodeRef->data)
    {
        return;
    }

    CguiBoxElementData *boxNodeData = boxNodeRef->data;

    node->updatePre            = CguiPreUpdateToggle;
    node->canHandleMouseEvents = true;
    node->handleEvent          = CguiHandleToggleEvents;

    CguiApplyOverrides(node, iData->overrides);
}

bool CguiHandleToggleEvents(CguiNode *node, CguiEvent *event)
{
    if (!node || !event)
    {
        return false;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_TOGGLE || !node->data || !node->instanceData)
    {
        return false;
    }

    if (node->childrenCount == 0)
    {
        return false;
    }

    CguiToggleData         *data       = node->data;
    CguiToggleInstanceData *iData      = node->instanceData;
    CguiNode               *boxNodeRef = node->children[CGUI_TOGGLE_COMPOSITION_BOX_NODE];
    if (boxNodeRef->type != CGUI_ELEMENT_NODE_TYPE_BOX || !boxNodeRef->data)
    {
        return false;
    }

    CguiBoxElementData *boxNodeData = boxNodeRef->data;

    if (iData->disabled)
    {
        return false;
    }

    switch (event->eventType)
    {
        case CGUI_EVENT_TYPE_MOUSE_CURSOR_MOVE: {
            CguiMouseCursorMoveEvent *cursorMoveEvent = (CguiMouseCursorMoveEvent *) event;

            if (cursorMoveEvent->moveIn)
            {
                iData->hovered = true;
            }
            if (cursorMoveEvent->moveOut)
            {
                iData->hovered = false;
            }

            return true;
        }

        case CGUI_EVENT_TYPE_MOUSE_BUTTON_PRESS: {
            CguiMouseButtonPressEvent *buttonPressEvent = (CguiMouseButtonPressEvent *) event;

            if (buttonPressEvent->button == iData->mouseToggle)
            {
                iData->held = true;
            }

            return true;
        }

        case CGUI_EVENT_TYPE_MOUSE_BUTTON_RELEASE: {
            CguiMouseButtonReleaseEvent *buttonReleaseEvent = (CguiMouseButtonReleaseEvent *) event;

            if (buttonReleaseEvent->button == iData->mouseToggle)
            {
                iData->held = false;
                if (iData->hovered)
                {
                    iData->active = !iData->active;
                    if (iData->pressCallback) iData->pressCallback(node, iData->active);
                }
            }

            return true;
        }
    }

    return true;
}
