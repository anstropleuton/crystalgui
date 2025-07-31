/// @file
///
/// @author    Anstro Pleuton
/// @copyright Copyright (c) 2025 Anstro Pleuton
///
/// Crystal GUI - A GUI framework for raylib.
///
/// This source file contains implementations for event management.
///
/// This project is licensed under the terms of MIT license.

#include <string.h>

#include "crystalgui/crystalgui.h"
#include "raylib.h"
#include "raymath.h"

extern CguiNode *cguiMouseButtonPressedNode;

void CguiDispatchEvents(CguiNode *root)
{
    Vector2   mousePosition = GetMousePosition();
    Vector2   mouseDelta    = GetMouseDelta();
    CguiNode *cursorHitNode = CguiCheckCollision(root, mousePosition);

    while (cursorHitNode)
    {
        if (cursorHitNode->canHandleMouseEvents && cursorHitNode->handleEvent)
        {
            break;
        }
        cursorHitNode = cursorHitNode->parent;
    }

    if (!Vector2Equals(mouseDelta, Vector2Zero()))
    {
        Vector2   prevMousePosition = Vector2Subtract(mousePosition, mouseDelta);
        CguiNode *prevCursorHitNode = CguiCheckCollision(root, prevMousePosition);

        while (prevCursorHitNode)
        {
            if (prevCursorHitNode->canHandleMouseEvents && prevCursorHitNode->handleEvent)
            {
                break;
            }
            prevCursorHitNode = prevCursorHitNode->parent;
        }

        if (cursorHitNode == prevCursorHitNode)
        {
            CguiMouseCursorMoveEvent event = {
                .eventType = CGUI_EVENT_TYPE_MOUSE_CURSOR_MOVE,
                .delta     = mouseDelta,
                .moveIn    = false,
                .moveOut   = false
            };

            while (cursorHitNode)
            {
                if (cursorHitNode->canHandleMouseEvents && cursorHitNode->handleEvent && cursorHitNode->handleEvent(cursorHitNode, (CguiEvent *) &event))
                {
                    break;
                }
                cursorHitNode = cursorHitNode->parent;
            }
        }
        else
        {
            CguiMouseCursorMoveEvent eventIn = {
                .eventType = CGUI_EVENT_TYPE_MOUSE_CURSOR_MOVE,
                .delta     = mouseDelta,
                .moveIn    = true,
                .moveOut   = false
            };

            CguiMouseCursorMoveEvent eventOut = {
                .eventType = CGUI_EVENT_TYPE_MOUSE_CURSOR_MOVE,
                .delta     = mouseDelta,
                .moveIn    = false,
                .moveOut   = true
            };

            while (cursorHitNode)
            {
                if (cursorHitNode->canHandleMouseEvents && cursorHitNode->handleEvent && cursorHitNode->handleEvent(cursorHitNode, (CguiEvent *) &eventIn))
                {
                    break;
                }
                cursorHitNode = cursorHitNode->parent;
            }

            while (prevCursorHitNode)
            {
                if (prevCursorHitNode->canHandleMouseEvents && prevCursorHitNode->handleEvent && prevCursorHitNode->handleEvent(prevCursorHitNode, (CguiEvent *) &eventOut))
                {
                    break;
                }
                prevCursorHitNode = prevCursorHitNode->parent;
            }
        }
    }

    for (int mouseButton = 0; mouseButton <= MOUSE_BUTTON_BACK; mouseButton++)
    {
        if (IsMouseButtonPressed(mouseButton))
        {
            CguiMouseButtonPressEvent event = {
                .eventType = CGUI_EVENT_TYPE_MOUSE_BUTTON_PRESS,
                .button    = mouseButton
            };

            while (cursorHitNode)
            {
                if (cursorHitNode->canHandleMouseEvents && cursorHitNode->handleEvent && cursorHitNode->handleEvent(cursorHitNode, (CguiEvent *) &event))
                {
                    cguiMouseButtonPressedNode = cursorHitNode;
                    break;
                }
                cursorHitNode = cursorHitNode->parent;
            }
        }
        if (IsMouseButtonReleased(mouseButton) && cguiMouseButtonPressedNode)
        {
            CguiMouseButtonPressEvent event = {
                .eventType = CGUI_EVENT_TYPE_MOUSE_BUTTON_RELEASE,
                .button    = mouseButton
            };

            cguiMouseButtonPressedNode->handleEvent(cguiMouseButtonPressedNode, (CguiEvent *) &event);
            cguiMouseButtonPressedNode = NULL;
        }
    }
}
