/// @file
///
/// @author    Anstro Pleuton
/// @copyright Copyright (c) 2025 Anstro Pleuton
///
/// Crystal GUI - A GUI framework for raylib.
///
/// This project is licensed under the terms of MIT license.

#ifndef CRYSTALGUI_H
#define CRYSTALGUI_H

#include "raylib.h"

#ifndef CGAPI
#define CGAPI RLAPI
#endif

#ifndef CG_MALLOC
#define CG_MALLOC(sz) MemAlloc(sz)
#endif

#ifndef CG_MALLOC_NULL
#define CG_MALLOC_NULL(sz) CG_MALLOC(sz) // raylib's MemAlloc already zers bytes to zero
#endif

#ifndef CG_REALLOC
#define CG_REALLOC(ptr, sz) MemRealloc(ptr, sz)
#endif

#ifndef CG_FREE
#define CG_FREE(ptr) MemFree(ptr)
#endif

// Free memory pointed by pointer and also set pointer to NULL
#ifndef CG_FREE_NULL
#define CG_FREE_NULL(ptr) \
    do                    \
    {                     \
        CG_FREE(ptr);     \
        ptr = NULL;       \
    }                     \
    while (false)
#endif

#ifndef CG_LOG
#define CG_LOG(level, ...) TraceLog(level, "Crystal GUI: " __VA_ARGS__)
#endif

#ifndef CG_LOG_TRACE
#define CG_LOG_TRACE(...) CG_LOG(LOG_TRACE, __VA_ARGS__)
#endif
#ifndef CG_LOG_DEBUG
#define CG_LOG_DEBUG(...) CG_LOG(LOG_DEBUG, __VA_ARGS__)
#endif
#ifndef CG_LOG_INFO
#define CG_LOG_INFO(...) CG_LOG(LOG_INFO, __VA_ARGS__)
#endif
#ifndef CG_LOG_WARNING
#define CG_LOG_WARNING(...) CG_LOG(LOG_WARNING, __VA_ARGS__)
#endif
#ifndef CG_LOG_ERROR
#define CG_LOG_ERROR(...) CG_LOG(LOG_ERROR, __VA_ARGS__)
#endif
#ifndef CG_LOG_FATAL
#define CG_LOG_FATAL(...) CG_LOG(LOG_FATAL, __VA_ARGS__)
#endif

#ifndef CG_NO_MACRO_DSL // Disable DSL-like macros

#define CG_NODE(node, ...) CguiInsertChildren(node, __VA_ARGS__, NULL)
#define CG_EMPTY(transformation, ...) CguiInsertChildren(CguiCreateNodeEx(transformation, NULL), __VA_ARGS__, NULL)

#define CG_CLAMP(transformation, ...) CG_NODE(CguiCreateClampLayout((transformation)), __VA_ARGS__)
#define CG_CLAMP_ITEM(preserveAspectRatio, aspectRatio, clampFill, minSize, maxSize, ...) CG_NODE(CguiCreateClampLayoutItem((preserveAspectRatio), (aspectRatio), (clampFill), (minSize), (maxSize)), __VA_ARGS__)
#define CG_LINEAR(transformation, direction, justify, spacing, ...) CG_NODE(CguiCreateLinearLayout((transformation), (direction), (justify), (spacing)), __VA_ARGS__)
#define CG_LINEAR_X_BEGIN(transformation, spacing, ...) CG_NODE(CguiCreateLinearLayout((transformation), CGUI_LAYOUT_DIRECTION_X, CGUI_LAYOUT_JUSTIFY_BEGIN, (spacing)), __VA_ARGS__)
#define CG_LINEAR_X_CENTER(transformation, spacing, ...) CG_NODE(CguiCreateLinearLayout((transformation), CGUI_LAYOUT_DIRECTION_X, CGUI_LAYOUT_JUSTIFY_CENTER, (spacing)), __VA_ARGS__)
#define CG_LINEAR_X_END(transformation, spacing, ...) CG_NODE(CguiCreateLinearLayout((transformation), CGUI_LAYOUT_DIRECTION_X, CGUI_LAYOUT_JUSTIFY_END, (spacing)), __VA_ARGS__)
#define CG_LINEAR_X_SPACE_BETWEEN(transformation, spacing, ...) CG_NODE(CguiCreateLinearLayout((transformation), CGUI_LAYOUT_DIRECTION_X, CGUI_LAYOUT_JUSTIFY_SPACE_BETWEEN, (spacing)), __VA_ARGS__)
#define CG_LINEAR_Y_BEGIN(transformation, spacing, ...) CG_NODE(CguiCreateLinearLayout((transformation), CGUI_LAYOUT_DIRECTION_Y, CGUI_LAYOUT_JUSTIFY_BEGIN, (spacing)), __VA_ARGS__)
#define CG_LINEAR_Y_CENTER(transformation, spacing, ...) CG_NODE(CguiCreateLinearLayout((transformation), CGUI_LAYOUT_DIRECTION_Y, CGUI_LAYOUT_JUSTIFY_CENTER, (spacing)), __VA_ARGS__)
#define CG_LINEAR_Y_END(transformation, spacing, ...) CG_NODE(CguiCreateLinearLayout((transformation), CGUI_LAYOUT_DIRECTION_Y, CGUI_LAYOUT_JUSTIFY_END, (spacing)), __VA_ARGS__)
#define CG_LINEAR_Y_SPACE_BETWEEN(transformation, spacing, ...) CG_NODE(CguiCreateLinearLayout((transformation), CGUI_LAYOUT_DIRECTION_Y, CGUI_LAYOUT_JUSTIFY_SPACE_BETWEEN, (spacing)), __VA_ARGS__)
#define CG_LINEAR_ITEM(weight, minSize, maxSize, ...) CG_NODE(CguiCreateLinearLayoutItem((weight), (minSize), (maxSize)), __VA_ARGS__)
#define CG_GRID(transformation, xSlotsCount, ySlotsCount, xJustify, yJustify, spacing, ...) CG_NODE(CguiCreateGridLayout((transformation), (xSlotsCount), (ySlotsCount), (xJustify), (yJustify), (spacing)), __VA_ARGS__)
#define CG_GRID_ITEM(xSlot, ySlot, xSpan, ySpan, ...) CG_NODE(CguiCreateGridLayoutItem((xSlot), (ySlot), (xSpan), (ySpan)), __VA_ARGS__)

#define CG_TEXT(text, color, ...) CG_NODE(CguiCreateTextElement((text), (color)), __VA_ARGS__)
#define CG_TEXT_PRO(text, font, fontSize, spacing, lineSpacing, color, xJustify, yJustify, ...) CG_NODE(CguiCreateTextElementPro(text, (font), (fontSize), (spacing), (lineSpacing), (color), (xJustify), (yJustify)), __VA_ARGS__)

#define CG_TEXTURE(texture, ...) CG_NODE(CguiCreateTextureElement((text), (color)), __VA_ARGS__)
#define CG_TEXTURE_PRO(texture, source, origin, rotation, tint, ...) CG_NODE(CguiCreateTextureElementPro((texture), (source), (origin), (rotation), (tint)), __VA_ARGS__)

#define CG_BOX(radius, color, ...) CG_NODE(CguiCreateBoxElement((radius), (color)), __VA_ARGS__)
#define CG_BOX_EX(radius, color, shadowDistance, shadowColor, borderThickness, borderColor, ...) CG_NODE(CguiCreateBoxElementEx((radius), (color), (shadowDistance), (shadowColor), (borderThickness), (borderColor)), __VA_ARGS__)
#define CG_BOX_PRO(radii, color, texture, shadowDistance, shadowOffset, shadowShrink, shadowColor, shadowTexture, borderThickness, borderColor, borderTexture, ...) CG_NODE(CguiCreateBoxElementPro((radii), (color), (texture), (shadowDistance), (shadowOffset), (shadowShrink), (shadowColor), (shadowTexture), (borderThickness), (borderColor), (borderTexture)), __VA_ARGS__)

#define CG_ROOT(...) CG_NODE(CguiCreateRoot(), __VA_ARGS__)

#define CG_LAYER(transformation, type, ...) CG_NODE(CguiCreateLayer((transformation), (type)), __VA_ARGS__)
#define CG_BACKLAYER(transformation, ...) CG_LAYER((transformation), CGUI_LAYER_TYPE_BACKLAYER, __VA_ARGS__)
#define CG_MIDLAYER(transformation, ...) CG_LAYER((transformation), CGUI_LAYER_TYPE_MIDLAYER, __VA_ARGS__)
#define CG_FRONTLAYER(transformation, ...) CG_LAYER((transformation), CGUI_LAYER_TYPE_FRONTLAYER, __VA_ARGS__)

#define CG_LABEL(transformation, text, type, disabled, xJustify, yJustify, ...) CG_NODE(CguiCreateLabel((transformation), (text), (type), (disabled), (xJustify), (yJustify)), __VA_ARGS__)
#define CG_LABEL_HEADING(transformation, text, disabled, xJustify, yJustify, ...) CG_LABEL((transformation), (text), CGUI_LABEL_TYPE_HEADING, (disabled), (xJustify), (yJustify), __VA_ARGS__)
#define CG_LABEL_BODY(transformation, text, disabled, xJustify, yJustify, ...) CG_LABEL((transformation), (text), CGUI_LABEL_TYPE_BODY, (disabled), (xJustify), (yJustify), __VA_ARGS__)
#define CG_LABEL_CAPTION(transformation, text, disabled, xJustify, yJustify, ...) CG_LABEL((transformation), (text), CGUI_LABEL_TYPE_CAPTION, (disabled), (xJustify), (yJustify), __VA_ARGS__)

#define CG_BUTTON(transformation, type, pressCallback, disabled, ...) CG_NODE(CguiCreateButton((transformation), (type), (pressCallback), (disabled)), __VA_ARGS__)
#define CG_BUTTON_NORMAL(transformation, pressCallback, disabled, ...) CG_BUTTON((transformation), CGUI_BUTTON_TYPE_NORMAL, (pressCallback), (disabled), __VA_ARGS__)
#define CG_BUTTON_FLAT(transformation, pressCallback, disabled, ...) CG_BUTTON((transformation), CGUI_BUTTON_TYPE_FLAT, (pressCallback), (disabled), __VA_ARGS__)
#define CG_BUTTON_ACCENT(transformation, pressCallback, disabled, ...) CG_BUTTON((transformation), CGUI_BUTTON_TYPE_ACCENT, (pressCallback), (disabled), __VA_ARGS__)
#define CG_BUTTON_DANGEROUS(transformation, pressCallback, disabled, ...) CG_BUTTON((transformation), CGUI_BUTTON_TYPE_DANGEROUS, (pressCallback), (disabled), __VA_ARGS__)
#define CG_BUTTON_WARNING(transformation, pressCallback, disabled, ...) CG_BUTTON((transformation), CGUI_BUTTON_TYPE_WARNING, (pressCallback), (disabled), __VA_ARGS__)
#define CG_BUTTON_SUCCESS(transformation, pressCallback, disabled, ...) CG_BUTTON((transformation), CGUI_BUTTON_TYPE_SUCCESS, (pressCallback), (disabled), __VA_ARGS__)
#define CG_BUTTON_INFO(transformation, pressCallback, disabled, ...) CG_BUTTON((transformation), CGUI_BUTTON_TYPE_INFO, (pressCallback), (disabled), __VA_ARGS__)

