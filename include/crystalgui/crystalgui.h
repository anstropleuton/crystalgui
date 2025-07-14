/// @file
///
/// @author    Anstro Pleuton
/// @copyright Copyright (c) 2025 Anstro Pleuton
///
/// Crystal GUI - A modern GUI framework for raylib.
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

#ifndef CG_REALLOC
#define CG_REALLOC(ptr, sz) MemRealloc(ptr, sz)
#endif

#ifndef CG_FREE
#define CG_FREE(ptr) MemFree(ptr)
#endif

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

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
// Raylib-like extra features.
//------------------------------------------------------------------------------

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

CGAPI Vector2   CguiRotatePoint(Vector2 point, Vector2 origin, float angle);   ///< Rotate point along origin with angle (in degrees).
CGAPI Rectangle CguiRecZero(void);                                             ///< Get an all-zero rectangle.
CGAPI Texture   CguiLoadTextureFromRenderTexture(RenderTexture renderTexture); ///< Load texture from render texture (NOTE: Flips the render texture's y axis).
CGAPI float     CguiGetMinRoundness(Rectangle rec, float roundness);           ///< Get the minimum roundness acceptable by the rectangle.
CGAPI Color     CguiColorMultiply(Color color, Vector4 values);                ///< Multiply color with RGBA values.
CGAPI void      CguiBeginScissorModeRec(Rectangle area);                       ///< Begin scissor mode (define a screen area for following drawing) using rectangle.
CGAPI Rectangle CguiFlipRectangleX(Rectangle rec, float axis);                 ///< Flip rectangle in X-axis.
CGAPI Rectangle CguiFlipRectangleY(Rectangle rec, float axis);                 ///< Flip rectangle in Y-axis.
CGAPI Rectangle CguiFlipRectangleXY(Rectangle rec, Vector2 axis);              ///< Flip rectangle in X- and Y-axis.

// Easing functions
// Easing values may not be clamped in range 0..1 (e.g., elastic easing)
// It is better to design your UI to be twice as large in hidden areas

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

/// Animated texture.
typedef struct CguiAnimatedTexture {
    Texture           *frames;         ///< Frames (textures) of animated texture.
    int                framesCount;    ///< Number of frames.
    int                framesCapacity; ///< Number of frames that can be inserted before reallocation.
    float              delay;          ///< Number of seconds to wait before the animation start.
    float              duration;       ///< Number of seconds the animation must last.
    CguiEasingFunction easingFunction; ///< Easing function to use for animating the frames (default: linear).
    int                repeatCount;    ///< Number of times to repeat the animation. Use -1 to repeat indefinitely.
    bool               repeatDelay;    ///< Whether to include delay before repeating.
} CguiAnimatedTexture;

// Animated texture management

CGAPI CguiAnimatedTexture CguiLoadAnimatedTexture(const char *filename);                                          ///< Loads an animated texture from video/video-like file (mp4, gif, etc.).
CGAPI void                CguiUnloadAnimatedTexture(CguiAnimatedTexture texture);                                 ///< Unloads an animated texture.
CGAPI void                CguiAnimatedTextureInsertFrame(CguiAnimatedTexture *texture, Texture frame, int index); ///< Insert a frame in the animated texture at index.
CGAPI void                CguiAnimatedTextureRemoveFrame(CguiAnimatedTexture *texture, int index);                ///< Insert a frame in the animated texture at index.

// Animated texture drawing

CGAPI void CguiDrawAnimatedTexture(CguiAnimatedTexture texture, float startTime, float x, float y, Color tint);                                                   ///< Draw an animated texture.
CGAPI void CguiDrawAnimatedTextureEx(CguiAnimatedTexture texture, float startTime, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint); ///< Draw an animated texture with extended parameters.

//------------------------------------------------------------------------------
// Events
//------------------------------------------------------------------------------

