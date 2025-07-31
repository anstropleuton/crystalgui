/// @file
///
/// @author    Anstro Pleuton
/// @copyright Copyright (c) 2025 Anstro Pleuton
///
/// Crystal GUI - A GUI framework for raylib.
///
/// This source file contains implementations for Crystalline theme.
///
/// This project is licensed under the terms of MIT license.

#include <string.h>

#include "crystalgui/crystalgui.h"
#include "raylib.h"

extern int cguiNameCounter;

static const float lightness0 = (32.0f + 4.0f * 0.0f) / 256.0f;
static const float lightness1 = (32.0f + 4.0f * 1.0f) / 256.0f;
static const float lightness2 = (32.0f + 4.0f * 2.0f) / 256.0f;
static const float lightness3 = (32.0f + 4.0f * 3.0f) / 256.0f;
static const float lightness4 = (32.0f + 4.0f * 4.0f) / 256.0f;
static const float lightness5 = (32.0f + 4.0f * 5.0f) / 256.0f;
static const float lightness6 = (32.0f + 4.0f * 6.0f) / 256.0f;

static const float hoveredLightnessDiff = +8.0f / 256.0f;
static const float heldLightnessDiff    = -8.0f / 256.0f;

static Color CguiColorFromSLAaH(Vector3 sva, float hue) // Saturation, Value, Alpha and added Hue
{
    return CguiColorFromHSLA(hue, sva.x, sva.y, sva.z);
}

static Color CguiColorFromSLAaHsS(Vector3 sva, float hue, float saturation) // Saturation, Value, Alpha, added Hue and substituted Saturation
{
    return CguiColorFromHSLA(hue, saturation, sva.y, sva.z);
}

static Color CguiColorFromSLAaHsL(Vector3 sva, float hue, float lightness) // Saturation, Value, Alpha, added Hue and substituted Lightness
{
    return CguiColorFromHSLA(hue, sva.x, lightness, sva.z);
}

CguiTheme *CguiCreateCrystallineThemeDark(void)
{
    CguiCrystallineThemeData data    = { 0 };
    data.backgroundSLA               = (Vector3) { 0.0f, lightness0, 1.0f };
    data.backgroundHoveredSLA        = (Vector3) { 0.0f, lightness0 + hoveredLightnessDiff, 1.0f };
    data.backgroundHeldSLA           = (Vector3) { 0.0f, lightness0 + heldLightnessDiff, 1.0f };
    data.backgroundDisabledSLA       = (Vector3) { 0.0f, 0.500f, 1.0f };
    data.backlayerSLA                = (Vector3) { 0.0f, lightness1, 1.0f };
    data.backlayerHoveredSLA         = (Vector3) { 0.0f, lightness1 + hoveredLightnessDiff, 1.0f };
    data.backlayerHeldSLA            = (Vector3) { 0.0f, lightness1 + heldLightnessDiff, 1.0f };
    data.backlayerDisabledSLA        = (Vector3) { 0.0f, 0.500f, 1.0f };
    data.midlayerSLA                 = (Vector3) { 0.0f, lightness2, 1.0f };
    data.midlayerHoveredSLA          = (Vector3) { 0.0f, lightness2 + hoveredLightnessDiff, 1.0f };
    data.midlayerHeldSLA             = (Vector3) { 0.0f, lightness2 + heldLightnessDiff, 1.0f };
    data.midlayerDisabledSLA         = (Vector3) { 0.0f, 0.500f, 1.0f };
    data.frontlayerSLA               = (Vector3) { 0.0f, lightness3, 1.0f };
    data.frontlayerHoveredSLA        = (Vector3) { 0.0f, lightness3 + hoveredLightnessDiff, 1.0f };
    data.frontlayerHeldSLA           = (Vector3) { 0.0f, lightness3 + heldLightnessDiff, 1.0f };
    data.frontlayerDisabledSLA       = (Vector3) { 0.0f, 0.500f, 1.0f };
    data.foregroundSLA               = (Vector3) { 0.0f, lightness4, 1.0f };
    data.foregroundHoveredSLA        = (Vector3) { 0.0f, lightness4 + hoveredLightnessDiff, 1.0f };
    data.foregroundHeldSLA           = (Vector3) { 0.0f, lightness4 + heldLightnessDiff, 1.0f };
    data.foregroundDisabledSLA       = (Vector3) { 0.0f, 0.500f, 1.0f };
    data.inactiveSLA                 = (Vector3) { 0.0f, lightness5, 1.0f };
    data.inactiveHoveredSLA          = (Vector3) { 0.0f, lightness5 + hoveredLightnessDiff, 1.0f };
    data.inactiveHeldSLA             = (Vector3) { 0.0f, lightness5 + heldLightnessDiff, 1.0f };
    data.inactiveDisabledSLA         = (Vector3) { 0.0f, 0.500f, 1.0f };
    data.activeSLA                   = (Vector3) { 0.8f, 0.400f, 1.0f };
    data.activeHoveredSLA            = (Vector3) { 0.8f, 0.400f + hoveredLightnessDiff, 1.0f };
    data.activeHeldSLA               = (Vector3) { 0.8f, 0.400f + heldLightnessDiff, 1.0f };
    data.activeDisabledSLA           = (Vector3) { 0.5f, 0.250f, 1.0f };
    data.flatSLA                     = (Vector3) { 1.0f, lightness5, 0.000f };
    data.flatHoveredSLA              = (Vector3) { 1.0f, lightness5 + hoveredLightnessDiff, 0.250f };
    data.flatHeldSLA                 = (Vector3) { 1.0f, lightness5 + heldLightnessDiff, 0.250f };
    data.flatDisabledSLA             = (Vector3) { 1.0f, 0.5f, 0.250f };
    data.textSLA                     = (Vector3) { 1.0f, 0.950f, 1.0f };
    data.disabledTextSLA             = (Vector3) { 1.0f, 0.500f, 1.0f };
    data.accentHue                   = 270.0f;
    data.dangerousHue                = 000.0f;
    data.warningHue                  = 030.0f;
    data.successHue                  = 120.0f;
    data.infoHue                     = 200.0f;
    data.headingFontSize             = 24.0f;
    data.bodyFontSize                = 18.0f;
    data.captionFontSize             = 14.0f;
    data.headingLineHeight           = 1.25f;
    data.bodyLineHeight              = 1.375f;
    data.captionLineHeight           = 1.5f;
    data.textFont                    = LoadFontEx("resource/fonts/Inter/static/Inter_18pt-Regular.ttf", 18, NULL, 0);
    data.textFontItalic              = LoadFontEx("resource/fonts/Inter/static/Inter_18pt-Italic.ttf", 18, NULL, 0);
    data.textFontBold                = LoadFontEx("resource/fonts/Inter/static/Inter_24pt-SemiBold.ttf", 24, NULL, 0);
    data.textFontBoldItalic          = LoadFontEx("resource/fonts/Inter/static/Inter_24pt-SemiBoldItalic.ttf", 24, NULL, 0);
    data.textFontLight               = LoadFontEx("resource/fonts/Inter/static/Inter_18pt-Light.ttf", 14, NULL, 0);
    data.textFontLightItalic         = LoadFontEx("resource/fonts/Inter/static/Inter_18pt-LightItalic.ttf", 14, NULL, 0);
    data.backlayerRadii              = (Vector4) { 20.0f, 20.0f, 20.0f, 20.0f };
    data.midlayerRadii               = (Vector4) { 15.0f, 15.0f, 15.0f, 15.0f };
    data.frontlayerRadii             = (Vector4) { 10.0f, 10.0f, 10.0f, 10.0f };
    data.layerShadowDistance         = 10.0f;
    data.layerShadowOffset           = (Vector2) { 0.0f, 10.0f };
    data.layerShadowShrink           = 5.0f;
    data.layerShadowSLA              = (Vector3) { 0.0f, 0.0f, 0.000f }; // No shadow
    data.layerBorderThickness        = 2.0f;
    data.layerBorderSLA              = (Vector3) { 0.0f, 1.0f, 0.050f };
    data.componentRadii              = (Vector4) { 5.0f, 5.0f, 5.0f, 5.0f };
    data.componentShadowDistance     = 10.0f;
    data.componentShadowOffset       = (Vector2) { 0.0f, 10.0f };
    data.componentShadowShrink       = 5.0f;
    data.componentShadowSLA          = (Vector3) { 0.0f, 0.0f, 0.000f }; // No shadow
    data.componentBorderThickness    = 2.0f;
    data.componentBorderSLA          = (Vector3) { 0.0f, 1.0f, 0.050f };
    data.componentTransitionDuration = 0.25f;
    data.componentTransitionEasing   = CguiEaseInOutQuad;

    return CguiCreateCrystallineThemeFromData(data);
}

