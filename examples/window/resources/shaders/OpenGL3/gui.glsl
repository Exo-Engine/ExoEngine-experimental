#version 330 core
layout (location = 0) in vec2 position;

uniform mat4 transformation;
uniform mat4 projection;

out vec2 TexCoords;

void main(void)
{
    gl_Position = projection * transformation * vec4(position, 0.0, 1.0);
    TexCoords = vec2((position.x + 1.0) / 2, 1 - (-1 * position.y + 1.0) / 2.0);
}

#FRAGMENT
#version 330 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D guiTexture;
uniform float opacity;
uniform float numberOfRows;
uniform float numberOfColumns;
uniform vec2 offset;

void main(void)
{    
    color = texture(guiTexture, vec2(TexCoords.x / numberOfRows, TexCoords.y / numberOfColumns) + offset);
    color.a = color.a - opacity;
}