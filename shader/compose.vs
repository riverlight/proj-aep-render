#version 330 core
attribute vec3 aPos;
attribute vec3 aColor;
attribute vec2 aTexCoord;

uniform mat4 uniPositionMat;

varying vec3 ourColor;
varying vec2 varTexcoord;

void main()
{
	gl_Position = uniPositionMat * vec4(aPos, 1.0);
	ourColor = aColor;
	varTexcoord = vec2(aTexCoord.x, aTexCoord.y);
}
