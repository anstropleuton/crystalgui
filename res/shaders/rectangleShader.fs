#version 100

precision mediump float;

// Input vertex attributes (from vertex shader)
varying vec2 fragTexCoord;
varying vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Render resolution
uniform vec2 resolution;
uniform vec4 rectangle;
uniform float roundness;
uniform vec4 rectangleTint;

// The documentation of how this function works is provided by this link
// https://iquilezles.org/articles/distfunctions
// https://www.shadertoy.com/view/WtdSDs
float generateRoundedRectangle(vec2 centerPosition, vec2 halfSize, float radius)
{
    if (min(halfSize.x, halfSize.y) < radius) radius = min(halfSize.x, halfSize.y);
    return length(max(abs(centerPosition) - halfSize + radius, 0.)) - radius;
}

void main()
{
    vec2 halfSize = rectangle.zw / 2.;
    vec2 centerPosition = fragTexCoord * resolution - rectangle.xy - halfSize;

    // Calculate if the position is inside rectangle
    float roundedRectangle = smoothstep(0.01, 0., generateRoundedRectangle(centerPosition, halfSize, roundness));

    vec4 tint = vec4(rectangleTint.rgb * rectangleTint.a, rectangleTint.a) * roundedRectangle;
    vec4 texel = vec4(texture2D(texture0, fragTexCoord).rgb * roundedRectangle, roundedRectangle) * (1. - tint.a);

    gl_FragColor = (texel + tint) * colDiffuse;
}
