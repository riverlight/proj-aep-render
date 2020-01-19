#version 330 core
attribute vec3 aPos;
attribute vec3 aColor;
attribute vec2 aTexCoord;


varying vec3 ourColor;
varying vec2 varTexcoord;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	ourColor = aColor;
	varTexcoord = vec2(aTexCoord.x, aTexCoord.y);
}
