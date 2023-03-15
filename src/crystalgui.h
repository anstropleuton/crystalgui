/*******************************************************************************************
*
*   Crystal GUI - A Crystal-Clear Modern-Looking GUI library for raylib.
*
*   DESCRIPTION:
*
*   Crystal GUI is based on raylib but also available as a standalone library,
*   as long as input and drawing functions are provided. This library also has a
*   built-in support for raygui-based programs, Just rename raylib.h to crystalgui.h!
*
*   WARNING: MORE DESCRIPTION TO BE ADDED.
*   WARNING: RAYGUI SUPPORT IS NOT YET AVAILABLE.
*   WARNING: STANDALONE MODE IS NOT YET AVAILABLE.
*
*   LICENSE: Unmodified MIT License.
*
*   Copyright (c) 2023 Anstro Pleuton
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*   copies of the Software, and to permit persons to whom the Software is
*   furnished to do so, subject to the following conditions:
*
*   The above copyright notice and this permission notice shall be included in all
*   copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*   SOFTWARE.
*
*******************************************************************************************/

#ifndef CGUI_H
#define CGUI_H "crystalgui.h"

#if !defined(CGUI_STANDALONE)
    #include "raylib.h"
#endif

#define CGUI_VERSION "3.2"

// Function specifiers in case library is build/used as a shared library (Windows)
// NOTE: Microsoft specifiers to tell compiler that symbols are imported/exported from a .dll
#if defined(_WIN32)
    #if defined(BUILD_LIBTYPE_SHARED)
        #if defined(__TINYC__)
            #define __declspec(x) __attribute__((x))
        #endif
        #define CGAPI __declspec(dllexport)     // We are building the library as a Win32 shared library (.dll)
    #elif defined(USE_LIBTYPE_SHARED)
        #define CGAPI __declspec(dllimport)     // We are using the library as a Win32 shared library (.dll)
    #endif
#endif

#ifndef CGAPI
    #define CGAPI       // Functions defined as 'extern' by default (implicit specifiers)
#endif

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------

#if !defined(CGUI_MAX_TRACELOG_MSG_LENGTH)
    #define CGUI_MAX_TRACELOG_MSG_LENGTH 128     // Max length of one trace-log message
#endif

#ifndef TRANSITION_SPEED
    #define TRANSITION_SPEED            10.0f
#endif

#ifndef REPEAT_SPEED
    #define REPEAT_SPEED                1.25f
#endif

#ifndef __cplusplus
    // Boolean type
    #if !(defined(false) || defined(true) || defined(bool))
        typedef enum { false, true } bool;
    #endif
#endif

/***********************************************************************************
*
*   LINKED LIST https://github.com/AnstroPleuton/rlList
*
***********************************************************************************/

//----------------------------------------------------------------------------------
// Enumerators Definition
//----------------------------------------------------------------------------------

typedef struct Element {
    void *data;
    void *next;
} Element;

//----------------------------------------------------------------------------------
// List storage data type
//----------------------------------------------------------------------------------

typedef struct List {
    /* The base does not contain an element,
     * the next of the base is the index of 0 */
    Element *base;
    int size;
    int typesize;
} List;

List *CreateList(int typesize);                 // Create a new Linked List, returns NULL if failed
Element *GetElement(int index, List *list);     // Get element from the index, returns NULL if invalid index or failed
int UpdateListSize(List *list);                 // Update the size of a linked list internally, returns non-zero if failed
int GetListSize(List *list);                    // Get the size of a linked list, returns 0 when no elements are available or when failed
Element *CreateElemet(int typesize);            // Create an element, returns NULL if failed. These functions are not supposed to be used externally
int DeleteElement(Element *element);            // Delete an element, returns non-zero if failed. These functions are not supposed to be used externally
Element *AddElement(int index, List *list);     // Add element to a linked list. You can add element at the end of a linked list by giving it the size of a linked list
int RemoveElement(int index, List *list);       // Remove element from the index, returns non-zero if invalid index or failed
int ClearList(List *list);                      // Remove all the elements from a linked list, returns non-zero if faied
int DeleteList(List *list);                     // Delete a Linked List, returns non-zero if faied

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------

#if defined(CGUI_STANDALONE)
#endif

// CguiIcon, icons are textures
typedef struct CguiIcon {
    Texture texture;
    const char *name;
} CguiIcon;

typedef struct FontProp {
    Font font;
    float size;
    float spacing;
    float shadowBlurRadius;
    Vector2 shadowOffset;
    Color shadowColor;
} FontProp;

//----------------------------------------------------------------------------------
// Enumeration definition
//----------------------------------------------------------------------------------

// Gui control state
typedef enum {
    CGUI_STATE_NORMAL = 0,
    CGUI_STATE_FOCUSED,
    CGUI_STATE_PRESSED,
    CGUI_STATE_ACTIVE,
    CGUI_STATE_DISABLED,
} CguiState;

// List of colors
typedef enum {
    CGUI_COLOR_BACKGROUND = 0,
    CGUI_COLOR_FOREGROUND,
    CGUI_COLOR_ACTIVE,
    CGUI_COLOR_UNKNOWN,
    CGUI_COLOR_DANGER,
    CGUI_COLOR_DISABLED,
    CGUI_COLOR_SHADOW,
    CGUI_COLOR_ALL,
} CguiColors;

//----------------------------------------------------------------------------------
// Cgui Function Types
//----------------------------------------------------------------------------------
// It is recommended to not alter the members that start from two underscores (__)

// Cgui button variable
typedef struct CguiButton {
    Rectangle bounds;
    const char *text;
    int __state;
    float __timer;
} CguiButton;

// Cgui drop down button variable
typedef struct CguiDropDownButton {
    CguiButton button;
    List *entries;
    int selectedEntry;
    bool __dropdownActive;
    float __dropDownHeigh;
} CguiDropDownButton;

// Cgui button variable with repeater
typedef struct CguiRepeatButton {
    Rectangle bounds;
    const char *text;
    int __state;
    float __timer;
    float __repeatTimer;
} CguiRepeatButton;

// Cgui button variable with link
typedef struct CguiHyperLinkButton {
    Rectangle bounds;
    const char *text;
    const char *url;
    int __state;
    float __timer;
} CguiHyperLinkButton;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------

