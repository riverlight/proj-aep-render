#version 330 core


varying vec3 ourColor;
varying vec2 varTexcoord;

// texture sampler
uniform sampler2D texture1;

void main()
{
	gl_FragColor = texture(texture1, varTexcoord);
}