#define CG_TOGGLE(transformation, active, pressCallback, disabled, ...) CG_NODE(CguiCreateToggle((transformation), (active), (pressCallback), (disabled)), __VA_ARGS__)

#endif // CG_NO_MACRO_DSL

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
// Extra raylib-like Features and Helpers.
//------------------------------------------------------------------------------
//
// These additions either fill functional gap in raylib or simplify common tasks
// as utilities.

// Hotkey

/// Hotkey as an exclusively modifier.
typedef enum CguiHotkey {
    CGUI_HOTKEY_SHIFT   = 1,
    CGUI_HOTKEY_CONTROL = 2,
    CGUI_HOTKEY_ALT     = 4,
    CGUI_HOTKEY_SUPER   = 8,
} CguiHotkey;

CGAPI bool CguiIsKeyRepeated(int key);                  ///< Check if a key is pressed or repeated.
CGAPI bool CguiIsHotkeyDown(int hotkey);                ///< Easily check if a single or multiple hotkeys are pressed exclusively.
CGAPI bool CguiIsKeyComboUp(int hotkey, int key);       ///< Easily check if a hotkey is down && key is up.
CGAPI bool CguiIsKeyComboDown(int hotkey, int key);     ///< Easily check if a hotkey is down && key is down.
CGAPI bool CguiIsKeyComboPressed(int hotkey, int key);  ///< Easily check if a hotkey is down && key is pressed.
CGAPI bool CguiIsKeyComboReleased(int hotkey, int key); ///< Easily check if a hotkey is down && key is released.

// Texts

/// Justify text in the bounds.
typedef enum CguiTextJustify {
    CGUI_TEXT_JUSTIFY_BEGIN,        ///< Align text at the beginning (e.g., left).
    CGUI_TEXT_JUSTIFY_CENTER,       ///< Align text at the center.
    CGUI_TEXT_JUSTIFY_END,          ///< Align text at the end (e.g., right).
    CGUI_TEXT_JUSTIFY_SPACE_BETWEEN ///< Add space between words.
} CguiTextJustify;

CGAPI void CguiDrawTextPro(const char *text, Font font, Rectangle bounds, float fontSize, float spacing, float lineSpacing, Color color, int xJustify, int yJustify); ///< Draw text with word-wrapping.

// Textures

CGAPI void CguiDrawTextureDest(Texture texture, Rectangle dest, Color tint);                          ///< Draw texture with destination rectangle parameter.
CGAPI void CguiDrawTextureFullscreen(Texture texture, Color tint);                                    ///< Draw texture stretched to the size of the screen.
CGAPI void CguiDrawTextureFullscreenEx(Texture texture, Rectangle src, Color tint);                   ///< Draw texture stretched to the size of the screen with extended parameters.
CGAPI void CguiDrawRenderTextureDest(RenderTexture renderTexture, Rectangle dest, Color tint);        ///< Draw render texture with destination rectangle parameter.
CGAPI void CguiDrawRenderTextureFullscreen(RenderTexture renderTexture, Color tint);                  ///< Draw render texture stretched to the size of the screen (NOTE: Flips the render texture's y axis).
CGAPI void CguiDrawRenderTextureFullscreenEx(RenderTexture renderTexture, Rectangle src, Color tint); ///< Draw render texture stretched to the size of the screen with extended parameters (NOTE: Flips the render texture's y axis).

CGAPI Vector2   CguiGetTextureSizeV(Texture texture);                     ///< Return texture's width and height as Vector2.
CGAPI Rectangle CguiGetTextureSizeRec(Texture texture);                   ///< Return texture's width and height as Rectangle (positioned zero).
CGAPI Vector2   CguiGetRenderTextureSizeV(RenderTexture renderTexture);   ///< Return render texture's width and height as Vector2 (NOTE: height is negated).
CGAPI Rectangle CguiGetRenderTextureSizeRec(RenderTexture renderTexture); ///< Return render texture's width and height as Rectangle (NOTE: height is negated, and positioned zero).

// Display sizes

CGAPI Vector2   CguiGetScreenSizeV(void);           ///< Get screen size in Vector2 form.
CGAPI Rectangle CguiGetScreenSizeRec(void);         ///< Get screen size in Rectangle form (positioned zero).
CGAPI Vector2   CguiGetMonitorSizeV(int monitor);   ///< Get monitor size in Vector2 form.
CGAPI Rectangle CguiGetMonitorSizeRec(int monitor); ///< Get monitor size in Rectangle form.
CGAPI int       CguiGetAppWidth(void);              ///< Get current monitor width if app is fullscreen mode, else get screen width.
CGAPI int       CguiGetAppHeight(void);             ///< Get current monitor height if app is fullscreen mode, else get screen height.
CGAPI Vector2   CguiGetAppSizeV(void);              ///< Get current monitor size if app is fullscreen mode, else get screen width and height in Vector2 form.
CGAPI Rectangle CguiGetAppSizeRec(void);            ///< Get current monitor size if app is fullscreen mode, else get screen width and height in Rectangle form.

// Misc.

CGAPI Vector2   CguiRotatePoint(Vector2 point, Vector2 origin, float angle);                  ///< Rotate point along origin with angle (in degrees).
CGAPI Rectangle CguiRecZero(void);                                                            ///< Get an all-zero rectangle.
CGAPI bool      CguiIsRectangleEqual(Rectangle a, Rectangle b);                               ///< Check whether the rectangles are equal.
CGAPI bool      CguiIsColorEqual(Color a, Color b);                                           ///< Check whether the colors are equal.
CGAPI Texture   CguiLoadTextureFromRenderTexture(RenderTexture renderTexture);                ///< Load texture from render texture (NOTE: Flips the render texture's y axis).
CGAPI float     CguiGetMinRoundness(Rectangle rec, float roundness);                          ///< Get the minimum roundness acceptable by the rectangle.
CGAPI Color     CguiColorMultiply(Color color, Vector4 values);                               ///< Multiply color with RGBA values.
CGAPI Color     CguiColorFromHSL(float hue, float saturation, float lightness);               ///< Get color from HSL color model.
CGAPI Color     CguiColorFromHSLA(float hue, float saturation, float lightness, float alpha); ///< Get color from HSL color model with alpha.
CGAPI Color     CguiColorFromVecHSL(Vector3 hsl);                                             ///< Get color from HSL color model (vector version).
CGAPI Color     CguiColorFromVecHSLA(Vector4 hsla);                                           ///< Get color from HSL color model with alpha (vector version).
CGAPI Vector3   CguiColorToHSLA(Color color);                                                 ///< Get HSL color model from color.
CGAPI Vector4   CguiColorToVecHSLA(Color color);                                              ///< Get HSL color model with alpha from color.
CGAPI void      CguiBeginScissorModeRec(Rectangle area);                                      ///< Begin scissor mode (define a screen area for following drawing) using rectangle.
CGAPI Rectangle CguiFlipRectangleX(Rectangle rec, float axis);                                ///< Flip rectangle in X-axis.
CGAPI Rectangle CguiFlipRectangleY(Rectangle rec, float axis);                                ///< Flip rectangle in Y-axis.
CGAPI Rectangle CguiFlipRectangleXY(Rectangle rec, Vector2 axis);                             ///< Flip rectangle in X- and Y-axis.

//------------------------------------------------------------------------------
// Easing Functions
//------------------------------------------------------------------------------
//
// Collection of easing functions for animations and transitions.
//
// Note:
// - The input parameter `t` must be in the range [0..1] as the input is not clamped.
// - The output produced by easing functions may fall outside the [0..1] range (e.g., Elastic easing).
//   It is recommended to design UI hidden areas with extra space (twice the size of visible areas) to accommodate this overshoot.

typedef float (*CguiEasingFunction)(float t); ///< Easing function type.

// Linear easing function
CGAPI float CguiEaseLinear(float t);

// Regular easing functions
CGAPI float CguiEaseIn(float t, float power);
CGAPI float CguiEaseOut(float t, float power);
CGAPI float CguiEaseInOut(float t, float power);

// Quadratic (2) easing functions
CGAPI float CguiEaseInQuad(float t);
CGAPI float CguiEaseOutQuad(float t);
CGAPI float CguiEaseInOutQuad(float t);

// Cubic (3) easing functions
CGAPI float CguiEaseInCubic(float t);
CGAPI float CguiEaseOutCubic(float t);
CGAPI float CguiEaseInOutCubic(float t);

// Quartic (4) easing functions
CGAPI float CguiEaseInQuart(float t);
CGAPI float CguiEaseOutQuart(float t);
CGAPI float CguiEaseInOutQuart(float t);

// Quintic (5) easing functions
CGAPI float CguiEaseInQuint(float t);
CGAPI float CguiEaseOutQuint(float t);
CGAPI float CguiEaseInOutQuint(float t);

// Sextic (6) easing functions
CGAPI float CguiEaseInSextic(float t);
CGAPI float CguiEaseOutSextic(float t);
CGAPI float CguiEaseInOutSextic(float t);

// Septic (7) easing functions
CGAPI float CguiEaseInSeptic(float t);
CGAPI float CguiEaseOutSeptic(float t);
CGAPI float CguiEaseInOutSeptic(float t);

// Octic (8) easing functions
CGAPI float CguiEaseInOctic(float t);
CGAPI float CguiEaseOutOctic(float t);
CGAPI float CguiEaseInOutOctic(float t);

// Nonic (9) easing functions
CGAPI float CguiEaseInNonic(float t);
CGAPI float CguiEaseOutNonic(float t);
CGAPI float CguiEaseInOutNonic(float t);

// Sine easing functions
CGAPI float CguiEaseInSine(float t);
CGAPI float CguiEaseOutSine(float t);
CGAPI float CguiEaseInOutSine(float t);

// Exponential easing functions
CGAPI float CguiEaseInExpo(float t);
CGAPI float CguiEaseOutExpo(float t);
CGAPI float CguiEaseInOutExpo(float t);

CGAPI float CguiEaseInExpoPro(float t, float base, float power);
CGAPI float CguiEaseOutExpoPro(float t, float base, float power);
CGAPI float CguiEaseInOutExpoPro(float t, float base, float power);

// Circular easing functions
CGAPI float CguiEaseInCirc(float t);
CGAPI float CguiEaseOutCirc(float t);
CGAPI float CguiEaseInOutCirc(float t);

// Back easing functions
CGAPI float CguiEaseInBack(float t);
CGAPI float CguiEaseOutBack(float t);
CGAPI float CguiEaseInOutBack(float t);

CGAPI float CguiEaseInBackPro(float t, float overshoot);
CGAPI float CguiEaseOutBackPro(float t, float overshoot);
CGAPI float CguiEaseInOutBackPro(float t, float overshoot);

// Elastic easing functions
CGAPI float CguiEaseInElastic(float t);
CGAPI float CguiEaseOutElastic(float t);
CGAPI float CguiEaseInOutElastic(float t);

CGAPI float CguiEaseInElasticPro(float t, float amplitude, float period, float base, float power);
CGAPI float CguiEaseOutElasticPro(float t, float amplitude, float period, float base, float power);
CGAPI float CguiEaseInOutElasticPro(float t, float amplitude, float period, float base, float power);

