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
uniform float shadowRadius;
uniform float shadowSize;
uniform vec2 shadowOffset;
uniform vec4 shadowColor;

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

    // Calculate the shadow opacity
    float shadowRoundedRectangle = 1. + smoothstep(.01, 0., generateRoundedRectangle(centerPosition, halfSize, roundness)) - smoothstep(0., shadowRadius, generateRoundedRectangle(centerPosition - shadowOffset, halfSize + shadowSize, roundness));

    gl_FragColor = shadowColor*shadowRoundedRectangle*colDiffuse;
}
