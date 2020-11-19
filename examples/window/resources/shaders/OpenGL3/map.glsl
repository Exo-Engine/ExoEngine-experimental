#version 330 core
layout(location = 0) in vec3	position;
layout(location = 1) in vec2	texCoord;

uniform mat4	view;
uniform mat4	projection;
uniform mat4	model;

out vec4	Position;
out vec2	TexCoord;

void	main(void) 
{
	gl_Position = projection * view * model * vec4(position, 1.0);
	TexCoord = texCoord;
	Position = model * vec4(position, 1);
}

#FRAGMENT
#version 330 core

#define PI	3.1415926535

in vec4		Position;
in vec2		TexCoord;

uniform vec3			lightAmbient;
uniform vec3			lightDiffuse;
uniform mat4			lightView;
uniform mat4			lightProjection;
uniform vec3			lightPos;
uniform float			lightRadius;
uniform mat4			model;
uniform sampler2DArray	ourTexture;
uniform sampler2DArray	normalMap;
uniform sampler2D		shadowMap;
uniform int				layer;
uniform float			size;
uniform float			intensity;

out vec4	color;

void	main(void)
{
	vec4	value;
	vec4	result;
	vec4	shadowPos;
	float	depth;
	float	dist;
	float	distFactor;

	shadowPos = lightProjection * lightView * Position;
	shadowPos.xy = shadowPos.xy * 0.5 + 0.5;
	depth = texture(shadowMap, vec2(shadowPos.x, 0.5)).x;

	depth = 2 * depth - 1;

	dist = distance(lightPos, Position.xyz);
	if (dist < lightRadius)
	{
		value = intensity * texture(ourTexture, vec3(TexCoord.x * size, TexCoord.y * size, layer));

		//distFactor = sqrt(pow(lightRadius, 2.0) - pow(dist, 2.0)) / lightRadius;
		distFactor = (lightRadius - dist) / lightRadius;

		result = value * vec4(lightAmbient, distFactor);

		if (depth >= shadowPos.z)
			result += value * (0.2 + vec4(lightDiffuse, distFactor) *
				dot(
					normalize(texture(normalMap, vec3(TexCoord.x * size, TexCoord.y * size, layer)).xyz * 2.0 - 1.0),
					normalize(lightPos - Position.xyz)
					));
		else if (depth + 0.01 >= shadowPos.z)
			result += value * (1 - (shadowPos.z - depth) / 0.01) * vec4(lightDiffuse, distFactor) *
				dot(
					normalize(texture(normalMap, vec3(TexCoord.x * size, TexCoord.y * size, layer)).xyz * 2.0 - 1.0),
					normalize(lightPos - Position.xyz)
					);


		//result += vec4(vec3(0.1, 0, 0) * depth, 1);
		//result += vec4(vec3(0, 0.1, 0) * shadowPos.x, 1);
		//result += vec4(vec3(0, 0, 0.1) * shadowPos.z, 1);

		color = result;
	}
	else
		color = vec4(1, 0, 0, 0);
}