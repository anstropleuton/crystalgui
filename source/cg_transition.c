/// @file
///
/// @author    Anstro Pleuton
/// @copyright Copyright (c) 2025 Anstro Pleuton
///
/// Crystal GUI - A GUI framework for raylib.
///
/// This source file contains implementations for interpolators and transitions.
///
/// This project is licensed under the terms of MIT license.

#include <math.h>
#include <string.h>

#include "crystalgui/crystalgui.h"
#include "raylib.h"
#include "raymath.h"

struct CguiRegisteredTransitionChain;
typedef struct CguiRegisteredTransitionChain CguiRegisteredTransitionChain;

struct CguiRegisteredTransitionChain {
    CguiTransitionChain           *chain;
    CguiRegisteredTransitionChain *next;
};

extern CguiRegisteredTransitionChain *cguiRegisteredTransitionChains;

int CguiInterpInt(int a, int b, float t)
{
    return (int) (a + (b - a) * t);
}

float CguiInterpFloat(float a, float b, float t)
{
    return a + (b - a) * t;
}

Vector2 CguiInterpVector2(Vector2 a, Vector2 b, float t)
{
    return Vector2Add(a, Vector2Scale(Vector2Subtract(b, a), t));
}

Vector3 CguiInterpVector3(Vector3 a, Vector3 b, float t)
{
    return Vector3Add(a, Vector3Scale(Vector3Subtract(b, a), t));
}

Vector4 CguiInterpVector4(Vector4 a, Vector4 b, float t)
{
    return Vector4Add(a, Vector4Scale(Vector4Subtract(b, a), t));
}

Color CguiInterpColor(Color a, Color b, float t)
{
    // Do clamp t to prevent overflow
    t = Clamp(t, 0.0f, 1.0f);
    return (Color) {
        (unsigned char) (a.r + (b.r - a.r) * t),
        (unsigned char) (a.g + (b.g - a.g) * t),
        (unsigned char) (a.b + (b.b - a.b) * t),
        (unsigned char) (a.a + (b.a - a.a) * t)
    };
}

Rectangle CguiInterpRectangle(Rectangle a, Rectangle b, float t)
{
    Rectangle result;
    result.x = a.x + (b.x - a.x) * t;
    result.y = a.y + (b.y - a.y) * t;

    // Size should not be < 0.0f
    result.width  = fmaxf(a.width + (b.width - a.width) * t, 0.0f);
    result.height = fmaxf(a.height + (b.height - a.height) * t, 0.0f);
    return result;
}

CguiTransformation CguiInterpTransformation(CguiTransformation a, CguiTransformation b, float t)
{
    CguiTransformation result;
    result.position           = CguiInterpVector2(a.position, b.position, t);
    result.size               = Vector2Max(CguiInterpVector2(a.size, b.size, t), Vector2Zero()); // Prevent negative size
    result.isRelativePosition = CguiInterpVector2(a.isRelativePosition, b.isRelativePosition, t);
    result.isRelativeSize     = CguiInterpVector2(a.isRelativeSize, b.isRelativeSize, t);
    result.anchor             = CguiInterpVector2(a.anchor, b.anchor, t);
    result.shrink             = CguiInterpVector2(a.shrink, b.shrink, t);
    return result;
}

CguiTextElementData CguiInterpTextElementData(CguiTextElementData a, CguiTextElementData b, float t)
{
    CguiTextElementData result;
    result.text        = t <= 0.5f ? a.text : b.text;
    result.font        = t <= 0.5f ? a.font : b.font;
    result.fontSize    = CguiInterpFloat(a.fontSize, b.fontSize, t);
    result.spacing     = CguiInterpFloat(a.spacing, b.spacing, t);
    result.lineSpacing = CguiInterpFloat(a.lineSpacing, b.lineSpacing, t);
    result.color       = CguiInterpColor(a.color, b.color, t);
    result.xJustify    = t <= 0.5f ? a.xJustify : b.xJustify;
    result.yJustify    = t <= 0.5f ? a.yJustify : b.yJustify;
    return result;
}