CguiTheme *CguiCreateCrystallineThemeLight(void)
{
    CguiCrystallineThemeData data    = { 0 };
    data.backgroundSLA               = (Vector3) { 0.0f, 1.0f - lightness5, 1.0f };
    data.backgroundHoveredSLA        = (Vector3) { 0.0f, 1.0f - lightness5 + hoveredLightnessDiff, 1.0f };
    data.backgroundHeldSLA           = (Vector3) { 0.0f, 1.0f - lightness5 + heldLightnessDiff, 1.0f };
    data.backgroundDisabledSLA       = (Vector3) { 0.0f, 0.500f, 1.0f };
    data.backlayerSLA                = (Vector3) { 0.0f, 1.0f - lightness4, 1.0f };
    data.backlayerHoveredSLA         = (Vector3) { 0.0f, 1.0f - lightness4 + hoveredLightnessDiff, 1.0f };
    data.backlayerHeldSLA            = (Vector3) { 0.0f, 1.0f - lightness4 + heldLightnessDiff, 1.0f };
    data.backlayerDisabledSLA        = (Vector3) { 0.0f, 0.500f, 1.0f };
    data.midlayerSLA                 = (Vector3) { 0.0f, 1.0f - lightness3, 1.0f };
    data.midlayerHoveredSLA          = (Vector3) { 0.0f, 1.0f - lightness3 + hoveredLightnessDiff, 1.0f };
    data.midlayerHeldSLA             = (Vector3) { 0.0f, 1.0f - lightness3 + heldLightnessDiff, 1.0f };
    data.midlayerDisabledSLA         = (Vector3) { 0.0f, 0.500f, 1.0f };
    data.frontlayerSLA               = (Vector3) { 0.0f, 1.0f - lightness2, 1.0f };
    data.frontlayerHoveredSLA        = (Vector3) { 0.0f, 1.0f - lightness2 + hoveredLightnessDiff, 1.0f };
    data.frontlayerHeldSLA           = (Vector3) { 0.0f, 1.0f - lightness2 + heldLightnessDiff, 1.0f };
    data.frontlayerDisabledSLA       = (Vector3) { 0.0f, 0.500f, 1.0f };
    data.foregroundSLA               = (Vector3) { 1.0f, 1.0f - lightness1, 1.0f };
    data.foregroundHoveredSLA        = (Vector3) { 1.0f, 1.0f - lightness1 + hoveredLightnessDiff, 1.0f };
    data.foregroundHeldSLA           = (Vector3) { 1.0f, 1.0f - lightness1 + heldLightnessDiff, 1.0f };
    data.foregroundDisabledSLA       = (Vector3) { 0.0f, 0.500f, 1.0f };
    data.inactiveSLA                 = (Vector3) { 0.0f, 1.0f - lightness0, 1.0f };
    data.inactiveHoveredSLA          = (Vector3) { 0.0f, 1.0f - lightness0 + hoveredLightnessDiff, 1.0f };
    data.inactiveHeldSLA             = (Vector3) { 0.0f, 1.0f - lightness0 + heldLightnessDiff, 1.0f };
    data.inactiveDisabledSLA         = (Vector3) { 0.0f, 0.500f, 1.0f };
    data.activeSLA                   = (Vector3) { 0.8f, 0.600f, 1.0f };
    data.activeHoveredSLA            = (Vector3) { 0.8f, 0.600f + hoveredLightnessDiff, 1.0f };
    data.activeHeldSLA               = (Vector3) { 0.8f, 0.600f + heldLightnessDiff, 1.0f };
    data.activeDisabledSLA           = (Vector3) { 0.5f, 0.750f, 1.0f };
    data.flatSLA                     = (Vector3) { 1.0f, 1.0f - lightness0, 0.000f };
    data.flatHoveredSLA              = (Vector3) { 1.0f, 1.0f - lightness0 + hoveredLightnessDiff, 0.250f };
    data.flatHeldSLA                 = (Vector3) { 1.0f, 1.0f - lightness0 + heldLightnessDiff, 0.250f };
    data.flatDisabledSLA             = (Vector3) { 1.0f, 0.5f, 0.250f };
    data.textSLA                     = (Vector3) { 1.0f, 0.050f, 1.0f };
    data.disabledTextSLA             = (Vector3) { 1.0f, 0.500f, 1.0f };
    data.accentHue                   = 270.0f;
    data.dangerousHue                = 000.0f;
    data.warningHue                  = 030.0f;
    data.successHue                  = 120.0f;
    data.infoHue                     = 200.0f;
    data.headingFontSize             = 24.0f;
    data.bodyFontSize                = 18.0f;
    data.captionFontSize             = 14.0f;
    data.headingLineHeight           = 1.25f;
    data.bodyLineHeight              = 1.375f;
    data.captionLineHeight           = 1.5f;
    data.textFont                    = LoadFontEx("resource/fonts/Inter/static/Inter_18pt-Regular.ttf", 18, NULL, 0);
    data.textFontItalic              = LoadFontEx("resource/fonts/Inter/static/Inter_18pt-Italic.ttf", 18, NULL, 0);
    data.textFontBold                = LoadFontEx("resource/fonts/Inter/static/Inter_24pt-SemiBold.ttf", 24, NULL, 0);
    data.textFontBoldItalic          = LoadFontEx("resource/fonts/Inter/static/Inter_24pt-SemiBoldItalic.ttf", 24, NULL, 0);
    data.textFontLight               = LoadFontEx("resource/fonts/Inter/static/Inter_18pt-Light.ttf", 14, NULL, 0);
    data.textFontLightItalic         = LoadFontEx("resource/fonts/Inter/static/Inter_18pt-LightItalic.ttf", 14, NULL, 0);
    data.backlayerRadii              = (Vector4) { 20.0f, 20.0f, 20.0f, 20.0f };
    data.midlayerRadii               = (Vector4) { 15.0f, 15.0f, 15.0f, 15.0f };
    data.frontlayerRadii             = (Vector4) { 10.0f, 10.0f, 10.0f, 10.0f };
    data.layerShadowDistance         = 10.0f;
    data.layerShadowOffset           = (Vector2) { 0.0f, 10.0f };
    data.layerShadowShrink           = 5.0f;
    data.layerShadowSLA              = (Vector3) { 0.0f, 0.0f, 0.050f };
    data.layerBorderThickness        = 2.0f;
    data.layerBorderSLA              = (Vector3) { 0.0f, 0.0f, 0.000f }; // No border
    data.componentRadii              = (Vector4) { 5.0f, 5.0f, 5.0f, 5.0f };
    data.componentShadowDistance     = 10.0f;
    data.componentShadowOffset       = (Vector2) { 0.0f, 10.0f };
    data.componentShadowShrink       = 5.0f;
    data.componentShadowSLA          = (Vector3) { 0.0f, 0.0f, 0.050f };
    data.componentBorderThickness    = 2.0f;
    data.componentBorderSLA          = (Vector3) { 0.0f, 0.0f, 0.000f }; // No border
    data.componentTransitionDuration = 0.25f;
    data.componentTransitionEasing   = CguiEaseInOutQuad;

    return CguiCreateCrystallineThemeFromData(data);
}

