#version 330

uniform mat4 combined_xform;
uniform mat4 model_xform;

layout (location=0) in vec3 vertex_position;

out vec3 TexCoords;

void main(void)
{	
	TexCoords = vertex_position*vec3(1,-1,1);//inverting the positions so the image does not render upside down

	gl_Position = combined_xform * model_xform * vec4(vertex_position, 1.0);
}