#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out vec2 TexCoords;

void main(void) 
{
	gl_Position = vec4(position.x, position.y, 0.0, 1.0);
	TexCoords = texCoord;
}

#FRAGMENT
#version 330 core

uniform sampler2D screenTexture;

in vec2 TexCoords;
out vec4 color;

void main(void) 
{
	color = texture(screenTexture, TexCoords);
}