CguiTheme *CguiCreateCrystallineThemeDarkHighContrast(void)
{
    CguiCrystallineThemeData data    = { 0 };
    data.backgroundSLA               = (Vector3) { 0.0f, lightness0, 1.0f };
    data.backgroundHoveredSLA        = (Vector3) { 0.0f, lightness0 + hoveredLightnessDiff, 1.0f };
    data.backgroundHeldSLA           = (Vector3) { 0.0f, lightness0 + heldLightnessDiff, 1.0f };
    data.backgroundDisabledSLA       = (Vector3) { 0.0f, 0.500f, 1.0f };
    data.backlayerSLA                = (Vector3) { 0.0f, lightness0, 1.0f };
    data.backlayerHoveredSLA         = (Vector3) { 0.0f, lightness0 + hoveredLightnessDiff, 1.0f };
    data.backlayerHeldSLA            = (Vector3) { 0.0f, lightness0 + heldLightnessDiff, 1.0f };
    data.backlayerDisabledSLA        = (Vector3) { 0.0f, 0.500f, 1.0f };
    data.midlayerSLA                 = (Vector3) { 0.0f, lightness0, 1.0f };
    data.midlayerHoveredSLA          = (Vector3) { 0.0f, lightness0 + hoveredLightnessDiff, 1.0f };
    data.midlayerHeldSLA             = (Vector3) { 0.0f, lightness0 + heldLightnessDiff, 1.0f };
    data.midlayerDisabledSLA         = (Vector3) { 0.0f, 0.500f, 1.0f };
    data.frontlayerSLA               = (Vector3) { 0.0f, lightness0, 1.0f };
    data.frontlayerHoveredSLA        = (Vector3) { 0.0f, lightness0 + hoveredLightnessDiff, 1.0f };
    data.frontlayerHeldSLA           = (Vector3) { 0.0f, lightness0 + heldLightnessDiff, 1.0f };
    data.frontlayerDisabledSLA       = (Vector3) { 0.0f, 0.500f, 1.0f };
    data.foregroundSLA               = (Vector3) { 1.0f, lightness1, 1.0f };
    data.foregroundHoveredSLA        = (Vector3) { 1.0f, lightness1 + hoveredLightnessDiff, 1.0f };
    data.foregroundHeldSLA           = (Vector3) { 1.0f, lightness1 + heldLightnessDiff, 1.0f };
    data.foregroundDisabledSLA       = (Vector3) { 0.0f, 0.500f, 1.0f };
    data.inactiveSLA                 = (Vector3) { 0.0f, lightness2, 1.0f };
    data.inactiveHoveredSLA          = (Vector3) { 0.0f, lightness2 + hoveredLightnessDiff, 1.0f };
    data.inactiveHeldSLA             = (Vector3) { 0.0f, lightness2 + heldLightnessDiff, 1.0f };
    data.inactiveDisabledSLA         = (Vector3) { 0.0f, 0.500f, 1.0f };
    data.activeSLA                   = (Vector3) { 1.0f, 0.400f, 1.0f };
    data.activeHoveredSLA            = (Vector3) { 1.0f, 0.400f + hoveredLightnessDiff, 1.0f };
    data.activeHeldSLA               = (Vector3) { 1.0f, 0.400f + heldLightnessDiff, 1.0f };
    data.activeDisabledSLA           = (Vector3) { 0.5f, 0.125f, 1.0f };
    data.flatSLA                     = (Vector3) { 1.0f, 0.5f, 0.005f };
    data.flatHoveredSLA              = (Vector3) { 1.0f, 0.5f, 0.005f };
    data.flatHeldSLA                 = (Vector3) { 1.0f, 0.5f, 0.005f };
    data.flatDisabledSLA             = (Vector3) { 1.0f, 0.5f, 0.005f };
    data.textSLA                     = (Vector3) { 1.0f, 1.000f, 1.0f };
    data.disabledTextSLA             = (Vector3) { 1.0f, 0.500f, 1.0f };
    data.accentHue                   = 270.0f;
    data.dangerousHue                = 000.0f;
    data.warningHue                  = 030.0f;
    data.successHue                  = 120.0f;
    data.infoHue                     = 200.0f;
    data.headingFontSize             = 24.0f;
    data.bodyFontSize                = 18.0f;
    data.captionFontSize             = 14.0f;
    data.headingLineHeight           = 1.25f;
    data.bodyLineHeight              = 1.375f;
    data.captionLineHeight           = 1.5f;
    data.textFont                    = LoadFontEx("resource/fonts/Inter/static/Inter_18pt-Regular.ttf", 18, NULL, 0);
    data.textFontItalic              = LoadFontEx("resource/fonts/Inter/static/Inter_18pt-Italic.ttf", 18, NULL, 0);
    data.textFontBold                = LoadFontEx("resource/fonts/Inter/static/Inter_24pt-SemiBold.ttf", 24, NULL, 0);
    data.textFontBoldItalic          = LoadFontEx("resource/fonts/Inter/static/Inter_24pt-SemiBoldItalic.ttf", 24, NULL, 0);
    data.textFontLight               = LoadFontEx("resource/fonts/Inter/static/Inter_18pt-Light.ttf", 14, NULL, 0);
    data.textFontLightItalic         = LoadFontEx("resource/fonts/Inter/static/Inter_18pt-LightItalic.ttf", 14, NULL, 0);
    data.backlayerRadii              = (Vector4) { 20.0f, 20.0f, 20.0f, 20.0f };
    data.midlayerRadii               = (Vector4) { 15.0f, 15.0f, 15.0f, 15.0f };
    data.frontlayerRadii             = (Vector4) { 10.0f, 10.0f, 10.0f, 10.0f };
    data.layerShadowDistance         = 10.0f;
    data.layerShadowOffset           = (Vector2) { 0.0f, 10.0f };
    data.layerShadowShrink           = 5.0f;
    data.layerShadowSLA              = (Vector3) { 0.0f, 0.0f, 0.050f };
    data.layerBorderThickness        = 2.0f;
    data.layerBorderSLA              = (Vector3) { 0.0f, 1.0f, 0.050f };
    data.componentRadii              = (Vector4) { 5.0f, 5.0f, 5.0f, 5.0f };
    data.componentShadowDistance     = 10.0f;
    data.componentShadowOffset       = (Vector2) { 0.0f, 10.0f };
    data.componentShadowShrink       = 5.0f;
    data.componentShadowSLA          = (Vector3) { 0.0f, 0.0f, 0.050f };
    data.componentBorderThickness    = 2.0f;
    data.componentBorderSLA          = (Vector3) { 0.0f, 1.0f, 0.500f };
    data.componentTransitionDuration = 0.25f;
    data.componentTransitionEasing   = CguiEaseInOutQuad;

    return CguiCreateCrystallineThemeFromData(data);
}

