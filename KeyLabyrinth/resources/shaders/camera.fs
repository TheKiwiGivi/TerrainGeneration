#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 lightPos;



void main()
{

float constant = 1.0;
float linear = 1.0;
float quadratic = 0.01;

float distance    = length(lightPos - FragPos);
float attenuation = 1.0 / (constant + linear * distance + 
    		    quadratic * (distance * distance)); 

	// linearly interpolate between both textures (80% container, 20% awesomeface)
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff*vec3(1,1,1);
	diffuse  *= attenuation;

	vec3 ambient = 0.1 * vec3(1, 1,1);
	ambient *= attenuation;
	fefe

	vec3 result = (ambient + diffuse);
    FragColor = vec4(result, 1);
	//FragColor = vec4(result, 1)*(mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2));
}