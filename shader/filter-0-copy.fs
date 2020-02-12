#version 330 core


in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;


void main()
{
	//FragColor = texture(texture1, TexCoord) * vec4(ourColor, 1.0);
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);
	vec4 FragColor;
	FragColor = texture(texture1, TexCoord);
	gl_FragColor = vec4( FragColor.r, FragColor.g, FragColor.b, 1.0);
}
