#version 330

// Note: SDF by Iñigo Quilez is licensed under MIT License

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec4      colDiffuse;

// Output fragment color
out vec4 finalColor;

uniform vec4 rectangle; // Rectangle dimensions (x, y, width, height)
uniform vec4 radii;     // Corner radii (top-left, top-right, bottom-left, bottom-right)
uniform vec4 color;
uniform int  useTexture; // Raylib doesn't have bool... ???

// Shadow parameters
uniform float shadowDistance;
uniform vec2  shadowOffset;
uniform float shadowShrink;
uniform vec4  shadowColor;
uniform int   useShadowTexture;

// Border parameters
uniform float borderThickness;
uniform vec4  borderColor;
uniform int   useBorderTexture;

// Create a rounded rectangle using signed distance field
// Thanks to Iñigo Quilez (https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm)
// And thanks to inobelar (https://www.shadertoy.com/view/fsdyzB) for shader
// MIT License
float BoxSDF(vec2 fragCoord, vec2 center, vec2 halfSize, vec4 radii)
{
    vec2 fragFromCenter = fragCoord - center;

    // Scale overlapping radii down proportionally
    float maxTop    = radii.x + radii.y;
    float maxBottom = radii.z + radii.w;
    float maxLeft   = radii.x + radii.z;
    float maxRight  = radii.y + radii.w;

    float scaleTop    = (maxTop > 2.0 * halfSize.x) ? (2.0 * halfSize.x) / maxTop : 1.0;
    float scaleBottom = (maxBottom > 2.0 * halfSize.x) ? (2.0 * halfSize.x) / maxBottom : 1.0;
    float scaleLeft   = (maxLeft > 2.0 * halfSize.y) ? (2.0 * halfSize.y) / maxLeft : 1.0;
    float scaleRight  = (maxRight > 2.0 * halfSize.y) ? (2.0 * halfSize.y) / maxRight : 1.0;

    // Use the most restrictive scale to preserve shape consistency
    float radiiScale = min(min(scaleTop, scaleBottom), min(scaleLeft, scaleRight));

    radii *= radiiScale;

    // Define the 4 corners positions relative to center
    vec2 topLeft     = vec2(-halfSize.x, halfSize.y);
    vec2 topRight    = vec2(halfSize.x, halfSize.y);
    vec2 bottomLeft  = vec2(-halfSize.x, -halfSize.y);
    vec2 bottomRight = vec2(halfSize.x, -halfSize.y);

    // Compute adjusted distances to each corner from their radius
    float distTL = length(fragFromCenter - topLeft) - radii.x;
    float distTR = length(fragFromCenter - topRight) - radii.y;
    float distBL = length(fragFromCenter - bottomLeft) - radii.z;
    float distBR = length(fragFromCenter - bottomRight) - radii.w;

    // Find the corner with the smallest adjusted distance
    float minDist       = 1.0 / 0.0;
    float nearestRadius = 0.0;

    if (distTL < minDist)
    {
        minDist       = distTL;
        nearestRadius = radii.x;
    }
    if (distTR < minDist)
    {
        minDist       = distTR;
        nearestRadius = radii.y;
    }
    if (distBL < minDist)
    {
        minDist       = distBL;
        nearestRadius = radii.z;
    }
    if (distBR < minDist)
    {
        minDist       = distBR;
        nearestRadius = radii.w;
    }

    // Calculate signed distance field
    vec2 dist = abs(fragFromCenter) - halfSize + radii.x;
    return min(max(dist.x, dist.y), 0.0) + length(max(dist, 0.0)) - radii.x;
}

vec4 AlphaBlendOver(vec4 top, vec4 bottom)
{
    // Premultiply
    vec4 pTop    = vec4(top.rgb * top.a, top.a);
    vec4 pBottom = vec4(bottom.rgb * bottom.a, bottom.a);

    // Blend
    vec4 result;
    result.a = pTop.a + pBottom.a * (1.0 - pTop.a);

    // Un-premultiply
    if (result.a > 0.0)
    {
        result.rgb = (pTop.rgb + pBottom.rgb * (1.0 - pTop.a));
        result.rgb /= result.a;
    }
    else
    {
        result.rgb = vec3(0.0);
    }

    return result;
}

void main()
{
    // Texel color fetching from texture sampler
    vec4 texelColor       = texture(texture0, fragTexCoord);
    vec4 shadowTexelColor = texture(texture1, fragTexCoord);
    vec4 borderTexelColor = texture(texture2, fragTexCoord);

    // Requires fragment coordinate in pixels
    vec2 fragCoord = gl_FragCoord.xy;

    // Calculate signed distance field for rounded rectangle
    vec2  halfSize = rectangle.zw * 0.5;
    vec2  center   = rectangle.xy + halfSize;
    float recSDF   = BoxSDF(fragCoord, center, halfSize, radii);

    // Calculate signed distance field for rectangle shadow
    vec2  shadowHalfSize = halfSize - shadowShrink;
    vec2  shadowCenter   = center + shadowOffset;
    float shadowSDF      = BoxSDF(fragCoord, shadowCenter, shadowHalfSize, radii);

    // Caculate alpha factors
    float recFactor    = smoothstep(1.0, 0.0, recSDF);
    float shadowFactor = smoothstep(shadowDistance, 0.0, shadowSDF);
    float borderFactor = smoothstep(0.0, 1.0, recSDF + borderThickness) * recFactor;

    // Multiply each color by its respective alpha factor
    vec4 factoredColor  = vec4(color.rgb, color.a * clamp(recFactor, 0.0, 1.0));
    vec4 factoredShadowColor = vec4(shadowColor.rgb, shadowColor.a * clamp(shadowFactor, 0.0, 1.0));
    vec4 factoredBorderColor = vec4(borderColor.rgb, borderColor.a * clamp(borderFactor, 0.0, 1.0));

    // Multiply texel color by its respective tint colors
    if (useTexture == 1) factoredColor = texelColor * factoredColor;
    if (useShadowTexture == 1) factoredShadowColor = shadowTexelColor * factoredShadowColor;
    if (useBorderTexture == 1) factoredBorderColor = borderTexelColor * factoredBorderColor;

    // Combine colors
    vec4 combinedColors = AlphaBlendOver(factoredBorderColor, AlphaBlendOver(factoredColor, factoredShadowColor));

    finalColor = combinedColors;
}