CguiTextureElementData CguiInterpTextureElementData(CguiTextureElementData a, CguiTextureElementData b, float t)
{
    CguiTextureElementData result;
    result.texture  = t <= 0.5f ? a.texture : b.texture;
    result.source   = CguiInterpRectangle(a.source, b.source, t);
    result.origin   = CguiInterpVector2(a.origin, b.origin, t);
    result.rotation = CguiInterpFloat(a.rotation, b.rotation, t);
    result.tint     = CguiInterpColor(a.tint, b.tint, t);
    return result;
}

CguiBoxElementData CguiInterpBoxElementData(CguiBoxElementData a, CguiBoxElementData b, float t)
{
    CguiBoxElementData result;
    result.radii           = CguiInterpVector4(a.radii, b.radii, t);
    result.color           = CguiInterpColor(a.color, b.color, t);
    result.texture         = t <= 0.5f ? a.texture : b.texture;
    result.shadowDistance  = CguiInterpFloat(a.shadowDistance, b.shadowDistance, t);
    result.shadowOffset    = CguiInterpVector2(a.shadowOffset, b.shadowOffset, t);
    result.shadowShrink    = CguiInterpFloat(a.shadowShrink, b.shadowShrink, t);
    result.shadowColor     = CguiInterpColor(a.shadowColor, b.shadowColor, t);
    result.shadowTexture   = t <= 0.5f ? a.shadowTexture : b.shadowTexture;
    result.borderThickness = CguiInterpFloat(a.borderThickness, b.borderThickness, t);
    result.borderColor     = CguiInterpColor(a.borderColor, b.borderColor, t);
    result.borderTexture   = t <= 0.5f ? a.borderTexture : b.borderTexture;
    return result;
}

CguiTransition *CguiCreateTransition(void)
{
    return CguiCreateTransitionPro(NULL, NULL, NULL, NULL, NULL, 0.0f, 0.0f, 0.0f, false, 0);
}

CguiTransition *CguiCreateTransitionEx(const void *from, const void *to, void *result, CguiInterpFunction interp, CguiEasingFunction easing, float duration)
{
    return CguiCreateTransitionPro(from, to, result, interp, easing, 0.0f, duration, 0.0f, false, 0);
}

CguiTransition *CguiCreateTransitionPro(const void *from, const void *to, void *result, CguiInterpFunction interp, CguiEasingFunction easing, float delayBefore, float duration, float delayAfter, bool reversed, int repeatCount)
{
    CguiTransition *transition = CG_MALLOC_NULL(sizeof(CguiTransition));
    if (!transition)
    {
        return NULL;
    }

    transition->from        = from;
    transition->to          = to;
    transition->result      = result;
    transition->interp      = interp;
    transition->easing      = easing;
    transition->delayBefore = delayBefore;
    transition->duration    = duration;
    transition->delayAfter  = delayAfter;
    transition->reversed    = reversed;
    transition->repeatCount = repeatCount;

    return transition;
}

void CguiDeleteTransition(CguiTransition *transition)
{
    if (!transition)
    {
        return;
    }

    CG_FREE_NULL(transition);
}

CguiTransitionChain *CguiCreateTransitionChain(void)
{
    CguiTransitionChain *chain = CG_MALLOC_NULL(sizeof(CguiTransitionChain));
    if (!chain)
    {
        return NULL;
    }

    return chain;
}

void CguiDeleteTransitionChain(CguiTransitionChain *chain)
{
    if (!chain)
    {
        return;
    }

    CguiTransition *current = chain->first;
    while (current)
    {
        CguiTransition *next = current->next;
        CguiDeleteTransition(current);
        current = next;
    }

    CguiDeleteTransitionChainSelf(chain);
}

