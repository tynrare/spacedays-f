#version 100

precision mediump float;

// Input vertex attributes (from vertex shader)
varying vec2 fragTexCoord;
varying vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform sampler2D tex_palette;
uniform vec2 resolution;
uniform float time;

float threshold = 0.6;

void main()
{
    vec2 uv = fragTexCoord;
    vec4 color = texture2D(texture0, uv);
    vec3 diff = color.xyz - vec3(1.0, 0.0, 0.0);
    float fac = 1.0 - smoothstep(threshold-0.3,threshold+0.1, dot(diff,diff));
    gl_FragColor = vec4(fac, fac, fac, 1.0);
}