// Bounce easing functions
CGAPI float CguiEaseInBounce(float t);
CGAPI float CguiEaseOutBounce(float t);
CGAPI float CguiEaseInOutBounce(float t);

CGAPI float CguiEaseOutBouncePro(float t, float b1, float b2, float b3, float b4, float c1, float c2, float c3, float c4);
CGAPI float CguiEaseInBouncePro(float t, float b1, float b2, float b3, float b4, float c1, float c2, float c3, float c4);
CGAPI float CguiEaseInOutBouncePro(float t, float b1, float b2, float b3, float b4, float c1, float c2, float c3, float c4);

//------------------------------------------------------------------------------
// Events
//------------------------------------------------------------------------------
//
// Mouse Events:
// - Mouse events depend on the node's transformation.
// - To enable mouse event handling, set the node's `canHandleMouseEvents` to true.
// - Mouse events are dispatched to the top-most drawn node (child-most, or last child in case of overlap) under the cursor.
// - Mouse events propagation "bubbles up" from the child-most node to its parents up to the root.
// - A node can consume an event to prevent it from propagating further.
//
// Keyboard Events:
// - Keyboard events depend on the focus state of the node.
// - To enable keyboard event handling, set the node's `canHandleKeyboardEvents` to true.
// - To request focus for keyboard event handling, call `CguiRequestFocus(node)`.
// - Navigation keys do not trigger events. These keys are used to navigate the focus.
// - Navigation keys are:
//   - TAB              -> Moves focus to the next sibling child.
//   - SHIFT+TAB        -> Moves focus to the previous sibling child.
//   - ENTER            -> Moves focus deeper into child nodes.
//   - BACKSPACE        -> Moves focus back to the parent node.
//   - CONTROL+ESCAPE   -> Unfocuses the node (reserved).
// - Nodes may capture navigation keys (e.g., for text editing). Though, the CONTROL+ESCAPE key cannot be overrided.
// - To capture navigation keys as well, set the node's `includeNavKeys` to true.

struct CguiNode;
typedef struct CguiNode CguiNode;

/// Event type for GUI events.
typedef enum CguiEventType {
    CGUI_EVENT_TYPE_MOUSE_CURSOR_MOVE,    ///< Event when mouse cursor moves.
    CGUI_EVENT_TYPE_MOUSE_BUTTON_PRESS,   ///< Event when mouse button is pressed.
    CGUI_EVENT_TYPE_MOUSE_BUTTON_RELEASE, ///< Event when mouse button is released.
    CGUI_EVENT_TYPE_KEYBOARD_KEY_PRESS,   ///< Event when keyboard key is pressed.
    CGUI_EVENT_TYPE_KEYBOARD_KEY_RELEASE, ///< Event when keyboard key is released.
} CguiEventType;

/// GUI event.
/// When not consumed, the events bubble up to the parent.
typedef struct CguiEvent {
    int eventType; ///< Event type.
} CguiEvent;

/// Mouse move event.
typedef struct CguiMouseCursorMoveEvent {
    int     eventType; ///< Event type.
    Vector2 delta;     ///< Mouse movement delta.
    bool    moveIn;    ///< Mouse is moving in to the bounds.
    bool    moveOut;   ///< Mouse is moving out from the bounds.
} CguiMouseCursorMoveEvent;

/// Mouse button press event.
typedef struct CguiMouseButtonPressEvent {
    int eventType; ///< Event type.
    int button;    ///< Button pressed.
} CguiMouseButtonPressEvent;

/// Mouse button release event.
typedef struct CguiMouseButtonReleaseEvent {
    int eventType; ///< Event type.
    int button;    ///< Button released.
} CguiMouseButtonReleaseEvent;

/// Keyboard key press event.
typedef struct CguiKeyboardKeyPressEvent {
    int eventType;  ///< Event type.
    int key;        ///< Key pressed.
    int typingChar; ///< Typing character (for appending key to a string, e.g., '@' when shift + 2 is pressed (depends on keyboard layout)).
} CguiKeyboardKeyPressEvent;

/// Keyboard key release event.
typedef struct CguiKeyboardKeyReleaseEvent {
    int eventType; ///< Event type.
    int key;       ///< Key released.
} CguiKeyboardKeyReleaseEvent;

CGAPI void CguiDispatchEvents(CguiNode *root);

//------------------------------------------------------------------------------
// Node (GUI Scene Graph)
//------------------------------------------------------------------------------
//
// A node represents a single entity in the GUI scene graph (tree).
// The scene graph maintains the hierarchy and relationships between GUI entities.
//
// Each node contains:
// - Transformation: Position, size, and transformation relative to its parent.
// - Properties: Name, Type, User Data, etc.
// - Handlers: Functions for transforming, updating, drawing, debugging and deleting node or children.
// - Hierarchy: Zero or more child nodes, owned by this node, and a reference to the parent node (NULL if root).
// - Templateing: Create an instance off of a node that will be synced when needed (a parallel, independent tree within the hierarchy).
//
// Hierarchy Warnings:
// - A node fully owns all the children.
// - A node cannot be a child of itself, either directly or indirectly, to avoid infinite recursion.
//   This rule is not enforced for performance reasons, so care is required.
// - A node cannot contain the same child pointer multiple times to avoid double-free.
//   This rule is enforced during insertion/removal operations.
//
// Template Graph Warnings:
// - A template node does not own any instances of it.
// - A template node cannot be an instance of itself, either directly or indirectly, to avoid infinite recursion.
//   This rule is not enforced for performance reasons, so care is required here as well.
//
// Notes on Handlers:
// - Node provides hooks for handling node updates and draws, called in pre-order (parent before children) or post-order (children before parent).
// - Prefer using pre-order functions for both updating and drawing operations unless you have an explicit reason otherwise.
// - Transform updates and debug drawing are pre-order functions.
//
// Memory freeing:
// - Allocated memory is freed automatically. If manual memory deletion is required, set the freed memory pointers to NULL to avoid double free.
// - The delete handler is only responsible for freeing any allocated data within the user data. If you manually free the data, you must set it to NULL to avoid double free.

/// GUI node transformation.
typedef struct CguiTransformation {
    Vector2 position;           ///< Position of the node.
    Vector2 size;               ///< Size of the node.
    Vector2 isRelativePosition; ///< Whether the position (x, y) is relative to parent (position acts as offset if true) [if non-zero or non-one, results are interpolated].
    Vector2 isRelativeSize;     ///< Whether the size (x, y) is relative to parent (size acts as scale if true) [if non-zero or non-one, results are interpolated].
    Vector2 anchor;             ///< Anchoring when position is relative to parent (0, 0 -> top left, 1, 1 -> bottom right, 0.5, 0.5 -> center, etc.).
    Vector2 shrink;             ///< Shrinking when size is relative to parent (0, 0 -> no shrinking, 20, 10 -> shrink node 20, 10 units smaller than parent, etc.).
} CguiTransformation;

typedef void (*CguiNodeFunction)(CguiNode *node);                          ///< General function to perform action to the node.
typedef bool (*CguiTransformNodeFunction)(CguiNode *node);                 ///< Transform function to update the node's transformation. Return true if transform changed.
typedef bool (*CguiHandleEventFunction)(CguiNode *node, CguiEvent *event); ///< Handle event. Return true if consumed.

/// GUI node for nesting.
struct CguiNode {
    bool  enabled;  ///< Toggle the entire node (updates, renders, event handling, etc.).
    char *name;     ///< Name of the node. Names starting with "Cgui" are reserved (auto-freed).
    int   type;     ///< Type of the node (for polymorphism). Numbers in form of 0x00FFxxxx are reserved.
    void *data;     ///< Node data (auto-freed).
    int   dataSize; ///< Number of bytes of data.

    CguiTransformation transformation; ///< Transformation of the node. Apply for recache when modifying.
    Rectangle          bounds;         ///< Calculated bounds of the node.
    bool               rebound;        ///< Whether to recalculate bounds. Another way to trigger rebound is to return true from the attached transform function.

    CguiNode  *parent;           ///< Reference to parent node.
    CguiNode **children;         ///< Children nodes (node owns its children).
    int        childrenCount;    ///< Number of children nodes.
    int        childrenCapacity; ///< Number of children that can be inserted before reallocation.

    // Instance node of a template node will copy all properties excluding the below
    // Reoverriding the fields is up to you using the override field

    CguiNode        *templateSource;    ///< Reference to template source node.
    CguiNode       **instances;         ///< References to instance nodes (node does not owns references).
    int              instancesCount;    ///< Number of instance nodes.
    int              instancesCapacity; ///< Number of instance that can be inserted before reallocation.
    bool             resync;            ///< Whether to sync instances from this node.
    void            *instanceData;      ///< Data specific to instance.
    int              instanceDataSize;  ///< Number of bytes of instance data.
    CguiNodeFunction override;          ///< Override function to override fields from instance data after template synced and replaced this node's data.

    // It is recommended to almost always use pre-order functions rather than post-order functions

    CguiTransformNodeFunction transform;      ///< Transform function.
    CguiNodeFunction          updatePre;      ///< Update function (called before all children).
    CguiNodeFunction          updatePost;     ///< Update function (called after all children).
    CguiNodeFunction          drawPre;        ///< Draw function (called before all children).
    CguiNodeFunction          drawPost;       ///< Draw function (called after all children).
    CguiNodeFunction          debugDraw;      ///< Debug-draw function.
    CguiNodeFunction          deleteNodeData; ///< Delete function.

    bool                    canHandleMouseEvents;    ///< Handle mouse events.
    bool                    canHandleKeyboardEvents; ///< Handle keyboard events.
    bool                    includeNavKeys;          ///< Include navigation keys in the keyboard event.
    CguiHandleEventFunction handleEvent;             ///< Event handler function.
};

// Node management

CGAPI CguiNode *CguiCreateNode(void);                                                                                                                                                ///< Create a new node.
CGAPI CguiNode *CguiCreateNodeEx(CguiTransformation transformation, const char *name);                                                                                               ///< Create a new named node.
CGAPI CguiNode *CguiCreateNodePro(CguiTransformation transformation, const char *name, int type, const void *data, int dataSize);                                                    ///< Create a new named node with type and optional data.
CGAPI CguiNode *CguiCreateNodeProMax(CguiTransformation transformation, const char *name, int type, const void *data, int dataSize, const void *instanceData, int instanceDataSize); ///< Create a new named node with type and optional data and instance data.
CGAPI void      CguiDeleteNode(CguiNode *node);                                                                                                                                      ///< Delete a node (this will deallocate children).
CGAPI void      CguiDeleteNodeSelf(CguiNode *node);                                                                                                                                  ///< Delete a node (this does not deallocate children).
CGAPI bool      CguiRenameNode(CguiNode *node, const char *newName);                                                                                                                 ///< Rename a node.