CguiTheme *CguiCreateCrystallineThemeLightHighContrast(void)
{
    CguiCrystallineThemeData data    = { 0 };
    data.backgroundSLA               = (Vector3) { 0.0f, 1.0f - lightness6, 1.0f };
    data.backgroundHoveredSLA        = (Vector3) { 0.0f, 1.0f - lightness6 + hoveredLightnessDiff, 1.0f };
    data.backgroundHeldSLA           = (Vector3) { 0.0f, 1.0f - lightness6 + heldLightnessDiff, 1.0f };
    data.backgroundDisabledSLA       = (Vector3) { 0.0f, 0.500f, 1.0f };
    data.backlayerSLA                = (Vector3) { 0.0f, 1.0f - lightness6, 1.0f };
    data.backlayerHoveredSLA         = (Vector3) { 0.0f, 1.0f - lightness6 + hoveredLightnessDiff, 1.0f };
    data.backlayerHeldSLA            = (Vector3) { 0.0f, 1.0f - lightness6 + heldLightnessDiff, 1.0f };
    data.backlayerDisabledSLA        = (Vector3) { 0.0f, 0.500f, 1.0f };
    data.midlayerSLA                 = (Vector3) { 0.0f, 1.0f - lightness6, 1.0f };
    data.midlayerHoveredSLA          = (Vector3) { 0.0f, 1.0f - lightness6 + hoveredLightnessDiff, 1.0f };
    data.midlayerHeldSLA             = (Vector3) { 0.0f, 1.0f - lightness6 + heldLightnessDiff, 1.0f };
    data.midlayerDisabledSLA         = (Vector3) { 0.0f, 0.500f, 1.0f };
    data.frontlayerSLA               = (Vector3) { 0.0f, 1.0f - lightness6, 1.0f };
    data.frontlayerHoveredSLA        = (Vector3) { 0.0f, 1.0f - lightness6 + hoveredLightnessDiff, 1.0f };
    data.frontlayerHeldSLA           = (Vector3) { 0.0f, 1.0f - lightness6 + heldLightnessDiff, 1.0f };
    data.frontlayerDisabledSLA       = (Vector3) { 0.0f, 0.500f, 1.0f };
    data.foregroundSLA               = (Vector3) { 1.0f, 1.0f - lightness5, 1.0f };
    data.foregroundHoveredSLA        = (Vector3) { 1.0f, 1.0f - lightness5 + hoveredLightnessDiff, 1.0f };
    data.foregroundHeldSLA           = (Vector3) { 1.0f, 1.0f - lightness5 + heldLightnessDiff, 1.0f };
    data.foregroundDisabledSLA       = (Vector3) { 0.0f, 0.500f, 1.0f };
    data.inactiveSLA                 = (Vector3) { 0.0f, 1.0f - lightness4, 1.0f };
    data.inactiveHoveredSLA          = (Vector3) { 0.0f, 1.0f - lightness4 + hoveredLightnessDiff, 1.0f };
    data.inactiveHeldSLA             = (Vector3) { 0.0f, 1.0f - lightness4 + heldLightnessDiff, 1.0f };
    data.inactiveDisabledSLA         = (Vector3) { 0.0f, 0.500f, 1.0f };
    data.activeSLA                   = (Vector3) { 1.0f, 0.600f, 1.0f };
    data.activeHoveredSLA            = (Vector3) { 1.0f, 0.600f + hoveredLightnessDiff, 1.0f };
    data.activeHeldSLA               = (Vector3) { 1.0f, 0.600f + heldLightnessDiff, 1.0f };
    data.activeDisabledSLA           = (Vector3) { 0.5f, 0.875f, 1.0f };
    data.flatSLA                     = (Vector3) { 1.0f, 0.5f, 0.005f };
    data.flatHoveredSLA              = (Vector3) { 1.0f, 0.5f, 0.005f };
    data.flatHeldSLA                 = (Vector3) { 1.0f, 0.5f, 0.005f };
    data.flatDisabledSLA             = (Vector3) { 1.0f, 0.5f, 0.005f };
    data.textSLA                     = (Vector3) { 1.0f, 0.000f, 1.0f };
    data.disabledTextSLA             = (Vector3) { 1.0f, 0.500f, 1.0f };
    data.accentHue                   = 270.0f;
    data.dangerousHue                = 000.0f;
    data.warningHue                  = 030.0f;
    data.successHue                  = 120.0f;
    data.infoHue                     = 200.0f;
    data.headingFontSize             = 24.0f;
    data.bodyFontSize                = 18.0f;
    data.captionFontSize             = 14.0f;
    data.headingLineHeight           = 1.25f;
    data.bodyLineHeight              = 1.375f;
    data.captionLineHeight           = 1.5f;
    data.textFont                    = LoadFontEx("resource/fonts/Inter/static/Inter_18pt-Regular.ttf", 18, NULL, 0);
    data.textFontItalic              = LoadFontEx("resource/fonts/Inter/static/Inter_18pt-Italic.ttf", 18, NULL, 0);
    data.textFontBold                = LoadFontEx("resource/fonts/Inter/static/Inter_24pt-SemiBold.ttf", 24, NULL, 0);
    data.textFontBoldItalic          = LoadFontEx("resource/fonts/Inter/static/Inter_24pt-SemiBoldItalic.ttf", 24, NULL, 0);
    data.textFontLight               = LoadFontEx("resource/fonts/Inter/static/Inter_18pt-Light.ttf", 14, NULL, 0);
    data.textFontLightItalic         = LoadFontEx("resource/fonts/Inter/static/Inter_18pt-LightItalic.ttf", 14, NULL, 0);
    data.backlayerRadii              = (Vector4) { 20.0f, 20.0f, 20.0f, 20.0f };
    data.midlayerRadii               = (Vector4) { 15.0f, 15.0f, 15.0f, 15.0f };
    data.frontlayerRadii             = (Vector4) { 10.0f, 10.0f, 10.0f, 10.0f };
    data.layerShadowDistance         = 10.0f;
    data.layerShadowOffset           = (Vector2) { 0.0f, 10.0f };
    data.layerShadowShrink           = 5.0f;
    data.layerShadowSLA              = (Vector3) { 0.0f, 0.0f, 0.050f };
    data.layerBorderThickness        = 2.0f;
    data.layerBorderSLA              = (Vector3) { 0.0f, 1.0f, 0.050f };
    data.componentRadii              = (Vector4) { 5.0f, 5.0f, 5.0f, 5.0f };
    data.componentShadowDistance     = 10.0f;
    data.componentShadowOffset       = (Vector2) { 0.0f, 10.0f };
    data.componentShadowShrink       = 5.0f;
    data.componentShadowSLA          = (Vector3) { 0.0f, 0.0f, 0.050f };
    data.componentBorderThickness    = 2.0f;
    data.componentBorderSLA          = (Vector3) { 0.0f, 0.0f, 0.500f };
    data.componentTransitionDuration = 0.25f;
    data.componentTransitionEasing   = CguiEaseInOutQuad;

    return CguiCreateCrystallineThemeFromData(data);
}

