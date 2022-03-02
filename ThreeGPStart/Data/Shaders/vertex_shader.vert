#version 330

uniform mat4 combined_xform;
uniform mat4 model_xform;
uniform vec3 CameraPosition;


layout (location=0) in vec3 vertex_position;
//layout (location=1) in vec3 vertex_colour;
layout (location = 1) in vec2 varying_texcoord;
layout(location=2) in vec3 Normals;

//out vec3 varying_colour;
out vec3 varying_CameraPostion;
out vec3 varying_Normals;
out vec2 texcord;
out vec3 FragmentPostition;

void main(void)
{
//	varying_colour = normalize(vertex_colour);
	varying_CameraPostion = CameraPosition;
	texcord = varying_texcoord;
	varying_Normals = Normals;
	FragmentPostition = vec3(model_xform*vec4(vertex_position,1.0));
	gl_Position = combined_xform * model_xform * vec4(vertex_position, 1.0);
}