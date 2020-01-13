#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;
//uniform sampler2D texture2;

void main()
{
	//FragColor = texture(texture1, TexCoord) * vec4(ourColor, 1.0);
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);
	FragColor = texture(texture1, TexCoord);
	//FragColor = vec4(vec3(1.0 - texture(texture1, TexCoord)), 1.0);
}
