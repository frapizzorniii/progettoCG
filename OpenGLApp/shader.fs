#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

// texture sampler
uniform sampler2D ourTexture;
uniform bool isBlock;


void main()
{
	if(isBlock)
	{
		FragColor = vec4(1.0, 0.0, 0.0, 0.0);
	}
	else
	{
		FragColor = texture(ourTexture, TexCoord);
	}

}