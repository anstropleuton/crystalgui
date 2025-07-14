/// @file
///
/// @author    Anstro Pleuton
/// @copyright Copyright (c) 2025 Anstro Pleuton
///
/// Crystal GUI - A modern GUI framework for raylib.
///
/// This source file contains implementations for easing functions of
/// Crystal GUI.
///
/// This project is licensed under the terms of MIT license.

#include <math.h>

#include "crystalgui.h"

float CguiEaseLinear(float t)
{
    return t;
}

float CguiEaseIn(float t, float power)
{
    return powf(t, power);
}

float CguiEaseOut(float t, float power)
{
    return 1 - powf(1 - t, power);
}

float CguiEaseInOut(float t, float power)
{
    if (t < .5)
        return powf(2 * t, power) / 2;
    else
        return 1 - powf(2 * (1 - t), power) / 2;
}

float CguiEaseInQuad(float t)
{
    return CguiEaseIn(t, 2);
}

float CguiEaseOutQuad(float t)
{
    return CguiEaseOut(t, 2);
}

float CguiEaseInOutQuad(float t)
{
    return CguiEaseInOut(t, 2);
}

float CguiEaseInCubic(float t)
{
    return CguiEaseIn(t, 3);
}

float CguiEaseOutCubic(float t)
{
    return CguiEaseOut(t, 3);
}

float CguiEaseInOutCubic(float t)
{
    return CguiEaseInOut(t, 3);
}

float CguiEaseInQuart(float t)
{
    return CguiEaseIn(t, 4);
}

float CguiEaseOutQuart(float t)
{
    return CguiEaseOut(t, 4);
}

float CguiEaseInOutQuart(float t)
{
    return CguiEaseInOut(t, 4);
}

float CguiEaseInQuint(float t)
{
    return CguiEaseIn(t, 5);
}

float CguiEaseOutQuint(float t)
{
    return CguiEaseOut(t, 5);
}

float CguiEaseInOutQuint(float t)
{
    return CguiEaseInOut(t, 5);
}

float CguiEaseInSextic(float t)
{
    return CguiEaseIn(t, 6);
}

float CguiEaseOutSextic(float t)
{
    return CguiEaseOut(t, 6);
}

float CguiEaseInOutSextic(float t)
{
    return CguiEaseInOut(t, 6);
}

float CguiEaseInSeptic(float t)
{
    return CguiEaseIn(t, 7);
}

float CguiEaseOutSeptic(float t)
{
    return CguiEaseOut(t, 7);
}

float CguiEaseInOutSeptic(float t)
{
    return CguiEaseInOut(t, 7);
}

float CguiEaseInOctic(float t)
{
    return CguiEaseIn(t, 8);
}

float CguiEaseOutOctic(float t)
{
    return CguiEaseOut(t, 8);
}

float CguiEaseInOutOctic(float t)
{
    return CguiEaseInOut(t, 8);
}

float CguiEaseInNonic(float t)
{
    return CguiEaseIn(t, 9);
}

float CguiEaseOutNonic(float t)
{
    return CguiEaseOut(t, 9);
}

float CguiEaseInOutNonic(float t)
{
    return CguiEaseInOut(t, 9);
}

float CguiEaseInSine(float t)
{
    return 1 - cosf((t * PI) / 2);
}

float CguiEaseOutSine(float t)
{
    return sinf((t * PI) / 2);
}

float CguiEaseInOutSine(float t)
{
    return -(cosf(PI * t) - 1) / 2;
}

float CguiEaseInExpo(float t)
{
    return CguiEaseInExpoPro(t, 2, 10);
}

float CguiEaseOutExpo(float t)
{
    return CguiEaseOutExpoPro(t, 2, 10);
}

float CguiEaseInOutExpo(float t)
{
    return CguiEaseInOutExpoPro(t, 2, 10);
}

float CguiEaseInExpoPro(float t, float base, float power)
{
    return powf(base, power * (t - 1));
}

float CguiEaseOutExpoPro(float t, float base, float power)
{
    return 1 - powf(base, -power * t);
}

float CguiEaseInOutExpoPro(float t, float base, float power)
{
    if (t < .5)
        return (powf(base, power * (2 * t - 1))) / (powf(base, power));
    else
        return 1 - (powf(base, -power * (2 * t - 1))) / (powf(base, power));
}

float CguiEaseInCirc(float t)
{
    return 1 - sqrtf(1 - t * t);
}

float CguiEaseOutCirc(float t)
{
    return sqrtf(1 - powf(t - 1, 2));
}

float CguiEaseInOutCirc(float t)
{
    if (t < .5)
        return (1 - sqrtf(1 - 4 * t * t)) / 2;
    else
        return (sqrtf(1 - powf(-2 * t + 2, 2)) + 1) / 2;
}

float CguiEaseInBack(float t)
{
    return CguiEaseInBackPro(t, 1.70158);
}

