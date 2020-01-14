#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;
//uniform sampler2D texture2;

uniform float     fProgress;

const float step_w = 0.0015625;
const float step_h = 0.0027778;
const float maxW   = 0.04;

void main(void)
{
    float x = TexCoord.x - maxW*sin((TexCoord.y*80.0 - fProgress*7.50));
    x = fract(x);
    vec3 color = texture2D(texture1, vec2(x, TexCoord.y)).rgb;

    gl_FragColor.a = 1.0;
    gl_FragColor.rgb = color;
}