CGAPI void CguiTransformNode(CguiNode *node, bool rebound);     ///< Transform a node recursively (parent first).
CGAPI bool CguiTransformNodeSelf(CguiNode *node, bool rebound); ///< Transform a node itself (non-recursively).
CGAPI void CguiUpdateNode(CguiNode *node);                      ///< Update a node recursively (parent first).
CGAPI void CguiUpdatePreNodeSelf(CguiNode *node);               ///< Pre-update a node itself (non-recursively).
CGAPI void CguiUpdatePostNodeSelf(CguiNode *node);              ///< Post-update a node itself (non-recursively).
CGAPI void CguiDrawNode(CguiNode *node);                        ///< Draw a node recursively (parent first).
CGAPI void CguiDrawPreNodeSelf(CguiNode *node);                 ///< Pre-draw a node itself (non-recursively).
CGAPI void CguiDrawPostNodeSelf(CguiNode *node);                ///< Post-draw a node itself (non-recursively).
CGAPI void CguiDebugDrawNode(CguiNode *node);                   ///< Debug-draw a node recursively (parent first).
CGAPI void CguiDebugDrawNodeSelf(CguiNode *node);               ///< Debug-draw a node itself (non-recursively).

CGAPI CguiNode *CguiCloneNode(CguiNode *node);     ///< Duplicate a node and its children.
CGAPI CguiNode *CguiCloneNodeSelf(CguiNode *node); ///< Duplicate a node without its children.

// Templating

CGAPI CguiNode *CguiCreateInstance(CguiNode *templateNode);                          ///< Create an instance from a template node and its children.
CGAPI CguiNode *CguiCreateInstanceSelf(CguiNode *templateNode);                      ///< Create an instance from a template node without its children.
CGAPI void      CguiApplyTemplateResync(CguiNode *node);                             ///< Set the template and children (recursively) to resync.
CGAPI bool      CguiLinkTemplate(CguiNode *node, CguiNode *templateNode);            ///< Link this node to be updated with the template.
CGAPI bool      CguiUnlinkTemplate(CguiNode *node);                                  ///< Unlink this node from being updated with the template.
CGAPI bool      CguiSetInstancesCapacity(CguiNode *node, int newCapacity);           ///< Set instances capacity (if capacity >= count).
CGAPI int       CguiFindInstanceIndex(CguiNode *templateSource, CguiNode *instance); ///< Find direct instance in source and return an index, -1 if not found.
CGAPI void      CguiSyncInstances(CguiNode *node, bool resync);                      ///< Syncs this node if it has attached template source and traverse instances recursively (parent first).
CGAPI bool      CguiSyncInstancesSelf(CguiNode *node, bool resync);                  ///< Syncs this node itself if it has attached template source (non-recursively).
CGAPI void      CguiSyncHierarchy(CguiNode *node);                                   ///< Sync all instances as well as all children's instances.

// Node children management

CGAPI bool      CguiInsertChild(CguiNode *parent, CguiNode *child);                                                ///< Insert a child to the parent at end, returns true if inserted.
CGAPI bool      CguiInsertChildAt(CguiNode *parent, CguiNode *child, int childIndex);                              ///< Insert a child to the parent at index, returns true if inserted.
CGAPI bool      CguiRemoveChild(CguiNode *parent, CguiNode *child);                                                ///< Remove a child from the parent, returns true if removed (this does not deallocate child).
CGAPI bool      CguiRemoveChildAt(CguiNode *parent, int childIndex);                                               ///< Remove the index'th child from the parent, returns true if removed (this does not deallocate child).
CGAPI CguiNode *CguiCreateChild(CguiNode *parent);                                                                 ///< Create a new child for the parent and insert at end.
CGAPI CguiNode *CguiCreateChildAt(CguiNode *parent, int childIndex);                                               ///< Create a new child for the parent and insert at index.
CGAPI bool      CguiDeleteChild(CguiNode *parent, CguiNode *child);                                                ///< Delete a child from the parent (this will deallocate child).
CGAPI bool      CguiDeleteChildAt(CguiNode *parent, int childIndex);                                               ///< Delete the index'th child from the parent (this will deallocate child).
CGAPI bool      CguiRemoveAllChildren(CguiNode *parent);                                                           ///< Clear all children from a node (this does not deallocate children).
CGAPI bool      CguiDeleteAllChildren(CguiNode *parent);                                                           ///< Clear all children from a node (this will deallocate children).
CGAPI bool      CguiSetChildrenCapacity(CguiNode *node, int newCapacity);                                          ///< Set children capacity (if capacity >= count).
CGAPI bool      CguiReserveChildrenCapacity(CguiNode *node, int capacity);                                         ///< Reserve additional capacity after count for faster insertion.
CGAPI bool      CguiShrinkChildrenCapacity(CguiNode *node);                                                        ///< Shrink capacity to equal the number of children.
CGAPI bool      CguiCloneAllChildren(CguiNode *fromParent, CguiNode *toParent);                                    ///< Clone all children from one parent to another's end.
CGAPI bool      CguiCloneChildrenRange(CguiNode *fromParent, int indexBegin, int indexEnd, CguiNode *toParent);    ///< Clone a range of children from one parent to another's end.
CGAPI bool      CguiTransferChild(CguiNode *fromParent, CguiNode *child, CguiNode *toParent);                      ///< Transfer a child from one parent to another's end.
CGAPI bool      CguiTransferChildAt(CguiNode *fromParent, int childIndex, CguiNode *toParent);                     ///< Transfer the index'th child from one parent to another's end.
CGAPI bool      CguiTransferAllChildren(CguiNode *fromParent, CguiNode *toParent);                                 ///< Transfer all children from one parent to another's end.
CGAPI bool      CguiTransferChildrenRange(CguiNode *fromParent, int indexBegin, int indexEnd, CguiNode *toParent); ///< Transfer a range of children from one parent to another's end.
CGAPI CguiNode *CguiInsertChildren(CguiNode *parent, ...);                                                         ///< Insert a children to the parent at end, returns true if inserted.
CGAPI CguiNode *CguiInsertChildrenAt(CguiNode *parent, int childIndex, ...);                                       ///< Insert a children to the parent at index, returns true if inserted.
CGAPI CguiNode *CguiInsertChildrenV(CguiNode *parent, va_list args);                                               ///< Insert a children to the parent at end, returns true if inserted.
CGAPI CguiNode *CguiInsertChildrenAtV(CguiNode *parent, int childIndex, va_list args);                             ///< Insert a children to the parent at index, returns true if inserted.

// Transformation helpers

CGAPI CguiTransformation CguiTZeroSize(void);                                                                      ///< Zero-size transformation, relative positioning (offsetting) still maintained.
CGAPI CguiTransformation CguiTAbsolute(Vector2 pos, Vector2 size);                                                 ///< Absolute positioning/sizing transformation.
CGAPI CguiTransformation CguiTOffset(Vector2 offset, Vector2 size);                                                ///< Relative positioning (offsetting) and absolute sizing transformation.
CGAPI CguiTransformation CguiTScale(Vector2 pos, Vector2 scale);                                                   ///< Absolute positioning and relative sizing (scaling) transformation.
CGAPI CguiTransformation CguiTOffsetScale(Vector2 offset, Vector2 scale);                                          ///< Relative positioning (offsetting)/sizing (scaling) transformation.
CGAPI CguiTransformation CguiTCenter(Vector2 size);                                                                ///< Centered in parent transformation.
CGAPI CguiTransformation CguiTTop(Vector2 size);                                                                   ///< Align at top transformation.
CGAPI CguiTransformation CguiTBottom(Vector2 size);                                                                ///< Align at bottom transformation.
CGAPI CguiTransformation CguiTLeft(Vector2 size);                                                                  ///< Align at left transformation.
CGAPI CguiTransformation CguiTRight(Vector2 size);                                                                 ///< Align at right transformation.
CGAPI CguiTransformation CguiTTopLeft(Vector2 size);                                                               ///< Align at top-left transformation.
CGAPI CguiTransformation CguiTTopRight(Vector2 size);                                                              ///< Align at top-right transformation.
CGAPI CguiTransformation CguiTBottomLeft(Vector2 size);                                                            ///< Align at bottom-left transformation.
CGAPI CguiTransformation CguiTBottomRight(Vector2 size);                                                           ///< Align at bottom-right transformation.
CGAPI CguiTransformation CguiTDockTop(float height);                                                               ///< Align at top and fill width transformation.
CGAPI CguiTransformation CguiTDockBottom(float height);                                                            ///< Align at bottom and fill width transformation.
CGAPI CguiTransformation CguiTDockLeft(float width);                                                               ///< Align at left and fill height transformation.
CGAPI CguiTransformation CguiTDockRight(float width);                                                              ///< Align at right and fill height transformation.
CGAPI CguiTransformation CguiTMargin(float margin);                                                                ///< Shrunk in parent transformation.
CGAPI CguiTransformation CguiTMarginPro(float marginTop, float marginBottom, float marginLeft, float marginRight); ///< Shrunk in parent transformation.
CGAPI CguiTransformation CguiTFillParent(void);                                                                    ///< Fill in parent transformation.
CGAPI bool               CguiIsTransformationEqual(CguiTransformation a, CguiTransformation b);                    ///< Check if all field of transformation match.
CGAPI void               CguiSetTransformation(CguiNode *node, CguiTransformation t);                              ///< Set node's transformation and mark it for bound recalculation if the transformation is changed.

// Misc.

CGAPI bool      CguiIsTreeStructureEqual(CguiNode *a, CguiNode *b);           ///< Check if two tree structures are equal (children count).
CGAPI bool      CguiIsTreeTypeEqual(CguiNode *a, CguiNode *b);                ///< Check if two tree structures and types are equal (children count and type info).
CGAPI bool      CguiCopyNodeValues(CguiNode *fromNode, CguiNode *toNode);     ///< Copy all excluding hierarchy fields from one node to another.
CGAPI bool      CguiCopyNodeValuesNoTi(CguiNode *fromNode, CguiNode *toNode); ///< Copy all excluding hierarchy and template/instance fields from one node to another.
CGAPI bool      CguiCopyNode(CguiNode *fromNode, CguiNode *toNode);           ///< Copy all excluding fields from one node to another (recursively, if structure matches).
CGAPI bool      CguiCopyNodeNoTi(CguiNode *fromNode, CguiNode *toNode);       ///< Copy all excluding template/instance fields from one node to another (recursively, if structure matches).
CGAPI int       CguiFindChildIndex(CguiNode *parent, CguiNode *child);        ///< Find the direct child in parent and return an index, -1 if not found.
CGAPI bool      CguiIsDescendantOf(CguiNode *parent, CguiNode *child);        ///< Check if a node is a child of parent (recursively).
CGAPI bool      CguiIsAncestorOf(CguiNode *child, CguiNode *parent);          ///< Check if a node is a parent of child (recursively).
CGAPI CguiNode *CguiFindTypeInChildren(CguiNode *parent, int type);           ///< Returns the first found node type in itself or children (recursively), NULL if not found.
CGAPI CguiNode *CguiFindTypeInParents(CguiNode *child, int type);             ///< Returns the first found node type in itself or parents (recursively), NULL if not found.
CGAPI Rectangle CguiComputeNodeBounds(CguiNode *node);                        ///< Compute node bounds recursively for recache applied nodes.
CGAPI CguiNode *CguiCheckCollision(CguiNode *node, Vector2 point);            ///< Check for collision with the bounds of to top-most drawn node (child-most, or last child in case of overlap) under the point, returns NULL if none.