#if defined(__cplusplus)
extern "C" {            // Prevents name mangling of functions
#endif

//----------------------------------------------------------------------------------
// Crystal GUI core functions
//----------------------------------------------------------------------------------

CGAPI void CguiLoad(void);                            // Load the Cgui resources (must be called before using other functions)
CGAPI void CguiUnload(void);                          // Unload the Cgui resources (must be called before closing the window)
CGAPI void CguiBeginBackground(void);                 // Begin drawing into the background. To make it blur behind the Cgui!
CGAPI void CguiEndBackground(void);                   // End the drawing, this function will immediately process the blur.
CGAPI void CguiUpdateResolution(void);                // This will update the global variables like resoluion, etc. (Internally called)

CGAPI void CguiNoTraceLog(int logType, const char *text, ...); // TraceLog that doesn't print anything, useful to not log something
CGAPI void CguiTraceLog(const char *text, ...);                // Logger used in Cgui functions
CGAPI Color CguiGetStateColor(int state);                      // Get the color depending on the state and ratio of old to new, ratio goes from 0.0f to 1.0f
CGAPI float CguiClamp(float value, float min, float max);      // Clamp value between min amd max
CGAPI void CguiDrawBackground(void);                           // Draw the contents from the input background (non-blurred)
CGAPI void CguiDrawBlurredBackground(void);                    // Draw the blurred background
CGAPI void CguiDrawRectangle(Rectangle bounds, Color tint, Color shadowColor); // Draw shader processed rectangle
CGAPI void CguiDrawText(const char *text, Rectangle bounds);   // Draw text with drop-down shadow

//----------------------------------------------------------------------------------
// Cgui functions
//----------------------------------------------------------------------------------

CGAPI bool CguiUpdateButton(CguiButton *button);                   // Cgui update button, returns true when clicked
CGAPI void CguiDrawButton(CguiButton *button);                     // Draw Cgui button
CGAPI int CguiUpdateDropDownButton(CguiDropDownButton *ddButton);  // Cgui update drop down button, returns clicked entry
CGAPI void CguiDrawDropDownButton(CguiDropDownButton *ddButton);   // Draw Cgui drop down button
CGAPI bool CguiUpdateRepeatButton(CguiRepeatButton *button);       // Cgui update repeat button, returns true when held
CGAPI void CguiDrawRepeatButton(CguiRepeatButton *button);         // Draw Cgui repeat button
CGAPI bool CguiUpdateHyperLinkButton(CguiHyperLinkButton *button); // Cgui update hyper link button, returns true when clicked and opens url
CGAPI void CguiDrawHyperLinkButton(CguiHyperLinkButton *button);   // Draw Cgui hyper link button

//----------------------------------------------------------------------------------
// Cgui constructors
//----------------------------------------------------------------------------------

CGAPI CguiButton CguiCreateButton(Rectangle bounds, const char *text);             // Create button for easier initialization
CGAPI void CguiDeleteButton(CguiButton *cguiButton);                               // Delete created button
CGAPI CguiDropDownButton CguiCreateDropDownButton(Rectangle bounds, const char *texts[], int textCount, int defaultSelected); // Create drop down button for easier initialization
CGAPI void CguiDeleteDropDownButton(CguiDropDownButton *cguiDropDownButton);       // Delete created drop down button
CGAPI CguiRepeatButton CguiCreateRepeatButton(Rectangle bounds, const char *text); // Create repeat button for easier initialization
CGAPI void CguiDeleteRepeatButton(CguiRepeatButton *cguiRepeatButton);             // Delete created repeat button
CGAPI CguiHyperLinkButton CguiCreateHyperLinkButton(Rectangle bounds, const char *text, const char *url); // Create hyper link button for easier initialization
CGAPI void CguiDeleteHyperLinkButton(CguiHyperLinkButton *cguiHyperLinkButton);    // Delete created hyper link button

//----------------------------------------------------------------------------------
// Theme settings
//----------------------------------------------------------------------------------

CGAPI void CguiSetDarkTheme(void);                    // Set dark theme default colors
CGAPI void CguiSetLightTheme(void);                   // Set light theme default colors

//----------------------------------------------------------------------------------
// set/get functions
//----------------------------------------------------------------------------------

CGAPI void CguiSetDefaultTraceLog(TraceLogCallback callback);
CGAPI TraceLogCallback CguiGetDefaultTraceLog(void);
CGAPI void CguiSetNoTraceLog(TraceLogCallback callback);
CGAPI TraceLogCallback CguiGetNoTraceLog(void);
CGAPI void CguiSetMouseButton(int mouseButton);
CGAPI int CguiGetMouseButton(void);
CGAPI void CguiSetFontProperty(FontProp fontProp);
CGAPI FontProp CguiGetFontProperty(void);
CGAPI void CguiSteBoundarySize(float boundarySize);
CGAPI float CguiGetBoundarySize(void);

CGAPI void CguiSetColor(int colorId, Color color);
CGAPI Color CguiGetColor(int colorId);
CGAPI void CguiSetFocusedFade(Vector3 fade);
CGAPI Vector3 CguiGetFocusedFade(void);
CGAPI void CguiSetPressedFade(Vector3 fade);
CGAPI Vector3 CguiGetPressedFade(void);

//----------------------------------------------------------------------------------
// Shader set/get functions
//----------------------------------------------------------------------------------

CGAPI void CguiSetBlurRadius(float value);
CGAPI float CguiGetBlurRadius(void);
CGAPI void CguiSetBlurQuality(float value);
CGAPI float CguiGetBlurQuality(void);
CGAPI void CguiSetRoundness(float value);
CGAPI float CguiGetRoundness(void);
CGAPI void CguiSetShadowRadius(float value);
CGAPI float CguiGetShadowRadius(void);
CGAPI void CguiSetShadowSize(float value);
CGAPI float CguiGetShadowSize(void);
CGAPI void CguiSetShadowOffset(Vector2 offset);
CGAPI Vector2 CguiGetShadowOffset(void);

#if defined(__cplusplus)
}            // Prevents name mangling of functions
#endif

// RAYGUI SUPPORT IS NOT YET AVAILABLE
// raygui-support mode for raygui-based programs
#if defined(RAYGUI_IMPLEMENTATION)

// This header uses custom implementation of raygui as a compatibility feature
//#undef RAYGUI_IMPLEMENTATION
#include "raygui.h"
#define CGUI_IMPLEMENTATION

/***********************************************************************************
*
*   SOME IMPLEMENTATIONS ARE DIRECTLY TAKEN FROM RAYGUI SO THOSE COPYRIGHT NOTICE ARE INCLUDED
*
*   Copyright (c) 2014-2023 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
***********************************************************************************/

#endif // RAYGUI_IMPLEMENTATION

/***********************************************************************************
*
*   CGUI IMPLEMENTATION
*
***********************************************************************************/

#if defined(CGUI_IMPLEMENTATION)

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// NOTE: MSVC C++ compiler does not support compound literals (C99 feature)
// Plain structures in C++ (without constructors) can be initialized with { }
#ifdef __cplusplus
    #define CGUI_CLITERAL(name) name
#else
    #define CGUI_CLITERAL(name) (name)
#endif

/************************************************************************************
*
*   LINKED LIST IMPLEMENTATION
*
************************************************************************************/

// Create a new Linked List, returns NULL if failed
List *CreateList(int typesize)
{
    List *list = (List *) RL_MALLOC(sizeof(List));
    if (!list) return NULL;

    list->size = 0;
    list->typesize = typesize;
    list->base = (Element *) RL_MALLOC(sizeof(Element));

    if (!list->base) { return NULL; }

    list->base->data = NULL;
    list->base->next = NULL;
    return list;
}

// Get element from the index, returns NULL if invalid index or failed
Element *GetElement(int index, List *list)
{
    Element *element = NULL;
    if (!list) { return NULL; }
    element = list->base;

    // Note, I am not using 'i <= index' because I want
    // the size to overflow in case the index was -1
    for (int i = 0; i < (index + 1); i++)
    {
        element = (Element *) element->next;
        if (!element) { return NULL; }
    }
    return element;
}

// Update the size of a linked list internally, returns non-zero if failed
int UpdateListSize(List *list)
{
    Element *element = NULL;
    int size = 0;

    if (!list) { return 1; }
    element = list->base;

    while (element)
    {
        element = (Element *) element->next;
        size++;
    }

    // The base is not considered as an element
    --size;
    return 0;
}

// Get the size of a linked list, returns 0 when no elements are available or when failed
int GetListSize(List *list)
{
    if (!list) { return 0; }
    if (UpdateListSize(list)) { return 0; }
    return list->size;
}

// Create an element, returns NULL if failed. These functions are not supposed to be used externally
Element *CreateElemet(int typesize)
{
    Element *element = (Element *) RL_MALLOC(sizeof(Element));
    if (!element) { return NULL; }

    element->next = NULL;
    element->data = RL_MALLOC(typesize);
    if (!element->data) { return NULL; }

    return element;
}

// Delete an element, returns non-zero if failed. These functions are not supposed to be used externally
int DeleteElement(Element *element)
{
    if (!element) { return 1; }
    if (!element->data) { return 2; }

    RL_FREE(element->data);
    RL_FREE(element);
    return 0;
}

// Add element to a linked list. You can add element at the end of a linked list by giving it the size of a linked list
Element *AddElement(int index, List *list)
{
    Element *new_element = NULL;
    Element *prev = NULL;
    Element *temp = NULL;

    if (!list) { return NULL; }
    if (index > list->size) { return NULL; }

    prev = GetElement(index - 1, list);
    new_element = CreateElemet(list->typesize);

    if (!new_element) { return NULL; }
    // There can't be no previous because the base should exist
    if (!prev) { return NULL; }

    temp = (Element *) prev->next;
    prev->next = (void *) new_element;
    new_element->next = (void *) temp;

    list->size++;
    return new_element;
}

