#version 330 core
attribute vec3 aPos;
attribute vec3 aColor;
attribute vec2 aTexCoord;


out vec3 ourColor;
out vec2 TexCoord;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	ourColor = aColor;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