//------------------------------------------------------------------------------
// Layout Nodes
//------------------------------------------------------------------------------
//
// Layout nodes are special nodes that automatically arranges their children
// according to a specific layout arrangement.
//
// Each layout node requires its child node to be of a matching item type, e.g.:
// - Clamp layout   -> Clamp items.
// - Linear layout  -> Linear items.
// - Grid layout    -> Grid items.
// - etc. (more to be added)
// Children with invalid matching type will be ignored during processing.
//
// Note: The transformation of the items will be overridden automatically.
// For manual positioning requirements, consider nesting a node within an item.

/// Layout node type, 0x00FF01 is the type prefix.
typedef enum CguiLayoutNodeType {
    CGUI_LAYOUT_NODE_TYPE_CLAMP = 0x00FF0100, ///< Clamp node type.
    CGUI_LAYOUT_NODE_TYPE_CLAMP_ITEM,         ///< Clamp node's item type.
    CGUI_LAYOUT_NODE_TYPE_LINEAR,             ///< Linear node type.
    CGUI_LAYOUT_NODE_TYPE_LINEAR_ITEM,        ///< Linear node's item type.
    CGUI_LAYOUT_NODE_TYPE_GRID,               ///< Grid node type.
    CGUI_LAYOUT_NODE_TYPE_GRID_ITEM,          ///< Grid node's item type.
} CguiLayoutNodeType;

/// Layout direction.
typedef enum CguiLayoutDirection {
    CGUI_LAYOUT_DIRECTION_X, ///< X-axis direction (column direction).
    CGUI_LAYOUT_DIRECTION_Y, ///< Y-axis direction (row direction).

    // Aliases

    CGUI_LAYOUT_DIRECTION_HORIZONTAL = CGUI_LAYOUT_DIRECTION_X,
    CGUI_LAYOUT_DIRECTION_VERTICAL   = CGUI_LAYOUT_DIRECTION_Y,
    CGUI_LAYOUT_DIRECTION_COLUMN     = CGUI_LAYOUT_DIRECTION_X,
    CGUI_LAYOUT_DIRECTION_ROW        = CGUI_LAYOUT_DIRECTION_Y
} CguiLayoutDirection;

/// Layout justification.
typedef enum CguiLayoutJustify {
    CGUI_LAYOUT_JUSTIFY_BEGIN,         ///< Justify content at the beginning.
    CGUI_LAYOUT_JUSTIFY_CENTER,        ///< Justify content at the center.
    CGUI_LAYOUT_JUSTIFY_END,           ///< Justify content at the ending.
    CGUI_LAYOUT_JUSTIFY_SPACE_BETWEEN, ///< Add equal spacing between items.
} CguiLayoutJustify;

/// Clamp layout data.
/// Items are independent.
typedef struct CguiClampLayoutData {
    // ... nothing in here ...
    int empty; ///< Prevent zero sizeof.
} CguiClampLayoutData;

CGAPI CguiNode *CguiCreateClampLayout(CguiTransformation transformation); ///< Helper to create a clamp layout node.
CGAPI bool      CguiTransformClampLayout(CguiNode *node);                 ///< Transform function (attached) for clamp layout node.

/// Clamp layout item data.
/// Control the clamp item.
typedef struct CguiClampLayoutItemData {
    bool    preserveAspectRatio; ///< Whether the preserve the aspect ratio of the item.
    float   aspectRatio;         ///< Aspect ratio of the item (width / height).
    bool    clampFill;           ///< Whether to clamp as fill rather than fit (false -> clamp fit, true -> clamp fill).
    Vector2 minSize;             ///< Minimum size of the item in the layout.
    Vector2 maxSize;             ///< Maximum size of the item in the layout.
} CguiClampLayoutItemData;

CGAPI CguiNode *CguiCreateClampLayoutItem(bool preserveAspectRatio, float aspectRatio, bool clampFill, Vector2 minSize, Vector2 maxSize); ///< Helper to create a clamp layout's item node.

/// Linear layout data.
/// Items are arranged linearly in a direction.
typedef struct CguiLinearLayoutData {
    int   direction; ///< Layout direction for items.
    int   justify;   ///< Layout justification if space is available (or exceeded).
    float spacing;   ///< Spacing between items of the layout.
} CguiLinearLayoutData;

CGAPI CguiNode *CguiCreateLinearLayout(CguiTransformation transformation, int direction, int justify, float spacing); ///< Helper to create a linear layout node.
CGAPI bool      CguiTransformLinearLayout(CguiNode *node);                                                            ///< Transform function (attached) for linear layout node.

/// Linear layout item data.
/// Control the linear item.
typedef struct CguiLinearLayoutItemData {
    float weight;  ///< Weight of the item in the layout.
    float minSize; ///< Minimum size of the item in the layout.
    float maxSize; ///< Maximum size of the item in the layout.

    float position; ///< Internal: Position of the item bounds.
    float size;     ///< Internal: Size of the item bounds.
} CguiLinearLayoutItemData;

CGAPI CguiNode *CguiCreateLinearLayoutItem(float weight, float minSize, float maxSize); ///< Helper to create a linear layout's item node.

/// Grid slot data.
typedef struct CguiGridLayoutSlotData {
    float weight;  ///< Weight of the slot in the layout.
    float minSize; ///< Minimum size of the slot in the layout.
    float maxSize; ///< Maximum size of the slot in the layout.

    float position; ///< Internal: Position of the slot bounds.
    float size;     ///< Internal: Size of the slot bounds.
} CguiGridLayoutSlotData;

/// Grid layout data.
/// Arrange items in a grid.
typedef struct CguiGridLayoutData {
    CguiGridLayoutSlotData *xSlots;         ///< Slots in the x-axis.
    int                     xSlotsCount;    ///< Number of slots in the x-axis.
    int                     xSlotsCapacity; ///< Number of slots that can be inserted before reallocation.
    CguiGridLayoutSlotData *ySlots;         ///< Slots in the y-axis.
    int                     ySlotsCount;    ///< Number of slots in the y-axis.
    int                     ySlotsCapacity; ///< Number of slots that can be inserted before reallocation.
    int                     xJustify;       ///< Layout justification if space in x-axis is available (or exceeded).
    int                     yJustify;       ///< Layout justification if space in y-axis is available (or exceeded).
    Vector2                 spacing;        ///< Spacing between slots of the grid.
} CguiGridLayoutData;

CGAPI CguiNode *CguiCreateGridLayout(CguiTransformation transformation, int xSlotsCount, int ySlotsCount, int xJustify, int yJustify, Vector2 spacing); ///< Helper to create a grid layout node.
CGAPI void      CguiDeleteGridLayoutData(CguiNode *node);                                                                                               ///< Delete function (attached) for grid layout node.
CGAPI bool      CguiTransformGridLayout(CguiNode *node);                                                                                                ///< Transform function (attached) for grid layout node.

// Grid layout helpers

CGAPI bool CguiGridLayoutInsertSlotX(CguiGridLayoutData *layoutData, int index, float weight, float minSize, float maxSize); ///< Insert a slot in the x-axis in the grid layout.
CGAPI bool CguiGridLayoutInsertSlotY(CguiGridLayoutData *layoutData, int index, float weight, float minSize, float maxSize); ///< Insert a slot in the y-axis in the grid layout.
CGAPI bool CguiGridLayoutRemoveSlotX(CguiGridLayoutData *layoutData, int index);                                             ///< Insert a slot in the x-axis in the grid layout.
CGAPI bool CguiGridLayoutRemoveSlotY(CguiGridLayoutData *layoutData, int index);                                             ///< Insert a slot in the y-axis in the grid layout.

/// Grid layout item data.
/// Control the grid item.
/// Invalid slot or span yields zero-ed out transformation.
typedef struct CguiGridLayoutItemData {
    int xSlot; ///< Slot index in x-axis it should start from.
    int ySlot; ///< Slot index in y-axis it should start from.
    int xSpan; ///< Number of spans in x-axis it should stretch.
    int ySpan; ///< Number of spans in y-axis it should stretch.
} CguiGridLayoutItemData;

CGAPI CguiNode *CguiCreateGridLayoutItem(int xSlot, int ySlot, int xSpan, int ySpan); ///< Helper to create a grid layout's item node.

//------------------------------------------------------------------------------
// GUI Basic Element Nodes
//------------------------------------------------------------------------------
//
// These are the basic visual element nodes used in the GUI. These provide
// foundation for the GUI components.

/// Basic element type. 0x00FF08 is the type prefix.
typedef enum CguiElementNodeType {
    CGUI_ELEMENT_NODE_TYPE_TEXT = 0x00FF0800, ///< Text element.
    CGUI_ELEMENT_NODE_TYPE_TEXTURE,           ///< Texture element.
    CGUI_ELEMENT_NODE_TYPE_BOX,               ///< Styled rectangle (Box) element.
    CGUI_ELEMENT_NODE_TYPE_EVENT,             ///< Event handling element.
} CguiElementNodeType;

/// Basic text element data.
typedef struct CguiTextElementData {
    const char *text;        ///< Text to display.
    Font        font;        ///< Font to use.
    float       fontSize;    ///< Size of the text.
    float       spacing;     ///< Spacing between characters.
    float       lineSpacing; ///< Spacing between lines.
    Color       color;       ///< Color of characters (tint).
    int         xJustify;    ///< Justification for text alignment in x-axis.
    int         yJustify;    ///< Justification for text alignment in y-axis.
} CguiTextElementData;

CGAPI CguiNode *CguiCreateTextElement(const char *text, Color color);                                                                                             ///< Helper to create a text element node.
CGAPI CguiNode *CguiCreateTextElementPro(const char *text, Font font, float fontSize, float spacing, float lineSpacing, Color color, int xJustify, int yJustify); ///< Helper to create a text element node.
CGAPI void      CguiDrawPreTextElement(CguiNode *node);                                                                                                           ///< Pre-draw function (attached) for text element node.
CGAPI bool      CguiIsTextElementDataEqual(CguiTextElementData a, CguiTextElementData b);                                                                         ///< Check if text element data is equal.

/// Basic texture element data.
typedef struct CguiTextureElementData {
    Texture   texture;  ///< Texture to draw.
    Rectangle source;   ///< Source rectangle for texture.
    Vector2   origin;   ///< Center of rotation for txture (note: origin is added to destination position).
    float     rotation; ///< Rotation of texture in degrees (note: texture may leak outside the node bounds).
    Color     tint;     ///< Tint color for texture.
} CguiTextureElementData;

CGAPI CguiNode *CguiCreateTextureElement(Texture texture);                                                                  ///< Helper to create a texture element node.
CGAPI CguiNode *CguiCreateTextureElementPro(Texture texture, Rectangle source, Vector2 origin, float rotation, Color tint); ///< Helper to create a texture element node.
CGAPI void      CguiDrawPreTextureElement(CguiNode *node);                                                                  ///< Pre-draw function (attached) for texture element node.
CGAPI bool      CguiIsTextureElementDataEqual(CguiTextureElementData a, CguiTextureElementData b);                          ///< Check if texture element data is equal.

