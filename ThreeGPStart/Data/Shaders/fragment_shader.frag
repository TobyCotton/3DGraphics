#version 330

//uniform vec4 diffuse_colour;
uniform sampler2D sampler_tex;

in vec2 texcord;
in vec3 varying_Normals;
in vec3 FragmentPostition;
in vec3 varying_CameraPostion;

out vec4 fragment_colour;
//out vec3 tex_colour;

void main(void)
{
	vec3 colour = vec3(1.0f,1.0f,1.0f);//a white light
	float specularStrength = 0.5;
	vec3 viewDirection = normalize(varying_CameraPostion-FragmentPostition);//calculates the normal from the camera and fragment position
	vec3 normals = normalize(varying_Normals);//renormalizing normals
	vec3 SpotlightPosition = vec3(2000,300 , 1000);
	vec3 SpotlightDirection = normalize(SpotlightPosition - FragmentPostition);
	vec3 lightDirection = vec3(0.45,0.45,1);//the directional lights direction

	vec3 SpotlightreflectDirection = reflect(-SpotlightDirection,normals);
	vec3 DirectionallightDirection = reflect(-lightDirection,normals);

	float specularSpot = pow(max(dot(viewDirection,SpotlightreflectDirection),0.0),32);
	float specularDirectional = pow(max(dot(viewDirection,DirectionallightDirection),0.0),32);
	vec3 spotSpec = specularStrength*specularSpot*colour;
	vec3 dirSpec = specularStrength*specularDirectional*colour;

	float Directionaldiff = max(dot(normals, lightDirection), 0.0);
	float diff = max(dot(normals, SpotlightDirection), 0.0);

	vec3 DirectionalDiffuse = Directionaldiff*colour;
	vec3 diffuse = diff * colour;
	vec3 ambient = 0.5 * colour ;
	vec3 tex_colour = (texture(sampler_tex, texcord).rgb)*(diffuse+ambient+DirectionalDiffuse+dirSpec+spotSpec);
	fragment_colour = vec4(tex_colour,1.0);
}