static void CguiCreateRootTheme(CguiTheme *theme, CguiCrystallineThemeData data)
{
    CguiNode *rootTemplate = CguiCreateNodeProMax(CguiTZeroSize(), TextFormat("CguiRoot #%d", ++cguiNameCounter), CGUI_COMPONENT_NODE_TYPE_ROOT, NULL, sizeof(CguiRootData), NULL, sizeof(CguiRootInstanceData));
    if (!rootTemplate)
    {
        return;
    }

    CguiRootData *rootData = rootTemplate->data;

    if (!CguiSetComponentTemplate(theme, rootTemplate))
    {
        CguiDeleteNode(rootTemplate);
        return;
    }

    rootData->backgroundColor = CguiColorFromSLAaHsS(data.backgroundSLA, 0.0f, 0.0f);
    rootData->transitionDuration   = data.componentTransitionDuration;
    rootData->transitionEasing     = data.componentTransitionEasing;
}

static void CguiCreateLayerTheme(CguiTheme *theme, CguiCrystallineThemeData data)
{
    CguiNode *layerTemplate = CguiCreateNodeProMax(CguiTZeroSize(), TextFormat("CguiLayer #%d", ++cguiNameCounter), CGUI_COMPONENT_NODE_TYPE_LAYER, NULL, sizeof(CguiLayerData), NULL, sizeof(CguiLayerInstanceData));
    if (!layerTemplate)
    {
        return;
    }

    CguiLayerData *layerData = layerTemplate->data;

    if (!CguiSetComponentTemplate(theme, layerTemplate))
    {
        CguiDeleteNode(layerTemplate);
        return;
    }

    CguiNode *boxNodeRef = CguiCreateBoxElement(0.0f, BLANK);
    if (!boxNodeRef)
    {
        CguiDeleteNode(layerTemplate);
        return;
    }

    boxNodeRef->transformation = CguiTFillParent();

    if (!CguiInsertChildAt(layerTemplate, boxNodeRef, CGUI_LAYER_COMPOSITION_BOX_NODE))
    {
        CguiDeleteNode(layerTemplate);
        CguiDeleteNode(boxNodeRef);
        return;
    }

    if (!CguiSetComponentTemplate(theme, layerTemplate))
    {
        CguiDeleteNode(layerTemplate);
        return;
    }

    layerData->transitionDuration = data.componentTransitionDuration;
    layerData->transitionEasing   = data.componentTransitionEasing;

    CguiBoxElementData boxData;

    boxData.shadowDistance  = data.layerShadowDistance;
    boxData.shadowOffset    = data.layerShadowOffset;
    boxData.shadowShrink    = data.layerShadowShrink;
    boxData.shadowColor     = CguiColorFromSLAaHsS(data.layerShadowSLA, 0.0f, 0.0f);
    boxData.borderThickness = data.layerBorderThickness;
    boxData.borderColor     = CguiColorFromSLAaHsS(data.layerBorderSLA, 0.0f, 0.0f);

    CguiBoxElementData *backlayer  = &layerData->boxDatas[CGUI_LAYER_TYPE_BACKLAYER];
    CguiBoxElementData *midlayer   = &layerData->boxDatas[CGUI_LAYER_TYPE_MIDLAYER];
    CguiBoxElementData *frontlayer = &layerData->boxDatas[CGUI_LAYER_TYPE_FRONTLAYER];

    *backlayer  = boxData;
    *midlayer   = boxData;
    *frontlayer = boxData;

    backlayer->radii  = data.backlayerRadii;
    backlayer->color  = CguiColorFromSLAaHsS(data.backlayerSLA, 0.0f, 0.0f);
    midlayer->radii   = data.midlayerRadii;
    midlayer->color   = CguiColorFromSLAaHsS(data.midlayerSLA, 0.0f, 0.0f);
    frontlayer->radii = data.frontlayerRadii;
    frontlayer->color = CguiColorFromSLAaHsS(data.frontlayerSLA, 0.0f, 0.0f);
}