/// Basic box element data.
typedef struct CguiBoxElementData {
    Vector4 radii;   ///< Radius of 4 corners: top-left, top-right, bottom-left and bottom-right.
    Color   color;   ///< Color of the box.
    Texture texture; ///< Add texture to the box, the color will act as a tint.

    float   shadowDistance; ///< Distance of shadow from the box.
    Vector2 shadowOffset;   ///< Position offset of the shadow from the box.
    float   shadowShrink;   ///< Shrinking of the shadow size from the box.
    Color   shadowColor;    ///< Shadow color.
    Texture shadowTexture;  ///< Add texture to the shadow, the color will act as a tint.

    float   borderThickness; ///< Inner-border thickness.
    Color   borderColor;     ///< Color of the border.
    Texture borderTexture;   ///< Add texture the border, the color will act as a tint.
} CguiBoxElementData;

CGAPI CguiNode *CguiCreateBoxElement(float radius, Color color);                                                                                                                                                                                                 ///< Helper to create a box element node.
CGAPI CguiNode *CguiCreateBoxElementEx(float radius, Color color, float shadowDistance, Color shadowColor, float borderThickness, Color borderColor);                                                                                                            ///< Helper to create a box element node with extended parameters.
CGAPI CguiNode *CguiCreateBoxElementPro(Vector4 radii, Color color, Texture texture, float shadowDistance, Vector2 shadowOffset, float shadowShrink, Color shadowColor, Texture shadowTexture, float borderThickness, Color borderColor, Texture borderTexture); ///< Helper to create a box element node with pro parameters.
CGAPI void      CguiDrawPreBoxElement(CguiNode *node);                                                                                                                                                                                                           ///< Pre-draw function (attached) for box element node.
CGAPI bool      CguiIsBoxElementDataEqual(CguiBoxElementData a, CguiBoxElementData b);                                                                                                                                                                           ///< Check if box element data is equal.

//------------------------------------------------------------------------------
// Interpolation & Transitions
//------------------------------------------------------------------------------
//
// Transitions allow time-based value interpolation. Interpolation may be linear
// or eased using easing function.

// Interpolators
// Note: time t is not limited to be in the range 0..1, clamp appropriately.

typedef void (*CguiInterpFunction)(const void *a, const void *b, float t, void *out); ///< Function to interpolate between two values.

// Interpolators for common types

CGAPI int                    CguiInterpInt(int a, int b, float t);                                                      ///< Interpolate between two int.
CGAPI float                  CguiInterpFloat(float a, float b, float t);                                                ///< Interpolate between two float.
CGAPI Vector2                CguiInterpVector2(Vector2 a, Vector2 b, float t);                                          ///< Interpolate between two Vector2.
CGAPI Vector3                CguiInterpVector3(Vector3 a, Vector3 b, float t);                                          ///< Interpolate between two Vector3.
CGAPI Vector4                CguiInterpVector4(Vector4 a, Vector4 b, float t);                                          ///< Interpolate between two Vector4.
CGAPI Color                  CguiInterpColor(Color a, Color b, float t);                                                ///< Interpolate between two Color.
CGAPI Rectangle              CguiInterpRectangle(Rectangle a, Rectangle b, float t);                                    ///< Interpolate between two Rectangle.
CGAPI CguiTransformation     CguiInterpTransformation(CguiTransformation a, CguiTransformation b, float t);             ///< Interpolate between two CguiTransformation.
CGAPI CguiTextElementData    CguiInterpTextElementData(CguiTextElementData a, CguiTextElementData b, float t);          ///< Interpolate between two CguiTransformation.
CGAPI CguiTextureElementData CguiInterpTextureElementData(CguiTextureElementData a, CguiTextureElementData b, float t); ///< Interpolate between two CguiTransformation.
CGAPI CguiBoxElementData     CguiInterpBoxElementData(CguiBoxElementData a, CguiBoxElementData b, float t);             ///< Interpolate between two CguiTransformation.

// Transition

struct CguiTransition;
typedef struct CguiTransition CguiTransition;

/// Transition information.
struct CguiTransition {
    const void *from;   ///< Starting value to begin interpolation.
    const void *to;     ///< Ending value to end interpolation.
    void       *result; ///< Value to update during interpolation.

    CguiInterpFunction interp; ///< Interpolation function to obtain intermediate value.
    CguiEasingFunction easing; ///< Easing function to customize the interpolation curve.

    float delayBefore; ///< Delay before interpolation of this transition.
    float duration;    ///< Duration for interpolating this transition.
    float delayAfter;  ///< Delay after intepolation of this transition.

    bool reversed; ///< Whether to transition in reverse direction.

    int repeatCount; ///< Number of times to repeat this transition (0 = no repeat, N = N repeats, -1 = infinite repition).

    CguiTransition *prev; ///< Previous transition in the chain.
    CguiTransition *next; ///< Next transition in the chain.
};

/// Transition chain.
typedef struct CguiTransitionChain {
    CguiTransition *first;         ///< First transition in the chain (auto-freed).
    CguiTransition *last;          ///< Last transition in the chain (auto-freed).
    bool            paused;        ///< Whether to pause transition from continuing further.
    CguiTransition *active;        ///< Current active transition.
    int             activeRepeats; ///< Number of repeats made in the active transition.
    float           activeTime;    ///< Number of seconds passed since the beginning of the active transition.
    bool            finished;      ///< Whether all the transitions in the chain has ended.
} CguiTransitionChain;

CGAPI CguiTransition *CguiCreateTransition(void);                                                                                                                                                                                         ///< Create a transition.
CGAPI CguiTransition *CguiCreateTransitionEx(const void *from, const void *to, void *result, CguiInterpFunction interp, CguiEasingFunction easing, float duration);                                                                       ///< Create a transition.
CGAPI CguiTransition *CguiCreateTransitionPro(const void *from, const void *to, void *result, CguiInterpFunction interp, CguiEasingFunction easing, float delayBefore, float duration, float delayAfter, bool reversed, int repeatCount); ///< Create a transition with pro parameters.
CGAPI void            CguiDeleteTransition(CguiTransition *transition);                                                                                                                                                                   ///< Delete a transition.

CGAPI CguiTransitionChain *CguiCreateTransitionChain(void);                                                                     ///< Create a new transition chain starting at given time.
CGAPI void                 CguiDeleteTransitionChain(CguiTransitionChain *chain);                                               ///< Delete the transition chain and all transitions.
CGAPI void                 CguiDeleteTransitionChainSelf(CguiTransitionChain *chain);                                           ///< Delete the transition chain itself (not attached transitions).
CGAPI bool                 CguiInsertTransition(CguiTransitionChain *chain, CguiTransition *transition);                        ///< Insert a transition at the end.
CGAPI bool                 CguiInsertTransitionAt(CguiTransitionChain *chain, CguiTransition *transition, int transitionIndex); ///< Insert a transition at specified index.
CGAPI bool                 CguiRemoveTransition(CguiTransitionChain *chain, CguiTransition *transition);                        ///< Remove a specific transition.
CGAPI bool                 CguiRemoveTransitionAt(CguiTransitionChain *chain, int transitionIndex);                             ///< Remove transition at specified index.
CGAPI CguiTransition      *CguiCreateTransitionInChain(CguiTransitionChain *chain);                                             ///< Create and add a new transition at the end.
CGAPI CguiTransition      *CguiCreateTransitionInChainAt(CguiTransitionChain *chain, int transitionIndex);                      ///< Create and add a new transition at index.
CGAPI bool                 CguiDeleteTransitionInChain(CguiTransitionChain *chain);                                             ///< Delete last transition in the chain.
CGAPI bool                 CguiDeleteTransitionInChainAt(CguiTransitionChain *chain, int transitionIndex);                      ///< Delete transition at index in the chain.
CGAPI int                  CguiFindTransition(CguiTransitionChain *chain, CguiTransition *transition);                          ///< Find index of a transition in the chain.
CGAPI CguiTransition      *CguiGetTransitionAt(CguiTransitionChain *chain, int transitionIndex);                                ///< Get transition at specified index.
CGAPI int                  CguiGetTransitionsCount(CguiTransitionChain *chain);                                                 ///< Get the number of transitions in the chain.

CGAPI void CguiUpdateTransition(CguiTransition *transition, float elapsedTime); ///< Update transition.
CGAPI void CguiUpdateTransitionChain(CguiTransitionChain *chain);               ///< Update transition chain.
CGAPI void CguiRegisterAutoTransition(CguiTransition *transition);              ///< Register to automatically update and delete transition.
CGAPI void CguiRegisterAutoTransitionChain(CguiTransitionChain *chain);         ///< Register to automatically update and delete transition chain.
CGAPI void CguiUnregisterAutoTransition(CguiTransition *transition);            ///< Unregister to automatically updating and deleting transition.
CGAPI void CguiUnregisterAutoTransitionChain(CguiTransitionChain *chain);       ///< Unregister to automatically updating and deleting transition chain.
CGAPI void CguiUpdateRegisteredTransitions(void);                               ///< Update all registered transitions.

// Helpers to create transition for common types

CGAPI CguiTransition *CguiTransitInt(const int *a, const int *b, int *result, CguiEasingFunction easing, float duration);
CGAPI CguiTransition *CguiTransitFloat(const float *a, const float *b, float *result, CguiEasingFunction easing, float duration);
CGAPI CguiTransition *CguiTransitVector2(const Vector2 *a, const Vector2 *b, Vector2 *result, CguiEasingFunction easing, float duration);
CGAPI CguiTransition *CguiTransitVector3(const Vector3 *a, const Vector3 *b, Vector3 *result, CguiEasingFunction easing, float duration);
CGAPI CguiTransition *CguiTransitVector4(const Vector4 *a, const Vector4 *b, Vector4 *result, CguiEasingFunction easing, float duration);
CGAPI CguiTransition *CguiTransitColor(const Color *a, const Color *b, Color *result, CguiEasingFunction easing, float duration);
CGAPI CguiTransition *CguiTransitRectangle(const Rectangle *a, const Rectangle *b, Rectangle *result, CguiEasingFunction easing, float duration);
CGAPI CguiTransition *CguiTransitTransformation(const CguiTransformation *a, const CguiTransformation *b, CguiTransformation *result, CguiEasingFunction easing, float duration);
CGAPI CguiTransition *CguiTransitTextElementData(const CguiTextElementData *a, const CguiTextElementData *b, CguiTextElementData *result, CguiEasingFunction easing, float duration);
CGAPI CguiTransition *CguiTransitTextureElementData(const CguiTextureElementData *a, const CguiTextureElementData *b, CguiTextureElementData *result, CguiEasingFunction easing, float duration);
CGAPI CguiTransition *CguiTransitBoxElementData(const CguiBoxElementData *a, const CguiBoxElementData *b, CguiBoxElementData *result, CguiEasingFunction easing, float duration);

//------------------------------------------------------------------------------
// GUI Component Nodes
//------------------------------------------------------------------------------
//
// GUI components are composition of GUI elements and other components.

/// Common override field selector to override fields during template syncing.
typedef enum CguiCommonOverrideField {
    CGUI_COMMON_OVERRIDE_FIELD_ENABLED        = 1 << 0,
    CGUI_COMMON_OVERRIDE_FIELD_NAME           = 1 << 1,
    CGUI_COMMON_OVERRIDE_FIELD_TRANSFORMATION = 1 << 2
} CguiCommonOverrideField;

