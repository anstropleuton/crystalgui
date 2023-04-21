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
uniform float blurRadius;

#define TauB4 1.570796327;
const float blurQuality = 7.5;

void main()
{
    // Calculated blur radius
    float CalculatedBlurRadius = blurRadius / blurQuality * TauB4;

    vec4 blurColor = vec4(0.);

    // Mix and match colors from surrounding pixels
    // Quality defines number of pixels
    for (float a = -blurQuality; a <= blurQuality; a++)
        for (float b = -blurQuality; b <= blurQuality; b++)

            // Add all the pixel valies
            blurColor += texture2D(texture0, fragTexCoord + vec2(a * CalculatedBlurRadius, b * CalculatedBlurRadius) / resolution);

    // Divide the mixture with number of pixels
    blurColor /= 4. * blurQuality * blurQuality + 4. * blurQuality + 1.;

    gl_FragColor = blurColor*colDiffuse;
}
