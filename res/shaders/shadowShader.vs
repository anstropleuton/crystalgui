#version 100

// Input vertex attributes
attribute vec3 vertexPosition;
attribute vec2 vertexTexCoord;
attribute vec3 vertexNormal;
attribute vec4 vertexColor;

// Input uniform values
uniform mat4 mvp;

// Output vertex attributes (to fragment shader)
varying vec2 fragTexCoord;
varying vec4 fragColor;

// Render resolution
uniform vec2 resolution;

uniform vec4 rectangle;
uniform float roundness;
uniform float shadowRadius;
uniform float shadowSize;
uniform vec2 shadowOffset;
uniform vec4 shadowColor;

void main()
{
    fragTexCoord = vertexTexCoord;
    fragColor = vertexColor;

    gl_Position = mvp*vec4(vertexPosition, 1.0);
}