static void CguiCreateLabelTheme(CguiTheme *theme, CguiCrystallineThemeData data)
{
    CguiNode *labelTemplate = CguiCreateNodeProMax(CguiTZeroSize(), TextFormat("CguiLabel #%d", ++cguiNameCounter), CGUI_COMPONENT_NODE_TYPE_LABEL, NULL, sizeof(CguiLabelData), NULL, sizeof(CguiLabelInstanceData));
    if (!labelTemplate)
    {
        return;
    }

    CguiLabelData *labelData = labelTemplate->data;

    CguiNode *textNodeRef = CguiCreateTextElement(NULL, BLANK);
    if (!textNodeRef)
    {
        CguiDeleteNode(labelTemplate);
        return;
    }

    textNodeRef->transformation = CguiTFillParent();

    if (!CguiInsertChildAt(labelTemplate, textNodeRef, CGUI_LABEL_COMPOSITION_TEXT_NODE))
    {
        CguiDeleteNode(labelTemplate);
        CguiDeleteNode(textNodeRef);
        return;
    }

    if (!CguiSetComponentTemplate(theme, labelTemplate))
    {
        CguiDeleteNode(labelTemplate);
        return;
    }

    labelData->transitionDuration = data.componentTransitionDuration;
    labelData->transitionEasing   = data.componentTransitionEasing;

    CguiTextElementData textData;

    textData.spacing = 1.0f;
    textData.color   = CguiColorFromSLAaHsS(data.textSLA, 0.0f, 0.0f);

    CguiTextElementData *heading         = &labelData->textDatas[CGUI_LABEL_TYPE_HEADING];
    CguiTextElementData *body            = &labelData->textDatas[CGUI_LABEL_TYPE_BODY];
    CguiTextElementData *caption         = &labelData->textDatas[CGUI_LABEL_TYPE_CAPTION];
    CguiTextElementData *disabledHeading = &labelData->disabledTextDatas[CGUI_LABEL_TYPE_HEADING];
    CguiTextElementData *disabledBody    = &labelData->disabledTextDatas[CGUI_LABEL_TYPE_BODY];
    CguiTextElementData *disabledCaption = &labelData->disabledTextDatas[CGUI_LABEL_TYPE_CAPTION];

    *heading         = textData;
    *body            = textData;
    *caption         = textData;
    *disabledHeading = textData;
    *disabledBody    = textData;
    *disabledCaption = textData;

    heading->font        = data.textFontBold;
    heading->fontSize    = data.headingFontSize;
    heading->lineSpacing = data.headingLineHeight;

    body->font        = data.textFont;
    body->fontSize    = data.bodyFontSize;
    body->lineSpacing = data.bodyLineHeight;

    caption->font        = data.textFontLight;
    caption->fontSize    = data.captionFontSize;
    caption->lineSpacing = data.captionLineHeight;

    disabledHeading->font        = data.textFontBold;
    disabledHeading->fontSize    = data.headingFontSize;
    disabledHeading->lineSpacing = data.headingLineHeight;
    disabledHeading->color       = CguiColorFromSLAaHsS(data.disabledTextSLA, 0.0f, 0.0f);

    disabledBody->font        = data.textFont;
    disabledBody->fontSize    = data.bodyFontSize;
    disabledBody->lineSpacing = data.bodyLineHeight;
    disabledBody->color       = CguiColorFromSLAaHsS(data.disabledTextSLA, 0.0f, 0.0f);

    disabledCaption->font        = data.textFontLight;
    disabledCaption->fontSize    = data.captionFontSize;
    disabledCaption->lineSpacing = data.captionLineHeight;
    disabledCaption->color       = CguiColorFromSLAaHsS(data.disabledTextSLA, 0.0f, 0.0f);
}