/// Common fields that can be overriden from components.
typedef struct CguiCommonOverrides {
    bool               enabled;        ///< Override enabled-ness.
    const char        *name;           ///< Override name.
    CguiTransformation transformation; ///< Override transformation.
    int                fields;         ///< Fields to override.
} CguiCommonOverrides;

CGAPI void CguiApplyOverrides(CguiNode *node, CguiCommonOverrides overrides);                                             ///< Apply overrides to the node by selecting appropriate fields.
CGAPI void CguiOverrideEnabled(CguiNode *node, CguiCommonOverrides *overrides, bool enabled);                             ///< Helper to Add a common override for enabled field.
CGAPI void CguiOverrideName(CguiNode *node, CguiCommonOverrides *overrides, const char *name);                            ///< Helper to Add a common override for enabled field.
CGAPI void CguiOverrideTransformation(CguiNode *node, CguiCommonOverrides *overrides, CguiTransformation transformation); ///< Helper to Add a common override for enabled field.

/// Component type. 0x00FF0F is the type prefix.
typedef enum CguiComponentNodeType {
    CGUI_COMPONENT_NODE_TYPE_ROOT = 0x00FF0F00, ///< Root node as a component.
    CGUI_COMPONENT_NODE_TYPE_LAYER,             ///< Layer container component.
    CGUI_COMPONENT_NODE_TYPE_LABEL,             ///< Label component.
    CGUI_COMPONENT_NODE_TYPE_BUTTON,            ///< Button component.
    CGUI_COMPONENT_NODE_TYPE_TOGGLE,            ///< Toggle component.
    CGUI_COMPONENT_NODE_TYPE_MAX
} CguiComponentNodeType;

/// Component ID used to arrange components.
typedef enum CguiComponentID {
    CGUI_COMPONENT_ROOT,   ///< Root node as a component.
    CGUI_COMPONENT_LAYER,  ///< Layer container component.
    CGUI_COMPONENT_LABEL,  ///< Label component.
    CGUI_COMPONENT_BUTTON, ///< Button component.
    CGUI_COMPONENT_TOGGLE, ///< Toggle component.
    CGUI_COMPONENT_MAX
} CguiComponentID;

/// Root node data.
typedef struct CguiRootData {
    // Auto-assigned by theme

    Color              backgroundColor;    ///< Rectangle fill that will act as a background color.
    float              transitionDuration; ///< Transition duration.
    CguiEasingFunction transitionEasing;   ///< Transition easing function.
} CguiRootData;

/// Root node instance-specific data.
typedef struct CguiRootInstanceData {
    Color                currentBackgroundColor;       ///< Current background color before the transition.
    Color                targetBackgroundColor;        ///< Target background color to transition towards.
    Color                transitioningBackgroundColor; ///< Transitioning background color which will be used to draw.
    CguiTransitionChain *transitionChain;              ///< Transition for background color.

    // Overriding fields

    CguiCommonOverrides overrides; ///< Common fields that can be overriden per instances.
} CguiRootInstanceData;

CGAPI CguiNode *CguiCreateRoot(void);              ///< Helper to create a root node.
CGAPI void      CguiPreUpdateRoot(CguiNode *node); ///< Pre-update function (attached) for root node.
CGAPI void      CguiPreDrawRoot(CguiNode *node);   ///< Pre-draw function (attached) for root node.
CGAPI void      CguiOverrideRoot(CguiNode *node);  ///< Override function (attached) for root node.

/// Composition or child indices of layer node.
typedef enum CguiLayerComposition {
    CGUI_LAYER_COMPOSITION_BOX_NODE ///< Box element node.
} CguiLayerComposition;

/// Layer types.
typedef enum CguiLayerType {
    CGUI_LAYER_TYPE_BACKLAYER,  ///< Layer with backlayer color.
    CGUI_LAYER_TYPE_MIDLAYER,   ///< Layer with midlayer color.
    CGUI_LAYER_TYPE_FRONTLAYER, ///< Layer with frontlayer color.
    CGUI_LAYER_TYPE_MAX
} CguiLayerType;

/// Layer container node data.
typedef struct CguiLayerData {
    // Auto-assigned by theme

    CguiBoxElementData boxDatas[CGUI_LAYER_TYPE_MAX]; ///< Box element data of layer container of different types.
    float              transitionDuration;            ///< Transition duration.
    CguiEasingFunction transitionEasing;              ///< Transition easing function.
} CguiLayerData;

/// Layer container node instance-specific data.
typedef struct CguiLayerInstanceData {
    CguiBoxElementData   currentBoxData;       ///< Current box data before the transition.
    CguiBoxElementData   targetBoxData;        ///< Target box data to transition towards.
    CguiBoxElementData   transitioningBoxData; ///< Transitioning box data which will be used to draw.
    CguiTransitionChain *transitionChain;      ///< Transition for box data.

    int type; ///< Layer type.

    // Overriding fields

    CguiCommonOverrides overrides; ///< Common fields that can be overriden per instances.
} CguiLayerInstanceData;

CGAPI CguiNode *CguiCreateLayer(CguiTransformation transformation, int type); ///< Helper to create a layer node.
CGAPI void      CguiPreUpdateLayer(CguiNode *node);                           ///< Pre-update function (attached) for layer node.
CGAPI void      CguiOverrideLayer(CguiNode *node);                            ///< Override function (attached) for layer node.

/// Composition or child indices of label node.
typedef enum CguiLabelComposition {
    CGUI_LABEL_COMPOSITION_TEXT_NODE, ///< Text element node.
} CguiLabelComposition;

/// Label types.
typedef enum CguiLabelType {
    CGUI_LABEL_TYPE_HEADING, ///< Heading type (bold/large text).
    CGUI_LABEL_TYPE_BODY,    ///< Body type (normal text).
    CGUI_LABEL_TYPE_CAPTION, ///< Caption type (light/small text).
    CGUI_LABEL_TYPE_MAX
} CguiLabelType;

/// Label component data.
typedef struct CguiLabelData {
    // Auto-assigned by theme

    CguiTextElementData textDatas[CGUI_LABEL_TYPE_MAX];         ///< Text element data for label for each type.
    CguiTextElementData disabledTextDatas[CGUI_LABEL_TYPE_MAX]; ///< Text element data for disabled label for each type.
    float               transitionDuration;                     ///< Transition duration.
    CguiEasingFunction  transitionEasing;                       ///< Transition easing function.
} CguiLabelData;

/// Label component instance-specific data.
typedef struct CguiLabelInstanceData {
    CguiTextElementData  currentTextData;       ///< Current text data before the transition.
    CguiTextElementData  targetTextData;        ///< Target text data to transition towards.
    CguiTextElementData  transitioningTextData; ///< Transitioning text data which will be used to draw.
    CguiTransitionChain *transitionChain;       ///< Transition for box data.

    const char *text;     ///< Label text.
    int         type;     ///< Label type.
    bool        disabled; ///< Disabled label (grayed out text).
    int         xJustify; ///< Text justification in x-axis.
    int         yJustify; ///< Text justification in y-axis.

    // Overriding fields

    CguiCommonOverrides overrides; ///< Common fields that can be overriden per instances.
} CguiLabelInstanceData;

CGAPI CguiNode *CguiCreateLabel(CguiTransformation transformation, const char *text, int type, bool disabled, int xJustify, int yJustify); ///< Helper to create a label node.
CGAPI void      CguiPreUpdateLabel(CguiNode *node);                                                                                        ///< Pre-update function (attached) for label node.
CGAPI void      CguiOverrideLabel(CguiNode *node);                                                                                         ///< Override function (attached) for label node.

typedef void (*CguiButtonPressCallback)(CguiNode *button); ///< Callback for when button is pressed.

/// Composition or child indices of button node.
typedef enum CguiButtonComposition {
    CGUI_BUTTON_COMPOSITION_BOX_NODE ///< Box element node.
} CguiButtonComposition;

/// Button types.
typedef enum CguiButtonType {
    CGUI_BUTTON_TYPE_NORMAL,    ///< Normal button.
    CGUI_BUTTON_TYPE_FLAT,      ///< Button with section color.
    CGUI_BUTTON_TYPE_ACCENT,    ///< Button with accent color.
    CGUI_BUTTON_TYPE_DANGEROUS, ///< Button with dangerous color.
    CGUI_BUTTON_TYPE_WARNING,   ///< Button with warning color.
    CGUI_BUTTON_TYPE_SUCCESS,   ///< Button with success color.
    CGUI_BUTTON_TYPE_INFO,      ///< Button with info color.
    CGUI_BUTTON_TYPE_MAX
} CguiButtonType;

/// Button component data.
typedef struct CguiButtonData {
    // Auto-assigned by theme

    CguiBoxElementData boxDatas[CGUI_BUTTON_TYPE_MAX];         ///< Box element data for button of different types.
    CguiBoxElementData hoveredBoxDatas[CGUI_BUTTON_TYPE_MAX];  ///< Box element data for button of different types when hovered.
    CguiBoxElementData heldBoxDatas[CGUI_BUTTON_TYPE_MAX];     ///< Box element data for button of different types when held.
    CguiBoxElementData disabledBoxDatas[CGUI_BUTTON_TYPE_MAX]; ///< Box element data for button of different types when disabled.
    float              transitionDuration;                     ///< Transition duration.
    CguiEasingFunction transitionEasing;                       ///< Transition easing function.
} CguiButtonData;

/// Button component instance-specific data.
typedef struct CguiButtonInstanceData {
    CguiBoxElementData   currentBoxData;       ///< Current box data before the transition.
    CguiBoxElementData   targetBoxData;        ///< Target box data to transition towards.
    CguiBoxElementData   transitioningBoxData; ///< Transitioning box data which will be used to draw.
    CguiTransitionChain *transitionChain;      ///< Transition for box data.

    int                     mouseButton;   ///< Mouse button to detect press (default: MOUSE_BUTTON_LEFT).
    int                     type;          ///< Button type.
    CguiButtonPressCallback pressCallback; ///< Callback called when button is pressed.

    // States

    bool hovered;  ///< True if the button is hovered by the mouse cursor.
    bool held;     ///< True if hovered and mouse button was pressed.
    bool active;   ///< True when button is pressed (for one frame).
    bool disabled; ///< Make the button non-interactible and gray-out the button.

    // Overriding fields

    CguiCommonOverrides overrides; ///< Common fields that can be overriden per instances.
} CguiButtonInstanceData;

CGAPI CguiNode *CguiCreateButton(CguiTransformation transformation, int type, CguiButtonPressCallback pressCallback, bool disabled); ///< Helper to create a button node.
CGAPI void      CguiPreUpdateButton(CguiNode *node);                                                                                 ///< Pre-update function (attached) for the button node.
CGAPI void      CguiOverrideButton(CguiNode *node);                                                                                  ///< Override function (attached) for the button node.
CGAPI bool      CguiHandleButtonEvents(CguiNode *node, CguiEvent *event);                                                            ///< Event handler function (attached) for the button node.

typedef void (*CguiTogglePressCallback)(CguiNode *toggle, bool active); ///< Callback for when toggle is pressed.

/// Composition or child indices of toggle node.
typedef enum CguiToggleComposition {
    CGUI_TOGGLE_COMPOSITION_BOX_NODE ///< Box element node.
} CguiToggleComposition;

