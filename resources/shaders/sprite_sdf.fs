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

float threshold = 0.74;

float edge = 0.01;
float innerline = 0.1;

vec4 colora = vec4(0.89, 0.1, 0.6, 0.99);
vec4 colorb = vec4(0.7, 0.4, 0.16, 0.29);

vec4 colorc = vec4(0.1, 0.1, 0.1, 0.9);

vec4 sdf0() {
    vec4 texel = texture2D(texture0, fragTexCoord); 
    float r = texel.r * fragColor.r;
    
    vec4 texel1 = texture2D(texture0, 
        vec2(fragTexCoord.x + texel.g / 1024.0, fragTexCoord.y));
    
    r = r * texel1.r;
    
    float alpha = smoothstep(threshold, threshold + edge, r);
    
    vec4 color = colora;
    color = mix(color, colorb, alpha);
    color.a = min(alpha, color.a);
    
    return color;
}

vec4 sdf1() {
    vec2 uv = fragTexCoord;
    vec4 texel = texture2D(texture0, fragTexCoord); 

    float a = distance(texel.r, 1.0) * 1.0;
    float alpha = smoothstep(threshold, threshold, a);
    
    vec4 colora = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 colorb = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 color = mix(colora, colorb, alpha);
    return color;
}

void main()
{
    vec4 color = sdf1();
    gl_FragColor = color;

    //gl_FragColor = texture2D(texture0, fragTexCoord);
}