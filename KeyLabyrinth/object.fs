#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;

//position of all light sources(torches)
uniform vec3 lightPos[50];

//looping though all light sources
uniform int totalTorches;
uniform vec3 viewPos;

void main()
{    

//used to calculate light dimming according to distance
	float constant = 1.0;
	float linear = 0.7;
	float quadratic = 10.92;
 
	vec3 norm = normalize(Normal);

	//used to sum up all light sources combined intensity
	vec3 totalDiffuse2 = vec3(0,0,0);
	vec3 totalSpecular2 = vec3(0,0,0);

	//intensity of specular light from torches
	float specularStrength = 0.5;
	
	//looping through all torches and adding up diffuse and specular lighting
	//TODO: find a way to only add from light sources that do not collide with walls before reaching
	for(int i = 0; i < totalTorches; i++){
	//the distance is the length of the vector (light position - fragment position)
	float distance = length(lightPos[i] - FragPos);

	//use distance to calculate dimming effect
	float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));
	
	//calculate light direction
	vec3 lightDir = normalize(FragPos-lightPos[i]);

	//use the direction and the normal to calculate the angle, to find out the diffuse intensity
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff*vec3(1,1,1);
	//apply dimming effect to the result and add to total
	diffuse  *= attenuation;
	totalDiffuse2 += diffuse;

	//specular
	//calculating view direction by using player position and current fragment position
    vec3 viewDir = normalize(viewPos - FragPos);


    vec3 reflectDir = normalize(reflect(lightDir, normalize(norm)));  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * vec3(1,1,1);
	specular *= attenuation;
	totalSpecular2 += specular;
	}

	//add basic ambient lighting. fairly low to make torches more meaningful
	vec3 ambient = (0.2 * vec3(1, 1, 1))*totalTorches;

	//add everything together
	vec3 result = (ambient + totalDiffuse2 + totalSpecular2);

    FragColor = vec4(result, 1)*texture(texture_diffuse1, TexCoords);
}