void CguiDeleteTransitionChainSelf(CguiTransitionChain *chain)
{
    if (!chain)
    {
        return;
    }

    CG_FREE_NULL(chain);
}

bool CguiInsertTransition(CguiTransitionChain *chain, CguiTransition *transition)
{
    if (!chain || !transition)
    {
        return false;
    }

    if (!chain->first || !chain->last)
    {
        chain->first = transition;
        chain->last  = transition;
    }
    else
    {
        chain->last->next = transition;
        transition->prev  = chain->last;
        chain->last       = transition;
    }

    return true;
}

bool CguiInsertTransitionAt(CguiTransitionChain *chain, CguiTransition *transition, int transitionIndex)
{
    if (!chain || !transition || transitionIndex < 0)
    {
        return false;
    }

    if (transitionIndex == 0)
    {
        transition->next = chain->first;
        transition->prev = NULL;
        if (chain->first) chain->first->prev = transition;
        chain->first = transition;
        if (!chain->last) chain->last = transition;
        return true;
    }

    int size = CguiGetTransitionsCount(chain);

    if (transitionIndex > size)
    {
        return false;
    }

    if (transitionIndex == size)
    {
        return CguiInsertTransition(chain, transition);
    }

    CguiTransition *current = CguiGetTransitionAt(chain, transitionIndex);
    if (!current)
    {
        return false;
    }

    transition->prev = current->prev;
    transition->next = current;
    if (current->prev) current->prev->next = transition;
    current->prev = transition;

    return true;
}

bool CguiRemoveTransition(CguiTransitionChain *chain, CguiTransition *transition)
{
    if (!chain || !transition)
    {
        return false;
    }

    if (transition->prev) transition->prev->next = transition->next;
    if (transition->next) transition->next->prev = transition->prev;
    if (chain->first == transition) chain->first = transition->next;
    if (chain->last == transition) chain->last = transition->prev;
    transition->prev = NULL;
    transition->next = NULL;
    return true;
}

bool CguiRemoveTransitionAt(CguiTransitionChain *chain, int transitionIndex)
{
    if (!chain || transitionIndex < 0)
    {
        return false;
    }

    CguiTransition *current = CguiGetTransitionAt(chain, transitionIndex);
    if (!current)
    {
        return false;
    }

    return CguiRemoveTransition(chain, current);
}

CguiTransition *CguiCreateTransitionInChain(CguiTransitionChain *chain)
{
    if (!chain)
    {
        return NULL;
    }

    CguiTransition *transition = CguiCreateTransition();
    if (!transition)
    {
        return NULL;
    }

    if (!CguiInsertTransition(chain, transition))
    {
        CguiDeleteTransition(transition);
        return NULL;
    }

    return transition;
}

CguiTransition *CguiCreateTransitionInChainAt(CguiTransitionChain *chain, int transitionIndex)
{
    if (!chain)
    {
        return NULL;
    }

    CguiTransition *transition = CguiCreateTransition();
    if (!transition)
    {
        return NULL;
    }

    if (!CguiInsertTransitionAt(chain, transition, transitionIndex))
    {
        CguiDeleteTransition(transition);
        return NULL;
    }

    return transition;
}

bool CguiDeleteTransitionInChain(CguiTransitionChain *chain)
{
    if (!chain || !chain->last)
    {
        return false;
    }

    CguiTransition *transition = chain->last;
    if (!CguiRemoveTransition(chain, transition))
    {
        return false;
    }

    CguiDeleteTransition(transition);

    return true;
}

bool CguiDeleteTransitionInChainAt(CguiTransitionChain *chain, int transitionIndex)
{
    if (!chain || !chain->last)
    {
        return false;
    }

    CguiTransition *transition = CguiGetTransitionAt(chain, transitionIndex);
    if (!transition)
    {
        return false;
    }

    if (!CguiRemoveTransition(chain, transition))
    {
        return false;
    }

    CguiDeleteTransition(transition);

    return true;
}