// Remove element from the index, returns non-zero if invalid index or failed
int RemoveElement(int index, List *list)
{
    Element *element = NULL;
    Element *prev = NULL;
    Element *next = NULL;

    if (!list) { return 1; }
    if (index >= list->size) { return 3; }

    element = GetElement(index, list);
    if (!element) { return 4; }

    prev = GetElement(index - 1, list);

    // To prevent unnecessary error logs, I used an 'if' check
    if (index < list->size - 1) { next = GetElement(index + 1, list); }
    if (!prev) { return 5; }

    prev->next = NULL;
    if (next) { prev->next = next; }

    if (DeleteElement(element)) { return 6; }

    list->size--;
    return 0;
}

// Remove all the elements from a linked list, returns non-zero if faied
int ClearList(List *list)
{
    if (!list) { return 1; }
    if (list->size == 0) { return 0; }

    for (int i = list->size - 1; i >= 0; i--)
        RemoveElement(i, list);
    return 0;
}

// Delete a Linked List, returns non-zero if faied
int DeleteList(List *list)
{
    if (!list) { return 1; }
    if (ClearList(list)) { return 2; }
    if (!list->base) { return 3; }
    RL_FREE(list->base);
    RL_FREE(list);
    return 0;
}

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------

static bool cguiLoaded = false;               // Prevent functions from using unloaded resources
static float cguiScreenResolution[2] = { 0 }; // Screen resolution for shaders and buffer
static CguiIcon cguiIcons[256] = { 0 };       // Each icon texture size is 128x128
static int cguiGlobalState = 0;               // Default Cgui state if this value is 0
static float cguiBoundarySize;                // Boundry spacing size

static TraceLogCallback cguiDefaultTraceLog = NULL; // NULL to log using raylib's TraceLog
static TraceLogCallback cguiNoTraceLog = NULL;      // Prevent logs whenever shader value is set
static int cguiMouseButton = 0;                     // Main mouse button clicks
static FontProp cguiFontProperty = { 0 };           // Many font properties combined
static Color cguiColors[CGUI_COLOR_ALL] = { 0 };    // All the colors used in Cgui

static Vector3 cguiFocusedFade = { 0 };       // Cgui hsv multiplied modification when the mouse is on top of a Cgui component
static Vector3 cguiPressedFade = { 0 };       // Cgui hsv multiplied modification when the Cgui mouse button is pressed

static Shader cguiBlurShader = { 0 };         // Blur the background for cool effects
static Shader cguiShadowShader = { 0 };       // Fancy shadows for buttons
static Shader cguiRectangleShader = { 0 };    // Rounded rectangle

//----------------------------------------------------------------------------------
// Buffer RenderTexture
//----------------------------------------------------------------------------------

static RenderTexture2D cguiInputBackground = { 0 };
static RenderTexture2D cguiBlurredBackground = { 0 };
static RenderTexture2D cguiFontBlurBuffer = { 0 };

//----------------------------------------------------------------------------------
// Shader values
//----------------------------------------------------------------------------------

static float cguiBlurRadius = { 0 };
static float cguiBlurQuality = { 0 };
static float cguiRoundness = { 0 };
static float cguiShadowRadius = { 0 };
static float cguiShadowSize = { 0 };
static float cguiShadowOffset[2] = { 0 };

//----------------------------------------------------------------------------------
// Shader Locations
//----------------------------------------------------------------------------------

static int cguiBlurShaderResolutionLoc = 0;
static int cguiBlurShaderBlurRadiusLoc = 0;

static int cguiShadowShaderRectangleLoc = 0;
static int cguiShadowShaderRoundnessLoc = 0;
static int cguiShadowShaderShadowRadiusLoc = 0;
static int cguiShadowShaderShadowSizeLoc = 0;
static int cguiShadowShaderShadowColorLoc = 0;
static int cguiShadowShaderShadowOffsetLoc = 0;

static int cguiRectangleShaderResolutionLoc = 0;
static int cguiRectangleShaderRectangleLoc = 0;
static int cguiRectangleShaderRoundnessLoc = 0;
static int cguiRectangleShaderRectangleTintLoc = 0;

//----------------------------------------------------------------------------------
// Shader codes
//----------------------------------------------------------------------------------

static char cguiBlurShaderCode[] = ""
    // Note: I am not specifying any version of the shader since I am not certain
    "#ifdef GL_ES\n"
    "precision mediump float;\n"
    "#endif\n"

    "uniform sampler2D texture0;\n"
    "uniform vec2 u_resolution;\n"

    // Custom uniforms
    "uniform float u_blurRadius;\n"
    // Quality is fixed inside the shader so a reload is required to change the quality
    "const float c_blurQuality = %f;\n"

    "void main()\n"
    "{\n"
        // The magic number here is Tau / 4 (or Pi / 2)
        "\tfloat blurRadius = u_blurRadius / c_blurQuality * 1.570796327;\n"
        "\tvec4 finalColor = vec4(0.);\n"

        "\tfor (float a = -c_blurQuality; a <= c_blurQuality; a++)\n"
            "\t\tfor (float b = -c_blurQuality; b <= c_blurQuality; b++)\n"
                "\t\t\tfinalColor += texture2D(texture0, (gl_FragCoord.xy + vec2(a * blurRadius, b * blurRadius)) / u_resolution);\n"
        "\tfinalColor /= 4. * c_blurQuality * c_blurQuality + 4. * c_blurQuality + 1.;\n"

        "\tgl_FragColor = finalColor;\n"
    "}\n";

static char shadowShaderCode[] = ""
    "#ifdef GL_ES\n"
    "precision mediump float;\n"
    "#endif\n"

    // Custom uniforms
    "uniform vec4 u_rectangle;\n"
    "uniform float u_roundness;\n"
    "uniform float u_shadowRadius;\n"
    "uniform float u_shadowSize;\n"
    "uniform vec4 u_shadowColor;\n"
    "uniform vec2 u_shadowOffset;\n"

    // The documentation of how this function works is provided by this link
    // https://iquilezles.org/articles/distfunctions
    "float RBSDF(vec2 centerPosition, vec2 size, float radius)\n"
    "{\n"
        "\tif (min(size.x, size.y) < radius)\n"
            "\t\tradius = min(size.x, size.y);\n"
        "\treturn length(max(abs(centerPosition) - size + radius, 0.)) - radius;\n"
    "}\n"

    "void main()\n"
    "{\n"
        "\tvec2 size = u_rectangle.zw / 2.;\n"
        "\tvec2 position = gl_FragCoord.xy - u_rectangle.xy - size;\n"
        "\tfloat rectangleRBSDF = smoothstep(0.01, 0., RBSDF(position, size, u_roundness));\n"
        "\tfloat shadowRBSDF = 1. + rectangleRBSDF - smoothstep(0., u_shadowRadius, RBSDF(position - u_shadowOffset, size + u_shadowSize, u_roundness));\n"
        "\tgl_FragColor = u_shadowColor * shadowRBSDF;\n"
    "}\n";

