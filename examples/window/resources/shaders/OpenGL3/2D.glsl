#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec2 TexCoords;

void main(void) 
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    TexCoords = texCoord;
}

#FRAGMENT
#version 330 core

in vec2 TexCoords;

uniform sampler2DArray ourTexture;
uniform int layer;
uniform int flipHorizontal;
uniform int flipVertical;
uniform float size;

out vec4 color;

void main(void) 
{    
    vec4 color_out = texture(ourTexture, vec3(TexCoords.x * size * flipHorizontal, TexCoords.y * size * flipVertical, layer));

    if(color_out.a < 0.1)
        discard;

	color = color_out;
}