int CguiFindTransition(CguiTransitionChain *chain, CguiTransition *transition)
{
    if (!chain || !transition)
    {
        return -1;
    }

    CguiTransition *current = chain->first;
    int             index   = 0;
    while (current)
    {
        if (current == transition)
        {
            return index;
        }
        current = current->next;
        index++;
    }

    return -1;
}

CguiTransition *CguiGetTransitionAt(CguiTransitionChain *chain, int transitionIndex)
{
    if (!chain || transitionIndex < 0)
    {
        return NULL;
    }

    CguiTransition *current = chain->first;
    int             index   = 0;
    while (current && index < transitionIndex)
    {
        current = current->next;
        index++;
    }

    if (index != transitionIndex)
    {
        return NULL;
    }

    return current;
}

int CguiGetTransitionsCount(CguiTransitionChain *chain)
{
    if (!chain)
    {
        return 0;
    }

    int             count   = 0;
    CguiTransition *current = chain->first;
    while (current)
    {
        count++;
        current = current->next;
    }

    return count;
}

void CguiUpdateTransition(CguiTransition *transition, float elapsedTime)
{
    if (!transition || !transition->interp)
    {
        return;
    }

    float localTime = elapsedTime;

    if (localTime < transition->delayBefore)
    {
        transition->interp(transition->from, transition->to, transition->reversed ? 1.0f : 0.0f, transition->result);
        return;
    }

    localTime -= transition->delayBefore;

    float t = Clamp(transition->duration == 0.0f ? 0.5f : localTime / transition->duration, 0.0f, 1.0f);

    if (transition->easing)
    {
        t = transition->easing(t);
    }

    transition->interp(transition->from, transition->to, t, transition->result);
}

void CguiUpdateTransitionChain(CguiTransitionChain *chain)
{
    if (!chain || chain->paused)
    {
        return;
    }

    if (chain->finished)
    {
        return;
    }

    if (!chain->active)
    {
        chain->active = chain->first;
    }

    CguiTransition *active = chain->active;

    float segmentTime = active->delayBefore + active->duration + active->delayAfter;

    chain->activeTime += 1.0f / 60.0f;

    if (chain->activeTime < segmentTime)
    {
        CguiUpdateTransition(active, chain->activeTime);
        return;
    }
    else
    {
        active->interp(active->from, active->to, active->reversed ? 0.0f : 1.0f, active->result);
    }

    chain->activeTime -= segmentTime;

    if (active->repeatCount == -1 || chain->activeRepeats + 1 < active->repeatCount)
    {
        chain->activeRepeats++;
        // Keep leftover time to allow smoother "continuousness", so it does not accumulate leftover time
        CguiUpdateTransition(active, chain->activeTime);
        return;
    }

    chain->active        = active->next;
    chain->activeRepeats = 0;

    if (!chain->active)
    {
        chain->finished = true;
    }
}

void CguiRegisterAutoTransition(CguiTransition *transition)
{
    if (!transition)
    {
        return;
    }

    CguiRegisteredTransitionChain *newRegister = CG_MALLOC_NULL(sizeof(CguiRegisteredTransitionChain));
    if (!newRegister)
    {
        return;
    }

    newRegister->chain = CguiCreateTransitionChain();
    if (!newRegister->chain)
    {
        return;
    }

    if (!CguiInsertTransition(newRegister->chain, transition))
    {
        CguiDeleteTransitionChain(newRegister->chain);
        return;
    }

    newRegister->next              = cguiRegisteredTransitionChains;
    cguiRegisteredTransitionChains = newRegister;
}