static char rectangleShaderCode[] = ""
    "#ifdef GL_ES\n"
    "precision mediump float;\n"
    "#endif\n"

    "uniform sampler2D texture0;\n"
    "uniform vec2 u_resolution;\n"

    "uniform vec4 u_rectangle;\n"
    "uniform float u_roundness;\n"
    "uniform vec4 u_rectangleTint;\n"

    "float RBSDF(vec2 centerPosition, vec2 size, float radius)\n"
    "{\n"
        "\tif (min(size.x, size.y) < radius)\n"
            "\t\tradius = min(size.x, size.y);\n"
        "\treturn length(max(abs(centerPosition) - size + radius, 0.)) - radius;\n"
    "}\n"

    "void main()\n"
    "{\n"
        "\tvec2 size = u_rectangle.zw / 2.;\n"
        "\tvec2 position = gl_FragCoord.xy - u_rectangle.xy - size;\n"
        "\tfloat rectangleRBSDF = smoothstep(0.01, 0., RBSDF(position, size, u_roundness));\n"
        "\tvec4 bgcolor = vec4(texture2D(texture0, gl_FragCoord.xy / u_resolution).rgb * rectangleRBSDF, rectangleRBSDF);\n"
        "\tvec4 tint = vec4(u_rectangleTint.rgb * u_rectangleTint.a, u_rectangleTint.a);\n"
        "\tgl_FragColor = bgcolor * (1. - tint.a) + tint * rectangleRBSDF;\n"
    "}\n";

//----------------------------------------------------------------------------------
// Internal functions
//----------------------------------------------------------------------------------

// Get minimum of two numbers
static int CguiMin(int a, int b)
{
    return (a < b) ? a : b;
}

// Get maximum of two numbers
static int CguiMax(int a, int b)
{
    return (a > b) ? a : b;
}

// Get icon from the text
static int CguiGetIcon(const char *text)
{

}

// Split the characters
// ...

//----------------------------------------------------------------------------------
// Declarations
//----------------------------------------------------------------------------------

// Load the Cgui resources (must be called before using other functions)
void CguiLoad(void)
{
    // Avoid loading resources that are already loaded
    if (cguiLoaded) return;

    // Pre-adjust the resolution
    cguiScreenResolution[0] = (float)GetScreenWidth();
    cguiScreenResolution[1] = (float)GetScreenHeight();

    // Buffer loading
    //------------------------------------------------------------------------------
    cguiInputBackground = LoadRenderTexture((int)cguiScreenResolution[0], (int)cguiScreenResolution[1]);
    cguiBlurredBackground = LoadRenderTexture((int)cguiScreenResolution[0], (int)cguiScreenResolution[1]);
    cguiFontBlurBuffer = LoadRenderTexture((int)cguiScreenResolution[0], (int)cguiScreenResolution[1]);
    //------------------------------------------------------------------------------

    // Shader loading
    //------------------------------------------------------------------------------
    cguiBlurShader = LoadShaderFromMemory(NULL, TextFormat(cguiBlurShaderCode, 2.5f));
    cguiShadowShader = LoadShaderFromMemory(NULL, TextFormat(shadowShaderCode));
    cguiRectangleShader = LoadShaderFromMemory(NULL, TextFormat(rectangleShaderCode));

    // Shader location is set once
    //------------------------------------------------------------------------------
    cguiBlurShaderResolutionLoc = GetShaderLocation(cguiBlurShader, "u_resolution");
    cguiBlurShaderBlurRadiusLoc = GetShaderLocation(cguiBlurShader, "u_blurRadius");

    cguiShadowShaderRectangleLoc = GetShaderLocation(cguiShadowShader, "u_rectangle");
    cguiShadowShaderRoundnessLoc = GetShaderLocation(cguiShadowShader, "u_roundness");
    cguiShadowShaderShadowRadiusLoc = GetShaderLocation(cguiShadowShader, "u_shadowRadius");
    cguiShadowShaderShadowSizeLoc = GetShaderLocation(cguiShadowShader, "u_shadowSize");
    cguiShadowShaderShadowColorLoc = GetShaderLocation(cguiShadowShader, "u_shadowColor");
    cguiShadowShaderShadowOffsetLoc = GetShaderLocation(cguiShadowShader, "u_shadowOffset");

    cguiRectangleShaderResolutionLoc = GetShaderLocation(cguiRectangleShader, "u_resolution");
    cguiRectangleShaderRectangleLoc = GetShaderLocation(cguiRectangleShader, "u_rectangle");
    cguiRectangleShaderRoundnessLoc = GetShaderLocation(cguiRectangleShader, "u_roundness");
    cguiRectangleShaderRectangleTintLoc = GetShaderLocation(cguiRectangleShader, "u_rectangleTint");
    //------------------------------------------------------------------------------

    cguiLoaded = true;

    // Default initialization
    //------------------------------------------------------------------------------
    cguiDefaultTraceLog = NULL;
    cguiNoTraceLog = (TraceLogCallback)CguiNoTraceLog;
    cguiMouseButton = MOUSE_BUTTON_LEFT;
    cguiBoundarySize = 2.0f;

    cguiFontProperty.font = GetFontDefault();
    cguiFontProperty.size = 30.0f;
    cguiFontProperty.spacing = 1.0f;
    cguiFontProperty.shadowBlurRadius = 1.0f;
    cguiFontProperty.shadowOffset = CGUI_CLITERAL(Vector2){ 0.0f, 1.0f };

#if defined(CGUI_DARK_THEME)
    CguiSetDarkTheme();
#else
    CguiSetLightTheme();
#endif
    //------------------------------------------------------------------------------

    // Shader default values
    //------------------------------------------------------------------------------
    CguiSetBlurRadius(10.0f);
    //CguiSetBlurQuality(7.5f); // This reloads the shader so it is not required to do so
    CguiSetRoundness(10.0f);
    CguiSetShadowRadius(30.0f);
    CguiSetShadowSize(-10.0f);
    CguiSetShadowOffset(CGUI_CLITERAL(Vector2){ 0.0f, -10.0f });

    // Update shader resolution
    SetShaderValue(cguiBlurShader, cguiBlurShaderResolutionLoc, cguiScreenResolution, SHADER_UNIFORM_VEC2);
    SetShaderValue(cguiRectangleShader, cguiRectangleShaderResolutionLoc, cguiScreenResolution, SHADER_UNIFORM_VEC2);
    //------------------------------------------------------------------------------
}

// Unload the Cgui resources (must be called before closing the window)
void CguiUnload(void)
{
    // Prevent unloading before loading
    if (!cguiLoaded) return;

    UnloadShader(cguiBlurShader);
    UnloadShader(cguiShadowShader);
    UnloadShader(cguiRectangleShader);
    UnloadRenderTexture(cguiInputBackground);
    UnloadRenderTexture(cguiBlurredBackground);
    UnloadRenderTexture(cguiFontBlurBuffer);

    cguiLoaded = false;
    cguiGlobalState = 0;
    cguiBoundarySize = 0;
    cguiDefaultTraceLog = (TraceLogCallback)NULL;
    cguiNoTraceLog = (TraceLogCallback)NULL;
    cguiMouseButton = 0;
    cguiFontProperty = (FontProp){ 0 };
    cguiFocusedFade = (Vector3){ 0 };
    cguiPressedFade = (Vector3){ 0 };
    cguiBlurShader = (Shader){ 0 };
    cguiShadowShader = (Shader){ 0 };
    cguiRectangleShader = (Shader){ 0 };
    cguiInputBackground = (RenderTexture2D){ 0 };
    cguiBlurredBackground = (RenderTexture2D){ 0 };
    cguiFontBlurBuffer = (RenderTexture2D){ 0 };
    cguiBlurRadius = 0.0f;
    cguiBlurQuality = 0.0f;
    cguiRoundness = 0.0f;
    cguiShadowRadius = 0.0f;
    cguiShadowSize = 0.0f;
    cguiBlurShaderResolutionLoc = 0;
    cguiBlurShaderBlurRadiusLoc = 0;
    cguiShadowShaderRectangleLoc = 0;
    cguiShadowShaderRoundnessLoc = 0;
    cguiShadowShaderShadowRadiusLoc = 0;
    cguiShadowShaderShadowSizeLoc = 0;
    cguiShadowShaderShadowColorLoc = 0;
    cguiShadowShaderShadowOffsetLoc = 0;
    cguiRectangleShaderResolutionLoc = 0;
    cguiRectangleShaderRectangleLoc = 0;
    cguiRectangleShaderRoundnessLoc = 0;
    cguiRectangleShaderRectangleTintLoc = 0;
}

