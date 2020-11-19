#version 330 core

#define PI	3.1415926535

layout(location = 0) in vec3	position;

uniform mat4	lightView;
uniform mat4	lightProjection;
uniform mat4	model;

void	main(void)
{
	gl_Position = lightProjection * lightView * model * vec4(position, 1.0);
	gl_Position.y = 0;
}

#FRAGMENT
#version 330 core

// Ouput data
out float	depth;

void	main(void)
{
	depth = gl_FragCoord.z;
}