void CguiRegisterAutoTransitionChain(CguiTransitionChain *chain)
{
    if (!chain)
    {
        return;
    }

    CguiRegisteredTransitionChain *newRegister = CG_MALLOC_NULL(sizeof(CguiRegisteredTransitionChain));
    if (!newRegister)
    {
        return;
    }

    newRegister->chain             = chain;
    newRegister->next              = cguiRegisteredTransitionChains;
    cguiRegisteredTransitionChains = newRegister;
}

void CguiUnregisterAutoTransition(CguiTransition *transition)
{
    if (!transition)
    {
        return;
    }

    CguiRegisteredTransitionChain *current  = cguiRegisteredTransitionChains;
    CguiRegisteredTransitionChain *previous = NULL;

    while (current)
    {
        if (current->chain->first == transition)
        {
            if (previous)
            {
                previous->next = current->next;
            }
            else
            {
                cguiRegisteredTransitionChains = current->next;
            }

            CguiRemoveTransition(current->chain, transition);
            CguiDeleteTransitionChain(current->chain);
            CG_FREE_NULL(current);
            break;
        }

        previous = current;
        current  = current->next;
    }
}

void CguiUnregisterAutoTransitionChain(CguiTransitionChain *chain)
{
    if (!chain)
    {
        return;
    }

    CguiRegisteredTransitionChain *current  = cguiRegisteredTransitionChains;
    CguiRegisteredTransitionChain *previous = NULL;

    while (current)
    {
        if (current->chain == chain)
        {
            if (previous)
            {
                previous->next = current->next;
            }
            else
            {
                cguiRegisteredTransitionChains = current->next;
            }

            CG_FREE_NULL(current);
            break;
        }

        previous = current;
        current  = current->next;
    }
}

void CguiUpdateRegisteredTransitions(void)
{
    CguiRegisteredTransitionChain *current = cguiRegisteredTransitionChains;

    while (current)
    {
        if (current->chain)
        {
            CguiUpdateTransitionChain(current->chain);
            if (current->chain->finished)
            {
                CguiDeleteTransitionChain(current->chain);
            }
        }
        current = current->next;
    }
}

void CguiInterpIntP(const int *a, const int *b, float t, int *out)
{
    if (!a || !b || !out)
    {
        return;
    }

    *out = CguiInterpInt(*a, *b, t);
}

void CguiInterpFloatP(const float *a, const float *b, float t, float *out)
{
    if (!a || !b || !out)
    {
        return;
    }

    *out = CguiInterpFloat(*a, *b, t);
}

void CguiInterpVector2P(const Vector2 *a, const Vector2 *b, float t, Vector2 *out)
{
    if (!a || !b || !out)
    {
        return;
    }

    *out = CguiInterpVector2(*a, *b, t);
}

void CguiInterpVector3P(const Vector3 *a, const Vector3 *b, float t, Vector3 *out)
{
    if (!a || !b || !out)
    {
        return;
    }

    *out = CguiInterpVector3(*a, *b, t);
}

void CguiInterpVector4P(const Vector4 *a, const Vector4 *b, float t, Vector4 *out)
{
    if (!a || !b || !out)
    {
        return;
    }

    *out = CguiInterpVector4(*a, *b, t);
}

void CguiInterpColorP(const Color *a, const Color *b, float t, Color *out)
{
    if (!a || !b || !out)
    {
        return;
    }

    *out = CguiInterpColor(*a, *b, t);
}

void CguiInterpRectangleP(const Rectangle *a, const Rectangle *b, float t, Rectangle *out)
{
    if (!a || !b || !out)
    {
        return;
    }

    *out = CguiInterpRectangle(*a, *b, t);
}

void CguiInterpTransformationP(const CguiTransformation *a, const CguiTransformation *b, float t, CguiTransformation *out)
{
    if (!a || !b || !out)
    {
        return;
    }

    *out = CguiInterpTransformation(*a, *b, t);
}

void CguiInterpTextElementDataP(const CguiTextElementData *a, const CguiTextElementData *b, float t, CguiTextElementData *out)
{
    if (!a || !b || !out)
    {
        return;
    }

    *out = CguiInterpTextElementData(*a, *b, t);
}

