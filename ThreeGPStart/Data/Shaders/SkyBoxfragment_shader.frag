#version 330

//uniform sampler2D sampler_tex;


in vec3 TexCoords;

out vec4 fragment_colour;

uniform samplerCube skybox;

void main(void)
{
	fragment_colour = texture(skybox,TexCoords);
}