// Begin drawing into the background. To make it blur behind the Cgui!
void CguiBeginBackground(void)
{
    // Prevent function usage if resources are not loaded
    if (!cguiLoaded) return;

    CguiUpdateResolution();
    BeginTextureMode(cguiInputBackground);
    ClearBackground(BLANK);
}

// End the drawing, this function will immediately process the blur.
void CguiEndBackground(void)
{
    // Prevent function usage if resources are not loaded
    if (!cguiLoaded) return;

    EndTextureMode();

    // Blur the background and keep it in cguiBlurredBackground
    //------------------------------------------------------------------------------
    BeginTextureMode(cguiBlurredBackground);
        // Note: We are clearing the buffer
        ClearBackground(BLANK);

        BeginShaderMode(cguiBlurShader);
            DrawTexturePro(cguiInputBackground.texture, CGUI_CLITERAL(Rectangle){ 0.0f, 0.0f, cguiScreenResolution[0], -cguiScreenResolution[1] }, CGUI_CLITERAL(Rectangle){ 0.0f, 0.0f, cguiScreenResolution[0], cguiScreenResolution[1] }, CGUI_CLITERAL(Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
        EndShaderMode();
    EndTextureMode();
    //------------------------------------------------------------------------------
}

// This will update the global variables like resoluion, etc. (Internally called)
void CguiUpdateResolution(void)
{
    // Prevent function usage if resources are not loaded
    if (!cguiLoaded) return;

    if (IsWindowResized())
    {
        // Update resolution
        //--------------------------------------------------------------------------
        cguiScreenResolution[0] = (float)GetScreenWidth();
        cguiScreenResolution[1] = (float)GetScreenHeight();
        //--------------------------------------------------------------------------

        // Reload the background buffers
        //--------------------------------------------------------------------------
        UnloadRenderTexture(cguiInputBackground);
        UnloadRenderTexture(cguiBlurredBackground);
        UnloadRenderTexture(cguiFontBlurBuffer);

        cguiInputBackground = LoadRenderTexture((int)cguiScreenResolution[0], (int)cguiScreenResolution[1]);
        cguiBlurredBackground = LoadRenderTexture((int)cguiScreenResolution[0], (int)cguiScreenResolution[1]);
        cguiFontBlurBuffer = LoadRenderTexture((int)cguiScreenResolution[0], (int)cguiScreenResolution[1]);
        //--------------------------------------------------------------------------

        // Update shader resolution
        //--------------------------------------------------------------------------
        SetShaderValue(cguiBlurShader, cguiBlurShaderResolutionLoc, cguiScreenResolution, SHADER_UNIFORM_VEC2);
        SetShaderValue(cguiRectangleShader, cguiRectangleShaderResolutionLoc, cguiScreenResolution, SHADER_UNIFORM_VEC2);
        //--------------------------------------------------------------------------

        // Clear the background
        //--------------------------------------------------------------------------
        BeginTextureMode(cguiInputBackground);
            ClearBackground(BLANK);
        CguiEndBackground();
        BeginTextureMode(cguiBlurredBackground);
            ClearBackground(BLANK);
        CguiEndBackground();
        BeginTextureMode(cguiFontBlurBuffer);
            ClearBackground(BLANK);
        CguiEndBackground();
        //--------------------------------------------------------------------------
    }
}

// TraceLog that doesn't print anything, useful to not log something
void CguiNoTraceLog(int logType, const char *text, ...)
{
    // Prevent function usage if resources are not loaded
    if (!cguiLoaded) return;

    // Do nothing
}

// Custom logger for no reason at all
void CguiTraceLog(const char *text, ...)
{
    va_list args;
    va_start(args, text);
    char buffer[CGUI_MAX_TRACELOG_MSG_LENGTH] = { 0 };

    strcpy(buffer, "CGUI: ");
    strcat(buffer, text);
    strcat(buffer, "\n");
    vprintf(buffer, args);
    fflush(stdout);

    va_end(args);
}

// Get the color depending on the state and ratio of old to new, ratio goes from 0.0f to 1.0f
Color CguiGetStateColor(int state)
{
    Vector3 hsvColor = ColorToHSV(cguiColors[CGUI_COLOR_BACKGROUND]);

    switch (state)
    {
    case CGUI_STATE_NORMAL:
        return cguiColors[CGUI_COLOR_BACKGROUND];
    case CGUI_STATE_FOCUSED:
        return ColorAlpha(ColorFromHSV(hsvColor.x + cguiFocusedFade.x, hsvColor.y + cguiFocusedFade.y, hsvColor.z + cguiFocusedFade.z), cguiColors[CGUI_COLOR_BACKGROUND].a / 255.0f);
    case CGUI_STATE_PRESSED:
        return ColorAlpha(ColorFromHSV(hsvColor.x + cguiPressedFade.x, hsvColor.y + cguiPressedFade.y, hsvColor.z + cguiPressedFade.z), cguiColors[CGUI_COLOR_BACKGROUND].a / 255.0f);
    case CGUI_STATE_ACTIVE:
        return cguiColors[CGUI_COLOR_FOREGROUND];
    case CGUI_STATE_DISABLED:
        return cguiColors[CGUI_COLOR_DISABLED];
    }
}

// Clamp value between min amd max
float CguiClamp(float value, float min, float max)
{
    if (min > max)
    {
        float tmp = max;
        max = min;
        min = tmp;
    }

    return fmaxf(fminf(value, max), min);
}

// Draw the contents from the input background (non-blurred)
void CguiDrawBackground(void)
{
    DrawTexturePro(cguiInputBackground.texture, CGUI_CLITERAL(Rectangle){ 0.0f, 0.0f, cguiScreenResolution[0], -cguiScreenResolution[1] }, CGUI_CLITERAL(Rectangle){ 0.0f, 0.0f, cguiScreenResolution[0], cguiScreenResolution[1] }, CGUI_CLITERAL(Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
}

// Draw the blurred background
void CguiDrawBlurredBackground(void)
{
    DrawTexturePro(cguiBlurredBackground.texture, CGUI_CLITERAL(Rectangle){ 0.0f, 0.0f, cguiScreenResolution[0], -cguiScreenResolution[1] }, CGUI_CLITERAL(Rectangle){ 0.0f, 0.0f, cguiScreenResolution[0], cguiScreenResolution[1] }, CGUI_CLITERAL(Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
}

// Draw shader processed rectangle
void CguiDrawRectangle(Rectangle bounds, Color tint, Color shadowColor)
{
    // Prevent function usage if resources are not loaded
    if (!cguiLoaded) return;

    float rectangle[4] = { rectangle[0] = bounds.x, rectangle[1] = GetScreenHeight() - bounds.y - bounds.height, rectangle[2] = bounds.width, rectangle[3] = bounds.height };
    float color[4] = { tint.r / 255.0f, tint.g / 255.0f, tint.b / 255.0f, tint.a / 255.0f };
    float shaderColor[4] = { shadowColor.r / 255.0f, shadowColor.g / 255.0f, shadowColor.b / 255.0f, shadowColor.a / 255.0f };

    // Apply the rectangle
    SetShaderValue(cguiShadowShader, cguiShadowShaderRectangleLoc, rectangle, SHADER_UNIFORM_VEC4);
    SetShaderValue(cguiRectangleShader, cguiRectangleShaderRectangleLoc, rectangle, SHADER_UNIFORM_VEC4);

    // Apply the color
    SetShaderValue(cguiShadowShader, cguiShadowShaderShadowColorLoc, shaderColor, SHADER_UNIFORM_VEC4);
    SetShaderValue(cguiRectangleShader, cguiRectangleShaderRectangleTintLoc, color, SHADER_UNIFORM_VEC4);

    // Apply and draw shadow shader and rectangle shader
    //------------------------------------------------------------------------------
    BeginShaderMode(cguiShadowShader);
        DrawRectangleRec(CGUI_CLITERAL(Rectangle){ 0.0f, 0.0f, cguiScreenResolution[0], cguiScreenResolution[0] }, BLANK);
    EndShaderMode();
    BeginShaderMode(cguiRectangleShader);
        DrawTexturePro(cguiBlurredBackground.texture, CGUI_CLITERAL(Rectangle){ 0.0f, 0.0f, cguiScreenResolution[0], -cguiScreenResolution[1] }, CGUI_CLITERAL(Rectangle){ 0.0f, 0.0f, cguiScreenResolution[0], cguiScreenResolution[1] }, CGUI_CLITERAL(Vector2){ 0.0f, 0.0f }, 0.0f, BLANK);
    EndShaderMode();
    //------------------------------------------------------------------------------
}

// Draw text with drop-down shadow
void CguiDrawText(const char *text, Rectangle bounds)
{
    // Prevent function usage if resources are not loaded
    if (!cguiLoaded) return;

    // Local Variables
    Vector2 textSize = MeasureTextEx(cguiFontProperty.font, text, cguiFontProperty.size, cguiFontProperty.spacing);
    Vector2 textPosition = { bounds.x + (bounds.width - textSize.x) / 2.0f, bounds.y + (bounds.height - textSize.y) / 2.0f };
    Vector2 textShadowPosition = { textPosition.x + cguiFontProperty.shadowOffset.x, textPosition.y + cguiFontProperty.shadowOffset.y };
    float shaderBlurRadius = cguiBlurRadius;

    // Put font in blur buffer
    BeginTextureMode(cguiFontBlurBuffer);
        // Erase contents before use
        ClearBackground(BLANK);
        DrawTextEx(cguiFontProperty.font, text, textShadowPosition, cguiFontProperty.size, cguiFontProperty.spacing, cguiFontProperty.shadowColor);
    EndTextureMode();

    // Temporarily set font blur radius and restore afterwards
    CguiSetBlurRadius(cguiFontProperty.shadowBlurRadius);

    // Blur and draw font as shadow
    BeginShaderMode(cguiBlurShader);
        DrawTexturePro(cguiFontBlurBuffer.texture, CGUI_CLITERAL(Rectangle){ 0.0f, 0.0f, cguiScreenResolution[0], -cguiScreenResolution[1] }, CGUI_CLITERAL(Rectangle){ 0.0f, 0.0f, cguiScreenResolution[0], cguiScreenResolution[1] }, CGUI_CLITERAL(Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
    EndShaderMode();
    CguiSetBlurRadius(shaderBlurRadius);

    // Draw font regularly
    DrawTextEx(cguiFontProperty.font, text, textPosition, cguiFontProperty.size, cguiFontProperty.spacing, cguiColors[CGUI_COLOR_FOREGROUND]);
}

//----------------------------------------------------------------------------------
// Cgui functions
//----------------------------------------------------------------------------------

// Update Cgui button, returns true when clicked
bool CguiUpdateButton(CguiButton *button)
{
    // Prevent function usage if resources are not loaded
    if (!cguiLoaded) return false;

    Color color;
    bool result = false;

    // Update state, do not update state if Global state is not set
    if (button->__state != CGUI_STATE_DISABLED && cguiGlobalState == 0)
    {
        button->__state = CGUI_STATE_NORMAL;

        // Mouse is on top of button
        if (CheckCollisionPointRec(GetMousePosition(), button->bounds))
        {
            // Update the timer for shadow transition
            button->__timer = CguiClamp(button->__timer + TRANSITION_SPEED * GetFrameTime(), 0.0f, 1.0f);

            button->__state = CGUI_STATE_FOCUSED;
            if (IsMouseButtonDown(cguiMouseButton)) button->__state = CGUI_STATE_PRESSED;
            if (IsMouseButtonReleased(cguiMouseButton)) result = true;
        }
        else button->__timer = CguiClamp(button->__timer - TRANSITION_SPEED * GetFrameTime(), 0.0f, 1.0f);
    }
    else button->__state = cguiGlobalState - 1;
    return result;
}

// Draw Cgui button
void CguiDrawButton(CguiButton *button)
{
    // Local Variables
    Color color = CguiGetStateColor(button->__state);
    Color shadowColor = { cguiColors[CGUI_COLOR_SHADOW].r, cguiColors[CGUI_COLOR_SHADOW].g, cguiColors[CGUI_COLOR_SHADOW].b, cguiColors[CGUI_COLOR_SHADOW].a * button->__timer };

    // Simple draw
    CguiDrawRectangle(button->bounds, color, shadowColor);
    CguiDrawText(button->text, button->bounds);
}

// Update Cgui drop down button, returns clicked entry
int CguiUpdateDropDownButton(CguiDropDownButton *ddButton)
{
    // Prevent function usage if resources are not loaded
    if (!cguiLoaded) return -3;

    // Update internal button
    if (CguiUpdateButton(&ddButton->button)) ddButton->__dropdownActive = !ddButton->__dropdownActive;

    // Update opening timer transition
    ddButton->__dropDownHeigh = CguiClamp(ddButton->__dropDownHeigh + TRANSITION_SPEED * GetFrameTime() * (ddButton->__dropdownActive ? 1.0f : -1.0f), 0.0f, 1.0f);

    // Local Variables
    int listSize = GetListSize(ddButton->entries);
    int resultEntry = -1;
    Rectangle dropDownBounds = (Rectangle){ ddButton->button.bounds.x, ddButton->button.bounds.y + ddButton->button.bounds.height, ddButton->button.bounds.width, ddButton->button.bounds.height * listSize * ddButton->__dropDownHeigh };
    CguiButton *button;

    // Select option when scrolled on Cgui
    if (CheckCollisionPointRec(GetMousePosition(), ddButton->button.bounds) || CheckCollisionPointRec(GetMousePosition(), dropDownBounds)) ddButton->selectedEntry -= GetMouseWheelMove();

    // Limit seleced entry count
    ddButton->selectedEntry = CguiClamp(ddButton->selectedEntry, 0, listSize - 1);

    // Update drop down buttons
    //------------------------------------------------------------------------------
    for (int i = 0; i < listSize; i++)
    {
        // Get and re-position the drop down button component
        button = (CguiButton *)GetElement(i, ddButton->entries)->data;
        button->bounds = (Rectangle){ ddButton->button.bounds.x + cguiBoundarySize, ddButton->button.bounds.y + ddButton->button.bounds.height * (i + 1) + cguiBoundarySize, ddButton->button.bounds.width - cguiBoundarySize * 2.0f, ddButton->button.bounds.height - cguiBoundarySize * 2.0f };

        // Close drop down when drop down button is clicked and only when the mouse is on drop down area
        if (CguiUpdateButton(button) && CheckCollisionPointRec(GetMousePosition(), dropDownBounds))
        {
            ddButton->selectedEntry = i;
            ddButton->__dropdownActive = false;
            resultEntry = i;
        }
    }
    //------------------------------------------------------------------------------
    if (ddButton->__dropDownHeigh) return -2;
    return resultEntry;
}

// Draw Cgui drop down button
void CguiDrawDropDownButton(CguiDropDownButton *ddButton)
{
    // Prevent function usage if resources are not loaded
    if (!cguiLoaded) return;

    // Shadow color alpha depends on timer
    int listSize = GetListSize(ddButton->entries);
    Color shadowColor = { cguiColors[CGUI_COLOR_SHADOW].r, cguiColors[CGUI_COLOR_SHADOW].g, cguiColors[CGUI_COLOR_SHADOW].b, cguiColors[CGUI_COLOR_SHADOW].a * ddButton->button.__timer };
    Rectangle dropDownBounds = { ddButton->button.bounds.x, ddButton->button.bounds.y + ddButton->button.bounds.height, ddButton->button.bounds.width, ddButton->button.bounds.height * listSize * ddButton->__dropDownHeigh };
    Color shadowShaderColor = cguiColors[CGUI_COLOR_SHADOW];
    Color backgroundColor = cguiColors[CGUI_COLOR_BACKGROUND];
    Element *element = GetElement(ddButton->selectedEntry, ddButton->entries);

    // Limit seleced entry count
    ddButton->selectedEntry = CguiClamp(ddButton->selectedEntry, 0, listSize - 1);

    // Draw Button
    if (element && listSize != 0) ddButton->button.text = (*(CguiButton *)element->data).text;
    else ddButton->button.text = "";
    CguiDrawButton(&ddButton->button);

    // Main drop down background
    if (ddButton->__dropDownHeigh > 0.0f) CguiDrawRectangle(dropDownBounds, cguiColors[CGUI_COLOR_BACKGROUND], cguiColors[CGUI_COLOR_SHADOW]);

    // No shadows on drop down button
    cguiColors[CGUI_COLOR_SHADOW] = (Color){ 0.0f, 0.0f, 0.0f, 0.0f };

    // Limit cgui draws only on drop down background
    BeginScissorMode(dropDownBounds.x, dropDownBounds.y, dropDownBounds.width, dropDownBounds.height);
        // Draw drop down buttons by reusing CguiDrawButton
        for (int i = 0; i < listSize; i++)
        {
            // Change background color for selected item
            if (i == ddButton->selectedEntry) cguiColors[CGUI_COLOR_BACKGROUND] = CguiGetStateColor(CGUI_STATE_FOCUSED);

            CguiDrawButton((CguiButton *)GetElement(i, ddButton->entries)->data);
            cguiColors[CGUI_COLOR_BACKGROUND] = backgroundColor;
        }
    EndScissorMode();

    // Reset the default shadow color
    cguiColors[CGUI_COLOR_SHADOW] = shadowShaderColor;
}

// Cgui update repeat button, returns true when held
bool CguiUpdateRepeatButton(CguiRepeatButton *button)
{
    // Prevent function usage if resources are not loaded
    if (!cguiLoaded) return false;

    Color color;
    bool result = false;

    // Update state, do not update state if Global state is not set
    if (button->__state != CGUI_STATE_DISABLED && cguiGlobalState == 0)
    {
        button->__state = CGUI_STATE_NORMAL;

        // Mouse is on top of button
        if (CheckCollisionPointRec(GetMousePosition(), button->bounds))
        {
            // Update the timer for shadow transition
            button->__timer = CguiClamp(button->__timer + TRANSITION_SPEED * GetFrameTime(), 0.0f, 1.0f);

            button->__state = CGUI_STATE_FOCUSED;
            if (IsMouseButtonDown(cguiMouseButton))
            {
                button->__state = CGUI_STATE_PRESSED;
                button->__repeatTimer = CguiClamp(button->__repeatTimer + REPEAT_SPEED * GetFrameTime() * 2.0f, 0.0f, 1.0f);
            }
            if (IsMouseButtonPressed(cguiMouseButton)) result = true;
        }
        else button->__timer = CguiClamp(button->__timer - TRANSITION_SPEED * GetFrameTime(), 0.0f, 1.0f);

        // If mouse button is not pressed, reset the timer
        if (!IsMouseButtonDown(cguiMouseButton)) button->__repeatTimer = 0.0f;

        // Set the result as condition of repeat timer
        if (!result) result = (button->__repeatTimer == 1.0f);
    }
    else button->__state = cguiGlobalState - 1;
    return result;
}

// Draw Cgui repeat button
void CguiDrawRepeatButton(CguiRepeatButton *button)
{
    CguiButton cguiButton = { button->bounds, button->text, button->__state, button->__timer };
    CguiDrawButton(&cguiButton);
}

// Cgui update hyper link button, returns true when clicked and opens url
bool CguiUpdateHyperLinkButton(CguiHyperLinkButton *button)
{
    CguiButton cguiButton = { button->bounds, button->text, button->__state, button->__timer };
    
    // Use existing function
    int result = CguiUpdateButton(&cguiButton);

    button->__state = cguiButton.__state;
    button->__timer = cguiButton.__timer;

    // Open url from button
    if (result) OpenURL(button->url);

    return result;
}

// Draw Cgui hyper link button
void CguiDrawHyperLinkButton(CguiHyperLinkButton *button)
{
    CguiButton cguiButton = { button->bounds, button->text, button->__state, button->__timer };

    Color foregroundColor = cguiColors[CGUI_COLOR_FOREGROUND];

    // temporarily make foreground color as active color for font
    cguiColors[CGUI_COLOR_FOREGROUND] = cguiColors[CGUI_COLOR_ACTIVE];

    // Use existing function
    CguiDrawButton(&cguiButton);

    // Reset color back
    cguiColors[CGUI_COLOR_FOREGROUND] = foregroundColor;
}

//----------------------------------------------------------------------------------
// Cgui constructors
//----------------------------------------------------------------------------------

// Create button for easier initialization
CguiButton CguiCreateButton(Rectangle bounds, const char *text)
{
    CguiButton button = { bounds, text };
    button.__state = 0;
    button.__timer = 0.0f;
    return button;
}

// Delete created button
void CguiDeleteButton(CguiButton *cguiButton)
{
    cguiButton->bounds = (Rectangle){ 0.0f, 0.0f, 0.0f, 0.0f };
    cguiButton->text = NULL;
    cguiButton->__state = 0;
    cguiButton->__timer = 0.0f;
}

// Create drop down button for easier initialization
CguiDropDownButton CguiCreateDropDownButton(Rectangle bounds, const char *texts[], int textCount, int defaultSelected)
{
    CguiDropDownButton cguiDropDownButton;
    CguiButton cguiButton = CguiCreateButton(bounds, "");
    List *entries = CreateList(sizeof(CguiButton));
    for (int i = 0; i < textCount; i++)
        *(CguiButton *)AddElement(i, entries)->data = CguiCreateButton((Rectangle){ 0.0f, 0.0f, 0.0f, 0.0f }, texts[i]);

    cguiDropDownButton.entries = entries;
    cguiDropDownButton.button = cguiButton;
    cguiDropDownButton.selectedEntry = defaultSelected;
    cguiDropDownButton.__dropdownActive = false;
    cguiDropDownButton.__dropDownHeigh = 0.0f;
    return cguiDropDownButton;
}

// Delete created drop down button
void CguiDeleteDropDownButton(CguiDropDownButton *cguiDropDownButton)
{
    // The list is automatically cleared
    DeleteList(cguiDropDownButton->entries);
    cguiDropDownButton->entries = NULL;
    CguiDeleteButton(&cguiDropDownButton->button);
    cguiDropDownButton->selectedEntry = 0;
    cguiDropDownButton->__dropdownActive = false;
    cguiDropDownButton->__dropDownHeigh = 0.0f;
}

// Create repeat button for easier initialization
CguiRepeatButton CguiCreateRepeatButton(Rectangle bounds, const char *text)
{
    CguiRepeatButton button = { bounds, text };
    button.__state = 0;
    button.__timer = 0.0f;
    button.__repeatTimer = 0.0f;
    return button;
}

// Delete created repeat button
void CguiDeleteRepeatButton(CguiRepeatButton *cguiRepeatButton)
{
    cguiRepeatButton->bounds = (Rectangle){ 0.0f, 0.0f, 0.0f, 0.0f };
    cguiRepeatButton->text = NULL;
    cguiRepeatButton->__state = 0;
    cguiRepeatButton->__timer = 0.0f;
    cguiRepeatButton->__repeatTimer = 0.0f;
}

// Create hyper link button for easier initialization
CguiHyperLinkButton CguiCreateHyperLinkButton(Rectangle bounds, const char *text, const char *url)
{
    CguiHyperLinkButton button = { bounds, text, url };
    button.__state = 0;
    button.__timer = 0.0f;
    return button;
}

// Delete created hyper link button
void CguiDeleteHyperLinkButton(CguiHyperLinkButton *cguiHyperLinkButton)
{
    cguiHyperLinkButton->bounds = (Rectangle){ 0.0f, 0.0f, 0.0f, 0.0f };
    cguiHyperLinkButton->text = NULL;
    cguiHyperLinkButton->url = NULL;
    cguiHyperLinkButton->__state = 0;
    cguiHyperLinkButton->__timer = 0.0f;
}

//----------------------------------------------------------------------------------
// Theme settings
//----------------------------------------------------------------------------------

// Set dark theme default colors
void CguiSetDarkTheme(void)
{
    cguiFontProperty.shadowColor = CGUI_CLITERAL(Color){ 0, 0, 0, 255 };
    cguiColors[CGUI_COLOR_BACKGROUND] = CGUI_CLITERAL(Color){ 51, 51, 51, 192 };
    cguiColors[CGUI_COLOR_FOREGROUND] = CGUI_CLITERAL(Color){ 255, 255, 255, 255 };
    cguiColors[CGUI_COLOR_ACTIVE] = CGUI_CLITERAL(Color){ 0, 170, 255, 255 };
    cguiColors[CGUI_COLOR_UNKNOWN] = CGUI_CLITERAL(Color){ 255, 255, 0, 255 };
    cguiColors[CGUI_COLOR_DANGER] = CGUI_CLITERAL(Color){ 255, 0, 0, 255 };
    cguiColors[CGUI_COLOR_DISABLED] = CGUI_CLITERAL(Color){ 127, 127, 127, 255 };
    cguiColors[CGUI_COLOR_SHADOW] = CGUI_CLITERAL(Color){ 0, 0, 0, 127 };
    cguiFocusedFade = CGUI_CLITERAL(Vector3){ 0.0f, 0.0f, 0.05f };
    cguiPressedFade = CGUI_CLITERAL(Vector3){ 0.0f, 0.0f, -0.05f };
}

// Set light theme default colors
void CguiSetLightTheme(void)
{
    cguiFontProperty.shadowColor = CGUI_CLITERAL(Color){ 0, 0, 0, 127 };
    cguiColors[CGUI_COLOR_BACKGROUND] = CGUI_CLITERAL(Color){ 255, 255, 255, 192 };
    cguiColors[CGUI_COLOR_FOREGROUND] = CGUI_CLITERAL(Color){ 51, 51, 51, 255 };
    cguiColors[CGUI_COLOR_ACTIVE] = CGUI_CLITERAL(Color){ 0, 170, 255, 255 };
    cguiColors[CGUI_COLOR_UNKNOWN] = CGUI_CLITERAL(Color){ 255, 255, 0, 255 };
    cguiColors[CGUI_COLOR_DANGER] = CGUI_CLITERAL(Color){ 255, 0, 0, 255 };
    cguiColors[CGUI_COLOR_DISABLED] = CGUI_CLITERAL(Color){ 127, 127, 127, 255 };
    cguiColors[CGUI_COLOR_SHADOW] = CGUI_CLITERAL(Color){ 51, 51, 51, 127 };
    cguiFocusedFade = CGUI_CLITERAL(Vector3){ 0.0f, 0.0f, -0.1f };
    cguiPressedFade = CGUI_CLITERAL(Vector3){ 0.0f, 0.0f, -0.2f };
}

//----------------------------------------------------------------------------------
// set/get functions
//----------------------------------------------------------------------------------

void CguiSetDefaultTraceLog(TraceLogCallback callback)
{
    cguiDefaultTraceLog = callback;
}

TraceLogCallback CguiGetDefaultTraceLog(void)
{
    return cguiDefaultTraceLog;
}

void CguiSetNoTraceLog(TraceLogCallback callback)
{
    cguiNoTraceLog = callback;
}

TraceLogCallback CguiGetNoTraceLog(void)
{
    return cguiNoTraceLog;
}

void CguiSetMouseButton(int mouseButton)
{
    cguiMouseButton = mouseButton;
}

int CguiGetMouseButton(void)
{
    return cguiMouseButton;
}

void CguiSetFontProperty(FontProp fontProp)
{
    cguiFontProperty = fontProp;
}

FontProp CguiGetFontProperty(void)
{
    return cguiFontProperty;
}

void CguiSteBoundarySize(float boundarySize)
{
    cguiBoundarySize = boundarySize;
}

float CguiGetBoundarySize(void)
{
    return cguiBoundarySize;
}

void CguiSetColor(int colorId, Color color)
{
    if (colorId < 0 || colorId >= CGUI_COLOR_ALL) return;
    cguiColors[colorId] = color;
}

Color CguiGetColor(int colorId)
{
    if (colorId < 0 || colorId >= CGUI_COLOR_ALL) return (Color){ 0, 0, 0, 0 };
    return cguiColors[colorId];
}

void CguiSetFocusedFade(Vector3 fade)
{
    cguiFocusedFade = fade;
}

Vector3 CguiGetFocusedFade(void)
{
    return cguiFocusedFade;
}

void CguiSetPressedFade(Vector3 fade)
{
    cguiPressedFade = fade;
}

Vector3 CguiGetPressedFade(void)
{
    return cguiPressedFade;
}

//----------------------------------------------------------------------------------
// Shader getters and setters (not always is simple)
//----------------------------------------------------------------------------------

void CguiSetBlurRadius(float value)
{
    if (!cguiLoaded) return;
    cguiBlurRadius = value;
    SetShaderValue(cguiBlurShader, cguiBlurShaderBlurRadiusLoc, &cguiBlurRadius, SHADER_UNIFORM_FLOAT);
}

float CguiGetBlurRadius(void)
{
    return cguiBlurRadius;
}

void CguiSetBlurQuality(float value)
{
    if (!cguiLoaded) return;
    cguiBlurQuality = value;

    // Requires reload of shader because quality is a constant
    UnloadShader(cguiBlurShader);
    cguiBlurShader = LoadShaderFromMemory(NULL, TextFormat(cguiBlurShaderCode, cguiBlurQuality));
    SetShaderValue(cguiBlurShader, cguiBlurShaderResolutionLoc, cguiScreenResolution, SHADER_UNIFORM_VEC2);

    // Reset the other uniforms
    CguiSetBlurRadius(cguiBlurRadius);
}

float CguiGetBlurQuality(void)
{
    return cguiBlurQuality;
}

void CguiSetRoundness(float value)
{
    if (!cguiLoaded) return;
    cguiRoundness = value;
    SetShaderValue(cguiShadowShader, cguiShadowShaderRoundnessLoc, &cguiRoundness, SHADER_UNIFORM_FLOAT);
    SetShaderValue(cguiRectangleShader, cguiRectangleShaderRoundnessLoc, &cguiRoundness, SHADER_UNIFORM_FLOAT);
}

float CguiGetRoundness(void)
{
    return cguiRoundness;
}

void CguiSetShadowRadius(float value)
{
    if (!cguiLoaded) return;
    cguiShadowRadius = value;
    SetShaderValue(cguiShadowShader, cguiShadowShaderShadowRadiusLoc, &cguiShadowRadius, SHADER_UNIFORM_FLOAT);
}

float CguiGetShadowRadius(void)
{
    return cguiShadowRadius;
}

void CguiSetShadowSize(float value)
{
    if (!cguiLoaded) return;
    cguiShadowSize = value;
    SetShaderValue(cguiShadowShader, cguiShadowShaderShadowSizeLoc, &cguiShadowSize, SHADER_UNIFORM_FLOAT);
}

float CguiGetShadowSize(void)
{
    return cguiShadowSize;
}

void CguiSetShadowOffset(Vector2 offset)
{
    if (!cguiLoaded) return;
    cguiShadowOffset[0] = offset.x;
    cguiShadowOffset[1] = offset.y;
    SetShaderValue(cguiShadowShader, cguiShadowShaderShadowOffsetLoc, cguiShadowOffset, SHADER_UNIFORM_VEC2);
}

Vector2 CguiGetShadowOffset(void)
{
    return CGUI_CLITERAL(Vector2){ cguiShadowOffset[0], cguiShadowOffset[1] };
}

#endif // CGUI_IMPLEMENTATION

#endif // CGUI_H