static void CguiCreateButtonTheme(CguiTheme *theme, CguiCrystallineThemeData data)
{
    CguiNode *buttonTemplate = CguiCreateNodeProMax(CguiTZeroSize(), TextFormat("CguiButton #%d", ++cguiNameCounter), CGUI_COMPONENT_NODE_TYPE_BUTTON, NULL, sizeof(CguiButtonData), NULL, sizeof(CguiButtonInstanceData));
    if (!buttonTemplate)
    {
        return;
    }

    CguiButtonData *buttonData = buttonTemplate->data;

    if (!CguiSetComponentTemplate(theme, buttonTemplate))
    {
        CguiDeleteNode(buttonTemplate);
        return;
    }

    CguiNode *boxNodeRef = CguiCreateBoxElement(0.0f, BLANK);
    if (!boxNodeRef)
    {
        CguiDeleteNode(buttonTemplate);
        return;
    }

    boxNodeRef->transformation = CguiTFillParent();

    if (!CguiInsertChildAt(buttonTemplate, boxNodeRef, CGUI_BUTTON_COMPOSITION_BOX_NODE))
    {
        CguiDeleteNode(buttonTemplate);
        CguiDeleteNode(boxNodeRef);
        return;
    }

    if (!CguiSetComponentTemplate(theme, buttonTemplate))
    {
        CguiDeleteNode(buttonTemplate);
        return;
    }

    buttonData->transitionDuration = data.componentTransitionDuration;
    buttonData->transitionEasing   = data.componentTransitionEasing;

    CguiBoxElementData boxData;

    boxData.radii           = data.componentRadii;
    boxData.shadowDistance  = data.componentShadowDistance;
    boxData.shadowOffset    = data.componentShadowOffset;
    boxData.shadowShrink    = data.componentShadowShrink;
    boxData.shadowColor     = CguiColorFromSLAaHsS(data.componentShadowSLA, 0.0f, 0.0f);
    boxData.borderThickness = data.componentBorderThickness;
    boxData.borderColor     = CguiColorFromSLAaHsS(data.componentBorderSLA, 0.0f, 0.0f);

    CguiBoxElementData *normal            = &buttonData->boxDatas[CGUI_BUTTON_TYPE_NORMAL];
    CguiBoxElementData *flat              = &buttonData->boxDatas[CGUI_BUTTON_TYPE_FLAT];
    CguiBoxElementData *accent            = &buttonData->boxDatas[CGUI_BUTTON_TYPE_ACCENT];
    CguiBoxElementData *dangerous         = &buttonData->boxDatas[CGUI_BUTTON_TYPE_DANGEROUS];
    CguiBoxElementData *warning           = &buttonData->boxDatas[CGUI_BUTTON_TYPE_WARNING];
    CguiBoxElementData *success           = &buttonData->boxDatas[CGUI_BUTTON_TYPE_SUCCESS];
    CguiBoxElementData *info              = &buttonData->boxDatas[CGUI_BUTTON_TYPE_INFO];
    CguiBoxElementData *hoveredNormal     = &buttonData->hoveredBoxDatas[CGUI_BUTTON_TYPE_NORMAL];
    CguiBoxElementData *hoveredFlat       = &buttonData->hoveredBoxDatas[CGUI_BUTTON_TYPE_FLAT];
    CguiBoxElementData *hoveredAccent     = &buttonData->hoveredBoxDatas[CGUI_BUTTON_TYPE_ACCENT];
    CguiBoxElementData *hoveredDangerous  = &buttonData->hoveredBoxDatas[CGUI_BUTTON_TYPE_DANGEROUS];
    CguiBoxElementData *hoveredWarning    = &buttonData->hoveredBoxDatas[CGUI_BUTTON_TYPE_WARNING];
    CguiBoxElementData *hoveredSuccess    = &buttonData->hoveredBoxDatas[CGUI_BUTTON_TYPE_SUCCESS];
    CguiBoxElementData *hoveredInfo       = &buttonData->hoveredBoxDatas[CGUI_BUTTON_TYPE_INFO];
    CguiBoxElementData *heldNormal        = &buttonData->heldBoxDatas[CGUI_BUTTON_TYPE_NORMAL];
    CguiBoxElementData *heldFlat          = &buttonData->heldBoxDatas[CGUI_BUTTON_TYPE_FLAT];
    CguiBoxElementData *heldAccent        = &buttonData->heldBoxDatas[CGUI_BUTTON_TYPE_ACCENT];
    CguiBoxElementData *heldDangerous     = &buttonData->heldBoxDatas[CGUI_BUTTON_TYPE_DANGEROUS];
    CguiBoxElementData *heldWarning       = &buttonData->heldBoxDatas[CGUI_BUTTON_TYPE_WARNING];
    CguiBoxElementData *heldSuccess       = &buttonData->heldBoxDatas[CGUI_BUTTON_TYPE_SUCCESS];
    CguiBoxElementData *heldInfo          = &buttonData->heldBoxDatas[CGUI_BUTTON_TYPE_INFO];
    CguiBoxElementData *disabledNormal    = &buttonData->disabledBoxDatas[CGUI_BUTTON_TYPE_NORMAL];
    CguiBoxElementData *disabledFlat      = &buttonData->disabledBoxDatas[CGUI_BUTTON_TYPE_FLAT];
    CguiBoxElementData *disabledAccent    = &buttonData->disabledBoxDatas[CGUI_BUTTON_TYPE_ACCENT];
    CguiBoxElementData *disabledDangerous = &buttonData->disabledBoxDatas[CGUI_BUTTON_TYPE_DANGEROUS];
    CguiBoxElementData *disabledWarning   = &buttonData->disabledBoxDatas[CGUI_BUTTON_TYPE_WARNING];
    CguiBoxElementData *disabledSuccess   = &buttonData->disabledBoxDatas[CGUI_BUTTON_TYPE_SUCCESS];
    CguiBoxElementData *disabledInfo      = &buttonData->disabledBoxDatas[CGUI_BUTTON_TYPE_INFO];

    *normal            = boxData;
    *flat              = boxData;
    *accent            = boxData;
    *dangerous         = boxData;
    *warning           = boxData;
    *success           = boxData;
    *info              = boxData;
    *hoveredNormal     = boxData;
    *hoveredFlat       = boxData;
    *hoveredAccent     = boxData;
    *hoveredDangerous  = boxData;
    *hoveredWarning    = boxData;
    *hoveredSuccess    = boxData;
    *hoveredInfo       = boxData;
    *heldNormal        = boxData;
    *heldFlat          = boxData;
    *heldAccent        = boxData;
    *heldDangerous     = boxData;
    *heldWarning       = boxData;
    *heldSuccess       = boxData;
    *heldInfo          = boxData;
    *disabledNormal    = boxData;
    *disabledFlat      = boxData;
    *disabledAccent    = boxData;
    *disabledDangerous = boxData;
    *disabledWarning   = boxData;
    *disabledSuccess   = boxData;
    *disabledInfo      = boxData;

    normal->color            = CguiColorFromSLAaHsS(data.inactiveSLA, 0.0f, 0.0f);
    flat->color              = CguiColorFromSLAaHsS(data.flatSLA, 0.0f, 0.0f);
    accent->color            = CguiColorFromSLAaH(data.activeSLA, data.accentHue);
    dangerous->color         = CguiColorFromSLAaH(data.activeSLA, data.dangerousHue);
    warning->color           = CguiColorFromSLAaH(data.activeSLA, data.warningHue);
    success->color           = CguiColorFromSLAaH(data.activeSLA, data.successHue);
    info->color              = CguiColorFromSLAaH(data.activeSLA, data.infoHue);
    hoveredNormal->color     = CguiColorFromSLAaHsS(data.inactiveHoveredSLA, 0.0f, 0.0f);
    hoveredFlat->color       = CguiColorFromSLAaHsS(data.flatHoveredSLA, 0.0f, 0.0f);
    hoveredAccent->color     = CguiColorFromSLAaH(data.activeHoveredSLA, data.accentHue);
    hoveredDangerous->color  = CguiColorFromSLAaH(data.activeHoveredSLA, data.dangerousHue);
    hoveredWarning->color    = CguiColorFromSLAaH(data.activeHoveredSLA, data.warningHue);
    hoveredSuccess->color    = CguiColorFromSLAaH(data.activeHoveredSLA, data.successHue);
    hoveredInfo->color       = CguiColorFromSLAaH(data.activeHoveredSLA, data.infoHue);
    heldNormal->color        = CguiColorFromSLAaHsS(data.inactiveHeldSLA, 0.0f, 0.0f);
    heldFlat->color          = CguiColorFromSLAaHsS(data.flatHeldSLA, 0.0f, 0.0f);
    heldAccent->color        = CguiColorFromSLAaH(data.activeHeldSLA, data.accentHue);
    heldDangerous->color     = CguiColorFromSLAaH(data.activeHeldSLA, data.dangerousHue);
    heldWarning->color       = CguiColorFromSLAaH(data.activeHeldSLA, data.warningHue);
    heldSuccess->color       = CguiColorFromSLAaH(data.activeHeldSLA, data.successHue);
    heldInfo->color          = CguiColorFromSLAaH(data.activeHeldSLA, data.infoHue);
    disabledNormal->color    = CguiColorFromSLAaHsS(data.inactiveDisabledSLA, 0.0f, 0.0f);
    disabledFlat->color      = CguiColorFromSLAaHsS(data.flatDisabledSLA, 0.0f, 0.0f);
    disabledAccent->color    = CguiColorFromSLAaH(data.activeDisabledSLA, data.accentHue);
    disabledDangerous->color = CguiColorFromSLAaH(data.activeDisabledSLA, data.dangerousHue);
    disabledWarning->color   = CguiColorFromSLAaH(data.activeDisabledSLA, data.warningHue);
    disabledSuccess->color   = CguiColorFromSLAaH(data.activeDisabledSLA, data.successHue);
    disabledInfo->color      = CguiColorFromSLAaH(data.activeDisabledSLA, data.infoHue);

    // Remove shadow and border from flat
    flat->shadowColor         = BLANK;
    hoveredFlat->shadowColor  = BLANK;
    heldFlat->shadowColor     = BLANK;
    disabledFlat->shadowColor = BLANK;

    flat->borderColor         = BLANK;
    hoveredFlat->borderColor  = BLANK;
    heldFlat->borderColor     = BLANK;
    disabledFlat->borderColor = BLANK;
}