float CguiEaseOutBack(float t)
{
    return CguiEaseOutBackPro(t, 1.70158);
}

float CguiEaseInOutBack(float t)
{
    return CguiEaseInOutBackPro(t, 1.70158 * 1.525);
}

float CguiEaseInBackPro(float t, float overshoot)
{
    return t * t * ((overshoot + 1) * t - overshoot);
}

float CguiEaseOutBackPro(float t, float overshoot)
{
    float inv = 1 - t;
    return 1 - inv * inv * ((overshoot + 1) * inv - overshoot);
}

float CguiEaseInOutBackPro(float t, float overshoot)
{
    if (t < .5)
    {
        float t2 = 2 * t;
        return (t2 * t2 * ((overshoot + 1) * t2 - overshoot)) / 2;
    }
    else
    {
        float t2 = 2 * t - 2;
        return (t2 * t2 * ((overshoot + 1) * t2 + overshoot) + 2) / 2;
    }
}

float CguiEaseInElastic(float t)
{
    return CguiEaseInElasticPro(t, 1, .3, 2, 10);
}

float CguiEaseOutElastic(float t)
{
    return CguiEaseOutElasticPro(t, 1, .3, 2, 10);
}

float CguiEaseInOutElastic(float t)
{
    return CguiEaseInOutElasticPro(t, 1, .3 * 1.5, 2, 10);
}

float CguiEaseInElasticPro(float t, float amplitude, float period, float base, float power)
{
    float s;
    if (amplitude < 1)
    {
        amplitude = 1;
        s         = period / 4;
    }
    else
    {
        s = period / (2 * PI) * asinf(1 / amplitude);
    }

    t -= 1;
    return -(amplitude * powf(base, power * t) * sinf((t - s) * (2 * PI) / period));
}

float CguiEaseOutElasticPro(float t, float amplitude, float period, float base, float power)
{
    float s;
    if (amplitude < 1)
    {
        amplitude = 1;
        s         = period / 4;
    }
    else
    {
        s = period / (2 * PI) * asinf(1 / amplitude);
    }

    return amplitude * powf(base, -power * t) * sinf((t - s) * (2 * PI) / period) + 1;
}

float CguiEaseInOutElasticPro(float t, float amplitude, float period, float base, float power)
{
    float s;
    if (amplitude < 1)
    {
        amplitude = 1;
        s         = period / 4;
    }
    else
    {
        s = period / (2 * PI) * asinf(1 / amplitude);
    }

    if (t < .5)
    {
        t = 2 * t - 1;
        return -amplitude * powf(base, power * t) * sinf((t - s) * (2 * PI) / period) / 2;
    }
    else
    {
        t = 2 * t - 1;
        return amplitude * powf(base, -power * t) * sinf((t - s) * (2 * PI) / period) / 2 + 1;
    }
}

// Bounce easing helper
float EaseBounce(float t, float b1, float b2, float b3, float b4, float c1, float c2, float c3, float c4)
{
    if (t < b1)
        return c1 * t * t;
    else if (t < b2)
        return c2 * (t - b3) * (t - b3) + b4;
    else if (t < b3)
        return c3 * (t - b4) * (t - b4) + b4;
    else
        return c4 * (t - 1) * (t - 1) + 1;
}

float CguiEaseInBounce(float t)
{
    return CguiEaseInBouncePro(t, 4.f / 11, 8.f / 11, 9.f / 10, 21.f / 22, 121.f / 16, 363.f / 40, 4356.f / 361, 54.f / 5);
}

float CguiEaseOutBounce(float t)
{
    return CguiEaseOutBouncePro(t, 4.f / 11, 8.f / 11, 9.f / 10, 21.f / 22, 121.f / 16, 363.f / 40, 4356.f / 361, 54.f / 5);
}

float CguiEaseInOutBounce(float t)
{
    return CguiEaseInOutBouncePro(t, 4.f / 11, 8.f / 11, 9.f / 10, 21.f / 22, 121.f / 16, 363.f / 40, 4356.f / 361, 54.f / 5);
}

float CguiEaseInBouncePro(float t, float b1, float b2, float b3, float b4, float c1, float c2, float c3, float c4)
{
    return 1 - EaseBounce(1 - t, b1, b2, b3, b4, c1, c2, c3, c4);
}

float CguiEaseOutBouncePro(float t, float b1, float b2, float b3, float b4, float c1, float c2, float c3, float c4)
{
    return EaseBounce(t, b1, b2, b3, b4, c1, c2, c3, c4);
}

float CguiEaseInOutBouncePro(float t, float b1, float b2, float b3, float b4, float c1, float c2, float c3, float c4)
{
    if (t < .5)
        return (1 - EaseBounce(1 - 2 * t, b1, b2, b3, b4, c1, c2, c3, c4)) / 2;
    else
        return (1 + EaseBounce(2 * t - 1, b1, b2, b3, b4, c1, c2, c3, c4)) / 2;
}
