#version 330 core

precision highp float;

varying vec3 ourColor;
varying vec2 TexCoord;

uniform sampler2D textIn;
uniform sampler2D textColorMap;

void main()
{
    vec4 textureInValue = texture2D(textIn, TexCoord);
	float bColor = textureInValue.b * 63.0;
	vec2 quad1;
	quad1.y = floor(floor(bColor) / 8.0);
	quad1.x = floor(bColor) - (quad1.y * 8.0);
	vec2 quad2;
	quad2.y = floor(ceil(bColor) / 8.0);
	quad2.x = ceil(bColor) - (quad2.y * 8.0);
	vec2 texPos1;
	texPos1.x = (quad1.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureInValue.r);
	texPos1.y = (quad1.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureInValue.g);
	vec2 texPos2;
	texPos2.x = (quad2.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureInValue.r);
	texPos2.y = (quad2.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureInValue.g);
	
	vec4 newColor1;
	vec4 newColor2;
	newColor1 = texture2D(textColorMap, texPos1);
	newColor2 = texture2D(textColorMap, texPos2);
	vec4 newColor = mix(newColor1, newColor2, fract(bColor));
	
    gl_FragColor = clamp(newColor,0.0,1.0);
}