/// Toggle component data.
typedef struct CguiToggleData {
    // Auto-assigned by theme

    CguiBoxElementData boxData;               ///< Box element data for toggle.
    CguiBoxElementData hoveredBoxData;        ///< Box element data for toggle when hovered.
    CguiBoxElementData heldBoxData;           ///< Box element data for toggle when held.
    CguiBoxElementData disabledBoxData;       ///< Box element data for toggle when disabled.
    CguiBoxElementData activeBoxData;         ///< Box element data for toggle when active.
    CguiBoxElementData activeHoveredBoxData;  ///< Box element data for toggle when active and hovered.
    CguiBoxElementData activeHeldBoxData;     ///< Box element data for toggle when active held.
    CguiBoxElementData activeDisabledBoxData; ///< Box element data for toggle when active and disabled.
    float              transitionDuration;    ///< Transition duration.
    CguiEasingFunction transitionEasing;      ///< Transition easing function.
} CguiToggleData;

/// Toggle component instance-specific data.
typedef struct CguiToggleInstanceData {
    CguiBoxElementData   currentBoxData;       ///< Current box data before the transition.
    CguiBoxElementData   targetBoxData;        ///< Target box data to transition towards.
    CguiBoxElementData   transitioningBoxData; ///< Transitioning box data which will be used to draw.
    CguiTransitionChain *transitionChain;      ///< Transition for box data.

    int                     mouseToggle;   ///< Mouse toggle to detect press (default: MOUSE_TOGGLE_LEFT).
    CguiTogglePressCallback pressCallback; ///< Callback called when toggle is pressed.

    // States

    bool hovered;  ///< True if the toggle is hovered by the mouse cursor.
    bool held;     ///< True if hovered and mouse toggle was pressed.
    bool active;   ///< True when toggle is pressed.
    bool disabled; ///< Make the toggle non-interactible and gray-out the toggle.

    // Overriding fields

    CguiCommonOverrides overrides; ///< Common fields that can be overriden per instances.
} CguiToggleInstanceData;

CGAPI CguiNode *CguiCreateToggle(CguiTransformation transformation, bool active, CguiTogglePressCallback pressCallback, bool disabled); ///< Helper to create a toggle node.
CGAPI void      CguiPreUpdateToggle(CguiNode *node);                                                                                    ///< Pre-update function (attached) for the toggle node.
CGAPI void      CguiOverrideToggle(CguiNode *node);                                                                                     ///< Override function (attached) for the toggle node.
CGAPI bool      CguiHandleToggleEvents(CguiNode *node, CguiEvent *event);                                                               ///< Event handler function (attached) for the toggle node.

//------------------------------------------------------------------------------
// Theming Components
//------------------------------------------------------------------------------

struct CguiTheme;
typedef struct CguiTheme CguiTheme;

typedef void (*CguiDeleteThemeData)(CguiTheme *theme); ///< Delete function to delete the theme data (do not free node data itself, as it is freed automatically).

/// Theming for components.
struct CguiTheme {
    char               *themeName;       ///< Name of the theme.
    void               *themeData;       ///< Theme data for internal storage (auto-freed).
    int                 themeDataSize;   ///< Theme data size.
    CguiDeleteThemeData deleteThemeData; ///< Delete function to delete theme data.
    CguiNode          **templates;       ///< Templates for components (auto-freed, including nodes, array in order of component node types).
};

CGAPI CguiTheme *CguiCreateTheme(const char *themeName);                        ///< Create a custom theme.
CGAPI void       CguiDeleteTheme(CguiTheme *theme);                             ///< Delete a theme and templates.
CGAPI void       CguiSetActiveTheme(CguiTheme *theme);                          ///< Set the active theme. All components will now use this theme.
CGAPI CguiTheme *CguiGetActiveTheme(void);                                      ///< Get the active theme.
CGAPI CguiTheme *CguiGetDefaultTheme(void);                                     ///< Get the default theme set initially.
CGAPI CguiNode  *CguiGetComponentTemplate(CguiTheme *theme, int componentType); ///< Get a component template node from the theme.
CGAPI bool       CguiSetComponentTemplate(CguiTheme *theme, CguiNode *node);    ///< Set a component template node to the theme.

// Crystalline theme

/// Data for Crystalline Theme (default Crystal GUI theme).
typedef struct CguiCrystallineThemeData {
    // Hues

    float accentHue;    ///< Hue for primary color or highlight.
    float dangerousHue; ///< Hue used to indicate errors or destructive actions.
    float warningHue;   ///< Hue used for warnings or cautionary indicators.
    float successHue;   ///< Hue used to indicate success or positive status.
    float infoHue;      ///< Hue used for informational elements or neutral status.

    // SLA -> Saturation, Lightness, Alpha, Saturation and Lightness as in HSL, Hue undefined

    Vector3 backgroundSLA;         ///< Background color for window.
    Vector3 backgroundHoveredSLA;  ///< Background color for window when hovered.
    Vector3 backgroundHeldSLA;     ///< Background color for window when held.
    Vector3 backgroundDisabledSLA; ///< Background color for window when disabled.
    Vector3 backlayerSLA;          ///< Background color for backlayer containers.
    Vector3 backlayerHoveredSLA;   ///< Background color for backlayer containers when hovered.
    Vector3 backlayerHeldSLA;      ///< Background color for backlayer containers when held.
    Vector3 backlayerDisabledSLA;  ///< Background color for backlayer containers when disabled.
    Vector3 midlayerSLA;           ///< Background color for midlayer containers.
    Vector3 midlayerHoveredSLA;    ///< Background color for midlayer containers when hovered.
    Vector3 midlayerHeldSLA;       ///< Background color for midlayer containers when held.
    Vector3 midlayerDisabledSLA;   ///< Background color for midlayer containers when disabled.
    Vector3 frontlayerSLA;         ///< Background color for frontlayer containers.
    Vector3 frontlayerHoveredSLA;  ///< Background color for frontlayer containers when hovered.
    Vector3 frontlayerHeldSLA;     ///< Background color for frontlayer containers when held.
    Vector3 frontlayerDisabledSLA; ///< Background color for frontlayer containers when disabled.
    Vector3 foregroundSLA;         ///< Foreground color for components.
    Vector3 foregroundHoveredSLA;  ///< Foreground color for components when hovered.
    Vector3 foregroundHeldSLA;     ///< Foreground color for components when held.
    Vector3 foregroundDisabledSLA; ///< Foreground color for components when disabled.
    Vector3 activeSLA;             ///< Foreground color for active components.
    Vector3 activeHoveredSLA;      ///< Foreground color for active components when hovered.
    Vector3 activeHeldSLA;         ///< Foreground color for active components when held.
    Vector3 activeDisabledSLA;     ///< Foreground color for active components when disabled.
    Vector3 inactiveSLA;           ///< Foreground color for inactive components.
    Vector3 inactiveHoveredSLA;    ///< Foreground color for inactive components when hovered.
    Vector3 inactiveHeldSLA;       ///< Foreground color for inactive components when held.
    Vector3 inactiveDisabledSLA;   ///< Foreground color for inactive components when disabled.
    Vector3 flatSLA;               ///< Foreground color for flat components.
    Vector3 flatHoveredSLA;        ///< Foreground color for flat components when hovered.
    Vector3 flatHeldSLA;           ///< Foreground color for flat components when held.
    Vector3 flatDisabledSLA;       ///< Foreground color for flat components when disabled.

    Vector3 textSLA;         ///< Default color for text.
    Vector3 disabledTextSLA; ///< Default color for disabled text.

    // Text

    float headingFontSize; ///< Font size for heading text.
    float bodyFontSize;    ///< Font size for body text.
    float captionFontSize; ///< Font size for caption text.

    float headingLineHeight; ///< Line height for heading text.
    float bodyLineHeight;    ///< Line height for body text.
    float captionLineHeight; ///< Line height for caption text.

    Font textFont;            ///< Font for text.
    Font textFontBold;        ///< Font for bold text.
    Font textFontItalic;      ///< Font for italic text.
    Font textFontBoldItalic;  ///< Font for bold-italic text.
    Font textFontLight;       ///< Font for light text.
    Font textFontLightItalic; ///< Font for light-italic text.

    // Dimensions

    Vector4 backlayerRadii;  ///< Radii of backlayer containers.
    Vector4 midlayerRadii;   ///< Radii of midlayer containers.
    Vector4 frontlayerRadii; ///< Radii of frontlayer containers.

    float   layerShadowDistance;  ///< Shadow distance of layer containers.
    Vector2 layerShadowOffset;    ///< Shadow offset of layer containers.
    float   layerShadowShrink;    ///< Shadow shrink of layer containers.
    Vector3 layerShadowSLA;       ///< Color for shadows.
    float   layerBorderThickness; ///< Border thickness of layer containers.
    Vector3 layerBorderSLA;       ///< Color for borders.

    Vector4 componentRadii;           ///< Radii of roundable components.
    float   componentShadowDistance;  ///< Shadow distance of roundable components.
    Vector2 componentShadowOffset;    ///< Shadow offset of roundable components.
    float   componentShadowShrink;    ///< Shadow shrink of roundable components.
    Vector3 componentShadowSLA;       ///< Color for shadows.
    float   componentBorderThickness; ///< Border thickness of roundable components.
    Vector3 componentBorderSLA;       ///< Color for borders.

    // Transitions

    float              componentTransitionDuration; ///< Duration for transition of component's states.
    CguiEasingFunction componentTransitionEasing;   ///< Easing function for transition.
} CguiCrystallineThemeData;

CGAPI CguiTheme *CguiCreateCrystallineThemeDark(void);                              ///< Helper to create the Crystalline theme (Dark version).
CGAPI CguiTheme *CguiCreateCrystallineThemeLight(void);                             ///< Helper to create the Crystalline theme (Light version).
CGAPI CguiTheme *CguiCreateCrystallineThemeDarkHighContrast(void);                  ///< Helper to create the Crystalline theme (Dark, high-contrast version).
CGAPI CguiTheme *CguiCreateCrystallineThemeLightHighContrast(void);                 ///< Helper to create the Crystalline theme (Light, high-contrast version).
CGAPI CguiTheme *CguiCreateCrystallineThemeFromData(CguiCrystallineThemeData data); ///< Helper to create the Crystalline theme from customized data.
CGAPI void       CguiDeleteCrystallineTheme(CguiTheme *theme);                      ///< Delete function (attached) for the Crystalline theme.

//------------------------------------------------------------------------------
// Core
//------------------------------------------------------------------------------
//
// Manage Crystal GUI.

CGAPI void CguiInit(void);                             ///< Initialize Crystal GUI and load resources. Call this before anything Crystal GUI.
CGAPI void CguiClose(void);                            ///< Unload resources and deinitialize Crystal GUI. Call this after everything Crystal GUI.
CGAPI void CguiUpdate(CguiNode *root);                 ///< Update the entire scene graph, including transformation, event handling, etc.
CGAPI void CguiDraw(CguiNode *root, bool debugBounds); ///< Draw the entire scene graph, including debug bounds if debugBounds is true.

#ifdef __cplusplus
}
#endif

#endif // CRYSTALGUI_H