void CguiInterpTextureElementDataP(const CguiTextureElementData *a, const CguiTextureElementData *b, float t, CguiTextureElementData *out)
{
    if (!a || !b || !out)
    {
        return;
    }

    *out = CguiInterpTextureElementData(*a, *b, t);
}

void CguiInterpBoxElementDataP(const CguiBoxElementData *a, const CguiBoxElementData *b, float t, CguiBoxElementData *out)
{
    if (!a || !b || !out)
    {
        return;
    }

    *out = CguiInterpBoxElementData(*a, *b, t);
}

CguiTransition *CguiTransitInt(const int *a, const int *b, int *result, CguiEasingFunction easing, float duration)
{
    return CguiCreateTransitionEx(a, b, result, (CguiInterpFunction) CguiInterpIntP, easing, duration);
}

CguiTransition *CguiTransitFloat(const float *a, const float *b, float *result, CguiEasingFunction easing, float duration)
{
    return CguiCreateTransitionEx(a, b, result, (CguiInterpFunction) CguiInterpFloatP, easing, duration);
}

CguiTransition *CguiTransitVector2(const Vector2 *a, const Vector2 *b, Vector2 *result, CguiEasingFunction easing, float duration)
{
    return CguiCreateTransitionEx(a, b, result, (CguiInterpFunction) CguiInterpVector2P, easing, duration);
}

CguiTransition *CguiTransitVector3(const Vector3 *a, const Vector3 *b, Vector3 *result, CguiEasingFunction easing, float duration)
{
    return CguiCreateTransitionEx(a, b, result, (CguiInterpFunction) CguiInterpVector3P, easing, duration);
}

CguiTransition *CguiTransitVector4(const Vector4 *a, const Vector4 *b, Vector4 *result, CguiEasingFunction easing, float duration)
{
    return CguiCreateTransitionEx(a, b, result, (CguiInterpFunction) CguiInterpVector4P, easing, duration);
}

CguiTransition *CguiTransitColor(const Color *a, const Color *b, Color *result, CguiEasingFunction easing, float duration)
{
    return CguiCreateTransitionEx(a, b, result, (CguiInterpFunction) CguiInterpColorP, easing, duration);
}

CguiTransition *CguiTransitRectangle(const Rectangle *a, const Rectangle *b, Rectangle *result, CguiEasingFunction easing, float duration)
{
    return CguiCreateTransitionEx(a, b, result, (CguiInterpFunction) CguiInterpRectangleP, easing, duration);
}

CguiTransition *CguiTransitTransformation(const CguiTransformation *a, const CguiTransformation *b, CguiTransformation *result, CguiEasingFunction easing, float duration)
{
    return CguiCreateTransitionEx(a, b, result, (CguiInterpFunction) CguiInterpTransformationP, easing, duration);
}

CguiTransition *CguiTransitTextElementData(const CguiTextElementData *a, const CguiTextElementData *b, CguiTextElementData *result, CguiEasingFunction easing, float duration)
{
    return CguiCreateTransitionEx(a, b, result, (CguiInterpFunction) CguiInterpTextElementDataP, easing, duration);
}

CguiTransition *CguiTransitTextureElementData(const CguiTextureElementData *a, const CguiTextureElementData *b, CguiTextureElementData *result, CguiEasingFunction easing, float duration)
{
    return CguiCreateTransitionEx(a, b, result, (CguiInterpFunction) CguiInterpTextureElementDataP, easing, duration);
}

CguiTransition *CguiTransitBoxElementData(const CguiBoxElementData *a, const CguiBoxElementData *b, CguiBoxElementData *result, CguiEasingFunction easing, float duration)
{
    return CguiCreateTransitionEx(a, b, result, (CguiInterpFunction) CguiInterpBoxElementDataP, easing, duration);
}