/// Event type for GUI events.
typedef enum CguiEventType {
    CGUI_EVENT_TYPE_MOUSE_MOVE,           ///< Event when mouse moves.
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
typedef struct CguiMouseMoveEvent {
    int     eventType; ///< Event type.
    Vector2 delta;     ///< Mouse movement delta.
    bool    moveIn;    ///< Mouse is moving in to the bounds.
    bool    moveOut;   ///< Mouse is moving out from the bounds.
} CguiMouseMoveEvent;

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

//------------------------------------------------------------------------------
// Node
//------------------------------------------------------------------------------

/// GUI node transformation.
typedef struct CguiTransformation {
    Vector2 position;           ///< Position of the node.
    Vector2 size;               ///< Size of the node.
    Vector2 isRelativePosition; ///< Whether the position (x, y) is relative to parent (position acts as offset if true) [if non-zero or non-one, results are interpolated].
    Vector2 isRelativeSize;     ///< Whether the size (x, y) is relative to parent (size acts as scale if true) [if non-zero or non-one, results are interpolated].
    Vector2 anchor;             ///< Anchoring when position is relative to parent (0, 0 -> top left, 1, 1 -> bottom right, 0.5, 0.5 -> center, etc.).
    Vector2 shrink;             ///< Shrinking when size is relative to parent (0, 0 -> no shrinking, 20, 10 -> shrink node 20, 10 units smaller than parent, etc.).
} CguiTransformation;

struct CguiNode;
typedef struct CguiNode CguiNode;

typedef bool (*CguiTransformNodeFunction)(CguiNode *node);                 ///< Transform function to update the node's transformation. Return true if transform changed.
typedef void (*CguiUpdateNodeFunction)(CguiNode *node);                    ///< Update function to update the node.
typedef void (*CguiDrawNodeFunction)(CguiNode *node);                      ///< Draw function to draw the node.
typedef void (*CguiDebugDrawNodeFunction)(CguiNode *node);                 ///< Debug-draw function to draw debug of the node.
typedef void (*CguiDeleteNodeDataFunction)(CguiNode *node);                ///< Delete function to delete node data properly (do not free node data itself, as it is freed automatically).
typedef bool (*CguiHandleEventFunction)(CguiNode *node, CguiEvent *event); ///< Handle event. Return true if consumed.

/// GUI node for nesting.
struct CguiNode {
    bool               enabled;        ///< Whether the node is enabled or disabled (enables or disables updates and rendering, including events).
    bool               visible;        ///< Whether the node is visible (updates the node, but does not render).
    CguiTransformation transformation; ///< Transformation of the node. Apply for recache when modifying.

    Rectangle bounds;  ///< Calculated bounds of the node.
    bool      rebound; ///< Whether to recalculate bounds. Another way to trigger rebound is to return true from the attached transform function.

    CguiNode  *parent;           ///< Reference to parent node.
    CguiNode **children;         ///< Children nodes (node owns its children).
    int        childrenCount;    ///< Number of children nodes.
    int        childrenCapacity; ///< Number of children that can be inserted before reallocation.

    // Instance node of a template node will copy all properties

    CguiNode *templateSource; ///< Node is an instance of a node that is a template.
    bool      syncInstances;  ///< Sync all the instance of this template node with this node.

    // It is recommended to almost always use pre-order functions rather than post-order functions

    CguiTransformNodeFunction  transform;      ///< Transform function.
    CguiUpdateNodeFunction     updatePre;      ///< Update function (called before all children).
    CguiUpdateNodeFunction     updatePost;     ///< Update function (called after all children).
    CguiDrawNodeFunction       drawPre;        ///< Draw function (called before all children).
    CguiDrawNodeFunction       drawPost;       ///< Draw function (called after all children).
    CguiDebugDrawNodeFunction  debugDraw;      ///< Debug-draw function.
    CguiDeleteNodeDataFunction deleteNodeData; ///< Delete function.
    CguiHandleEventFunction    handleEvent;    ///< Event handler function.

    char *name;     ///< Name of the node. Names starting with "Cgui" are reserved (auto-freed).
    int   type;     ///< Type of the node (for polymorphism). Numbers in form of 0x00FFxxxx are reserved.
    void *data;     ///< Node data (auto-freed).
    int   dataSize; ///< Number of bytes of data.
};

// Node management

CGAPI CguiNode *CguiCreateNode(void);                                                          ///< Create a new node.
CGAPI CguiNode *CguiCreateNodeEx(const char *name);                                            ///< Create a new named node.
CGAPI CguiNode *CguiCreateNodePro(const char *name, int type, const void *data, int dataSize); ///< Create a new named node with type and optional data.
CGAPI void      CguiDeleteNode(CguiNode *node);                                                ///< Delete a node (this will deallocate children).
CGAPI void      CguiDeleteNodeSelf(CguiNode *node);                                            ///< Delete a node (this does not deallocate children).
CGAPI bool      CguiRenameNode(CguiNode *node, const char *newName);                           ///< Rename a node.

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

// Node children management

CGAPI CguiNode *CguiCreateChild(CguiNode *parent);                                                                 ///< Create a new child for the parent and inserts at end.
CGAPI CguiNode *CguiCreateChildAt(CguiNode *parent, int childIndex);                                               ///< Create a new child for the parent and inserts at index.
CGAPI bool      CguiInsertChild(CguiNode *parent, CguiNode *child);                                                ///< Insert a child to the parent at end, returns true if inserted.
CGAPI bool      CguiInsertChildAt(CguiNode *parent, CguiNode *child, int childIndex);                              ///< Insert a child to the parent at index, returns true if inserted.
CGAPI bool      CguiRemoveChild(CguiNode *parent, CguiNode *child);                                                ///< Remove a child from the parent, returns true if removed (this does not deallocate child).
CGAPI bool      CguiRemoveChildAt(CguiNode *parent, int childIndex);                                               ///< Remove the index'th child from the parent, returns true if removed (this does not deallocate child).
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

// Transformation helpers

CGAPI CguiTransformation CguiTAbsolute(Vector2 pos, Vector2 size);                              ///< Absolute positioning/sizing transformation.
CGAPI CguiTransformation CguiTOffset(Vector2 offset, Vector2 size);                             ///< Relative positioning (offsetting) and absolute sizing transformation.
CGAPI CguiTransformation CguiTScale(Vector2 pos, Vector2 scale);                                ///< Absolute positioning and relative sizing (scaling) transformation.
CGAPI CguiTransformation CguiTOffsetScale(Vector2 offset, Vector2 scale);                       ///< Relative positioning (offsetting)/sizing (scaling) transformation.
CGAPI CguiTransformation CguiTCentered(Vector2 size);                                           ///< Centered in parent transformation.
CGAPI CguiTransformation CguiTPadded(float padding);                                            ///< Padded in parent transformation.
CGAPI bool               CguiIsTransformationEqual(CguiTransformation a, CguiTransformation b); ///< Check if all field of transformation match.

// Misc.

CGAPI bool      CguiCopyNodeValues(CguiNode *fromNode, CguiNode *toNode); ///< Copy all fields from one node to another (excluding hierarchy and name).
CGAPI int       CguiFindChildIndex(CguiNode *parent, CguiNode *child);    ///< Find the direct child in parent and return an index, -1 if not found.
CGAPI bool      CguiIsDescendantOf(CguiNode *parent, CguiNode *child);    ///< Check if a node is a child of parent (recursively).
CGAPI bool      CguiIsAncestorOf(CguiNode *child, CguiNode *parent);      ///< Check if a node is a parent of child (recursively).
CGAPI CguiNode *CguiFindTypeInChildren(CguiNode *parent, int type);       ///< Returns the first found node type in itself or children (recursively), NULL if not found.
CGAPI CguiNode *CguiFindTypeInParents(CguiNode *child, int type);         ///< Returns the first found node type in itself or parents (recursively), NULL if not found.
CGAPI Rectangle CguiComputeNodeBounds(CguiNode *node);                    ///< Compute node bounds recursively for recache applied nodes.
CGAPI CguiNode *CguiCheckCollision(CguiNode *node, Vector2 point);        ///< Check for collision with the bounds of the inner-most node, return NULL if no collision.
CGAPI void      CguiRequestFocus(CguiNode *node);                         ///< Request to focus the node.
CGAPI void      CguiRequestUnfocus(void);                                 ///< Request to unfocus the node.
CGAPI CguiNode *CguiGetFocusedNode(void);                                 ///< Get the currently focused node.
CGAPI void      CguiSyncAnyInstances(CguiNode *node);                     ///< Syncs this node if it has attached template source (recursively).

//------------------------------------------------------------------------------
// Layout Nodes
//------------------------------------------------------------------------------

// Note: Layout nodes directly modifies the transformation of it's children
// if the children are of the corresponding item type of the layout node.

/// Layout node type, 0x00FF00 is the type prefix.
typedef enum CguiLayoutNodeType {
    CGUI_LAYOUT_NODE_TYPE_CLAMP = 0x00FF0000, ///< Clamp node type.
    CGUI_LAYOUT_NODE_TYPE_CLAMP_ITEM,         ///< Clamp node's item type.
    CGUI_LAYOUT_NODE_TYPE_LINEAR,             ///< Linear node type.
    CGUI_LAYOUT_NODE_TYPE_LINEAR_ITEM,        ///< Linear node's item type.
    CGUI_LAYOUT_NODE_TYPE_GRID,               ///< Grid node type.
    CGUI_LAYOUT_NODE_TYPE_GRID_ITEM,          ///< Grid node's item type.
    CGUI_LAYOUT_NODE_TYPE_FLOW,               ///< Flow node type.
    CGUI_LAYOUT_NODE_TYPE_FLOW_ITEM           ///< Flow node's item type.
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
} CguiClampLayoutData;

CGAPI CguiNode *CguiCreateClampLayout(void);              ///< Helper to create a clamp layout node.
CGAPI bool      CguiTransformClampLayout(CguiNode *node); ///< Transform function (attached) for clamp layout node.

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
/// Items are arranged horizontally.
typedef struct CguiLinearLayoutData {
    int   direction; ///< Layout direction for items.
    int   justify;   ///< Layout justification if space is available (or exceeded).
    float spacing;   ///< Spacing between items of the layout.
} CguiLinearLayoutData;

CGAPI CguiNode *CguiCreateLinearLayout(int direction, int justify, float spacing); ///< Helper to create a linear layout node.
CGAPI bool      CguiTransformLinearLayout(CguiNode *node);                         ///< Transform function (attached) for linear layout node.

/// Linear layout item data.
/// Control the linear item.
typedef struct CguiLinearLayoutItemData {
    float weight;  ///< Weight of the item in the layout.
    float minSize; ///< Minimum size of the item in the layout.
    float maxSize; ///< Maximum size of the item in the layout.
} CguiLinearLayoutItemData;

CGAPI CguiNode *CguiCreateLinearLayoutItem(float weight, float minSize, float maxSize); ///< Helper to create a linear layout's item node.

/// Grid slot data.
typedef struct CguiGridLayoutSlotData {
    float weight;  ///< Weight of slot in the layout.
    float minSize; ///< Minimum size of slot in the layout.
    float maxSize; ///< Maximum size of slot in the layout.

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

CGAPI CguiNode *CguiCreateGridLayout(int xSlotsCount, int ySlotsCount, int xJustify, int yJustify, Vector2 spacing); ///< Helper to create a grid layout node.
CGAPI void      CguiDeleteGridLayoutData(CguiNode *node);                                                            ///< Delete function (attached) for grid layout node.
CGAPI bool      CguiTransformGridLayout(CguiNode *node);                                                             ///< Transform function (attached) for grid layout node.

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

/// Flow layout data.
/// Arrange items in a flow.
typedef struct CguiFlowLayoutData {
    int     direction; ///< Whether the flow direction is x-axis first or y-axis first.
    Vector2 spacing;   ///< Spacing between items of the layout.
} CguiFlowLayoutData;

CGAPI CguiNode *CguiCreateFlowLayout(int direction, Vector2 spacing); ///< Helper to create a flow layout node.
CGAPI bool      CguiTransformFlowLayout(CguiNode *node);              ///< Transform function (attached) for flow layout node.

/// Flow layout item data.
/// Control the flow item.
typedef struct CguiFlowLayoutItemData {
    float   grow;    ///< Weight for growing the item size.
    float   shrink;  ///< Weight for shrinking the item size.
    Vector2 minSize; ///< Minimum size of the item in the layout.
    Vector2 maxSize; ///< Maximum size of the item in the layout.
} CguiFlowLayoutItemData;

CGAPI CguiNode *CguiCreateFlowLayoutItem(float grow, float shrink, Vector2 minSize, Vector2 maxSize); ///< Helper to create a flow layout's item node.

//------------------------------------------------------------------------------
// Mode Nodes
//------------------------------------------------------------------------------

/// Mode node type, 0x00FF01 is the type prefix.
typedef enum CguiModeNodeType {
    CGUI_MODE_NODE_TYPE_SHADER = 0x00FF0100, ///< Shader mode.
    CGUI_MODE_NODE_TYPE_TEXTURE,             ///< Render texture mode.
    CGUI_MODE_NODE_TYPE_CAMERA_2D,           ///< Camera2D mode.
    CGUI_MODE_NODE_TYPE_CAMERA_3D,           ///< Camera3D mode.
    CGUI_MODE_NODE_TYPE_BLEND,               ///< Blending mode.
    CGUI_MODE_NODE_TYPE_SCISSOR,             ///< Scissor mode.
} CguiModeNodeType;

/// A node that calls shader mode begin and end.
typedef struct CguiShaderModeData {
    Shader shader; ///< Shader to use.
} CguiShaderModeData;

CGAPI CguiNode *CguiCreateShaderMode(Shader shader);    ///< Helper to create a shader mode node.
CGAPI void      CguiDrawPreShaderMode(CguiNode *node);  ///< Pre-draw function (attached) for shader mode node.
CGAPI void      CguiDrawPostShaderMode(CguiNode *node); ///< Post-draw function (attached) for shader mode node.

/// A node that calls texture mode begin and end.
typedef struct CguiTextureModeData {
    RenderTexture texture; ///< Texture to draw on.
} CguiTextureModeData;

CGAPI CguiNode *CguiCreateTextureMode(Texture texture);  ///< Helper to create a texture mode node.
CGAPI void      CguiDrawPreTextureMode(CguiNode *node);  ///< Pre-draw function (attached) for texture mode node.
CGAPI void      CguiDrawPostTextureMode(CguiNode *node); ///< Post-draw function (attached) for texture mode node.

/// A node that calls camera mode begin and end.
typedef struct CguiCamera2DModeData {
    Camera2D camera; ///< Camera to use.
} CguiCamera2DModeData;

CGAPI CguiNode *CguiCreateCamera2DMode(Camera2D camera);  ///< Helper to create a camera mode node.
CGAPI void      CguiDrawPreCamera2DMode(CguiNode *node);  ///< Pre-draw function (attached) for camera mode node.
CGAPI void      CguiDrawPostCamera2DMode(CguiNode *node); ///< Post-draw function (attached) for camera mode node.

/// A node that calls camera mode begin and end.
typedef struct CguiCamera3DModeData {
    Camera3D camera; ///< Camera to use.
} CguiCamera3DModeData;

CGAPI CguiNode *CguiCreateCamera3DMode(Camera3D camera);  ///< Helper to create a camera mode node.
CGAPI void      CguiDrawPreCamera3DMode(CguiNode *node);  ///< Pre-draw function (attached) for camera mode node.
CGAPI void      CguiDrawPostCamera3DMode(CguiNode *node); ///< Post-draw function (attached) for camera mode node.

/// A node that calls blend mode begin and end.
typedef struct CguiBlendModeData {
    int mode; ///< Blending mode.
} CguiBlendModeData;

CGAPI CguiNode *CguiCreateBlendMode(int mode);         ///< Helper to create a blend mode node.
CGAPI void      CguiDrawPreBlendMode(CguiNode *node);  ///< Pre-draw function (attached) for blend mode node.
CGAPI void      CguiDrawPostBlendMode(CguiNode *node); ///< Post-draw function (attached) for blend mode node.

/// A node that calls scissor mode begin and end.
typedef struct CguiScissorModeData {
    // ... nothing in here ...
} CguiScissorModeData;

CGAPI CguiNode *CguiCreateScissorMode(void);             ///< Helper to create a scissor mode node.
CGAPI void      CguiDrawPreScissorMode(CguiNode *node);  ///< Pre-draw function (attached) for scissor mode node.
CGAPI void      CguiDrawPostScissorMode(CguiNode *node); ///< Post-draw function (attached) for scissor mode node.

//------------------------------------------------------------------------------
// GUI Basic Element Nodes
//------------------------------------------------------------------------------

/// Basic element type. 0x00FF02 is the type prefix.
typedef enum CguiElementNodeType {
    CGUI_ELEMENT_NODE_TYPE_TEXT = 0x00FF0200, ///< Text element.
    CGUI_ELEMENT_NODE_TYPE_TEXTURE,           ///< Texture element.
    CGUI_ELEMENT_NODE_TYPE_BOX,               ///< Styled rectangle (Box) element.
} CguiElementNodeType;

/// Basic text element data.
typedef struct CguiTextElementData {
    const char *text;           ///< Text to display.
    Font        font;           ///< Font to use.
    float       size;           ///< Size of the text.
    float       spacing;        ///< Spacing between characters.
    float       lineSpacing;    ///< Spacing between lines.
    Color       color;          ///< Color of characters (tint).
    bool        strictWordWrap; ///< Cut off long words for strict word-wrapping.
} CguiTextElementData;

CGAPI CguiNode *CguiCreateTextElement(const char *text, Font font, float size, float spacing, float lineSpacing, Color color, bool strictWordWrap); ///< Helper to create a text element node.
CGAPI void      CguiDrawPreTextElement(CguiNode *node);                                                                                             ///< Pre-draw function (attached) for text element node.

/// Basic texture element data.
typedef struct CguiTextureElementData {
    Texture   texture;  ///< Texture to draw.
    Rectangle source;   ///< Source rectangle for texture.
    Vector2   origin;   ///< Center of rotation for txture (note: origin is added to destination position).
    float     rotation; ///< Rotation of texture in degrees (note: texture may leak outside the node bounds).
    Color     tint;     ///< Tint color for texture.
} CguiTextureElementData;

CGAPI CguiNode *CguiCreateTextureElement(Texture texture, Rectangle source, Vector2 origin, float rotation, Color tint); ///< Helper to create a texture element node.
CGAPI void      CguiDrawPreTextureElement(CguiNode *node);                                                               ///< Pre-draw function (attached) for texture element node.

/// Basic box element data.
typedef struct CguiBoxElementData {
    Vector4 radii;   ///< Radius of 4 corners: top-left, top-right, bottom-left and bottom-right.
    Color   color;   ///< Color of the box.
    Texture texture; ///< Add texture to the box, the color will act as a tint.

    float   shadowDist;    ///< Distance of shadow from the box.
    Vector2 shadowOffset;  ///< Position offset of the shadow from the box.
    float   shadowShrink;  ///< Shrinking of the shadow size from the box.
    Color   shadowColor;   ///< Shadow color.
    Texture shadowTexture; ///< Add texture to the shadow, the color will act as a tint.

    float   borderThickness; ///< Inner-border thickness.
    Color   borderColor;     ///< Color of the border.
    Texture borderTexture;   ///< Add texture the border, the color will act as a tint.
} CguiBoxElementData;

CGAPI CguiNode *CguiCreateBoxElement(Vector4 radii, Color color, Texture texture, float shadowDist, Vector2 shadowOffset, float shadowShrink, Color shadowColor, Texture shadowTexture, float borderThickness, Color borderColor, Texture borderTexture); ///< Helper to create a box element node.
CGAPI void      CguiDrawPreBoxElement(CguiNode *node);                                                                                                                                                                                                    ///< Pre-draw function (attached) for box element node.

//------------------------------------------------------------------------------
// GUI Basic Component Nodes
//------------------------------------------------------------------------------

/// Basic component type. 0x00FF04 is the type prefix.
typedef enum CguiComponentNodeType {
    CGUI_COMPONENT_NODE_TYPE_LABEL = 0x00FF0400, ///< Label component.
    CGUI_COMPONENT_NODE_TYPE_BUTTON,             ///< Button component.
} CguiComponentNodeType;

//------------------------------------------------------------------------------
// Theming Components
//------------------------------------------------------------------------------

/// Theming for components.
typedef struct CguiTheme {
    char *themeName;      ///< Name of the theme.
    void *globalData;     ///< Global data for theme (auto-freed).
    int   globalDataSize; ///< Global data size.

    CguiNode *labelTemplate; ///< Template for labels.
} CguiTheme;

//------------------------------------------------------------------------------
// Core
//------------------------------------------------------------------------------

CGAPI void CguiInit(void);  ///< Initialize Crystal GUI and load resources. Call this before anything Crystal GUI.
CGAPI void CguiClose(void); ///< Unload resources and deinitialize Crystal GUI. Call this after everything Crystal GUI.

#ifdef __cplusplus
}
#endif

#endif // CRYSTALGUI_H