static void CguiCreateToggleTheme(CguiTheme *theme, CguiCrystallineThemeData data)
{
    CguiNode *toggleTemplate = CguiCreateNodeProMax(CguiTZeroSize(), TextFormat("CguiToggle #%d", ++cguiNameCounter), CGUI_COMPONENT_NODE_TYPE_TOGGLE, NULL, sizeof(CguiToggleData), NULL, sizeof(CguiToggleInstanceData));
    if (!toggleTemplate)
    {
        return;
    }

    CguiToggleData *toggleData = toggleTemplate->data;

    if (!CguiSetComponentTemplate(theme, toggleTemplate))
    {
        CguiDeleteNode(toggleTemplate);
        return;
    }

    CguiNode *boxNodeRef = CguiCreateBoxElement(0.0f, BLANK);
    if (!boxNodeRef)
    {
        CguiDeleteNode(toggleTemplate);
        return;
    }

    boxNodeRef->transformation = CguiTFillParent();

    if (!CguiInsertChildAt(toggleTemplate, boxNodeRef, CGUI_TOGGLE_COMPOSITION_BOX_NODE))
    {
        CguiDeleteNode(toggleTemplate);
        CguiDeleteNode(boxNodeRef);
        return;
    }

    if (!CguiSetComponentTemplate(theme, toggleTemplate))
    {
        CguiDeleteNode(toggleTemplate);
        return;
    }

    toggleData->transitionDuration = data.componentTransitionDuration;
    toggleData->transitionEasing   = data.componentTransitionEasing;

    CguiBoxElementData boxData;

    boxData.radii           = data.componentRadii;
    boxData.shadowDistance  = data.componentShadowDistance;
    boxData.shadowOffset    = data.componentShadowOffset;
    boxData.shadowShrink    = data.componentShadowShrink;
    boxData.shadowColor     = CguiColorFromSLAaHsS(data.componentShadowSLA, 0.0f, 0.0f);
    boxData.borderThickness = data.componentBorderThickness;
    boxData.borderColor     = CguiColorFromSLAaHsS(data.componentBorderSLA, 0.0f, 0.0f);

    CguiBoxElementData *inactive       = &toggleData->boxData;
    CguiBoxElementData *hovered        = &toggleData->hoveredBoxData;
    CguiBoxElementData *held           = &toggleData->heldBoxData;
    CguiBoxElementData *disabled       = &toggleData->disabledBoxData;
    CguiBoxElementData *active         = &toggleData->activeBoxData;
    CguiBoxElementData *activeHovered  = &toggleData->activeHoveredBoxData;
    CguiBoxElementData *activeHeld     = &toggleData->activeHeldBoxData;
    CguiBoxElementData *activeDisabled = &toggleData->activeDisabledBoxData;

    *inactive       = boxData;
    *hovered        = boxData;
    *held           = boxData;
    *disabled       = boxData;
    *active         = boxData;
    *activeHovered  = boxData;
    *activeHeld     = boxData;
    *activeDisabled = boxData;

    inactive->color       = CguiColorFromSLAaHsS(data.inactiveSLA, 0.0f, 0.0f);
    hovered->color        = CguiColorFromSLAaHsS(data.inactiveHoveredSLA, 0.0f, 0.0f);
    held->color           = CguiColorFromSLAaHsS(data.inactiveHeldSLA, 0.0f, 0.0f);
    disabled->color       = CguiColorFromSLAaHsS(data.inactiveDisabledSLA, 0.0f, 0.0f);
    active->color         = CguiColorFromSLAaH(data.activeSLA, data.accentHue);
    activeHovered->color  = CguiColorFromSLAaH(data.activeHoveredSLA, data.accentHue);
    activeHeld->color     = CguiColorFromSLAaH(data.activeHeldSLA, data.accentHue);
    activeDisabled->color = CguiColorFromSLAaH(data.activeDisabledSLA, data.accentHue);
}

CguiTheme *CguiCreateCrystallineThemeFromData(CguiCrystallineThemeData data)
{
    CguiTheme *theme = CguiCreateTheme("CguiCrystallineLightHighContrast");
    if (!theme)
    {
        return NULL;
    }

    theme->themeData = CG_MALLOC(sizeof(CguiCrystallineThemeData));
    if (!theme->themeData)
    {
        CguiDeleteTheme(theme);
        return NULL;
    }

    theme->themeDataSize   = sizeof(CguiCrystallineThemeData);
    theme->deleteThemeData = CguiDeleteCrystallineTheme;
    memcpy(theme->themeData, &data, theme->themeDataSize);

    // Component themes
    CguiCreateRootTheme(theme, data);
    CguiCreateLayerTheme(theme, data);
    CguiCreateLabelTheme(theme, data);
    CguiCreateButtonTheme(theme, data);
    CguiCreateToggleTheme(theme, data);

    return theme;
}

void CguiDeleteCrystallineTheme(CguiTheme *theme)
{
    if (!theme)
    {
        return;
    }

    CguiCrystallineThemeData *data = theme->themeData;
    if (!data)
    {
        return;
    }

    UnloadFont(data->textFont);
    UnloadFont(data->textFontItalic);
    UnloadFont(data->textFontBold);
    UnloadFont(data->textFontBoldItalic);
    UnloadFont(data->textFontLight);
    UnloadFont(data->textFontLightItalic);
}
