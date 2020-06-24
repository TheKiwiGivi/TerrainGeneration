#version 330 core
out vec4 FragColor;

//from vs
in vec2 TexCoord;
in vec3 Normal;
in vec4 FragPosLightSpace;
in vec3 FragPos;
// textures
uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

//position of all light sources(torches)
uniform vec3 lightPos[50];

uniform bool sunColor;
uniform bool sunActive;

//looping though all light sources
uniform int totalTorches;
uniform vec3 viewPos;
uniform vec3 ambientColor;

uniform bool playerAttOn;
uniform float playerAtt;

//uniform vec3 lightPos;

float ShadowCalculation(vec4 fragPosLightSpace, int i)
{
	
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos[i] - FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}


void main()
{
//vec3 color = texture(diffuseTexture, TexCoord).rgb;
//used to calculate light dimming according to distance
float constant = 1.0;
float linear = 0.9;
float quadratic = 10.01;

float playerPower = 1.0f;
 
vec3 normal = normalize(Normal);

	//used to sum up all light sources combined intensity
	vec3 lightColor = vec3(0.3);
	float shadow = 0.0;
	vec3 diffuseTotal = vec3(0,0,0);
	vec3 specularTotal = vec3(0,0,0);

	

	for(int i = 0; i < totalTorches; i++){
		if((i == 0 && sunActive) || i !=0){


		//distance
		float distance = length(lightPos[i] - FragPos);

	//use distance to calculate dimming effect
	//float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));
	float attenuation = 1.0;
	if(i != 0){
		attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));
	}

	if(i == 1 && !playerAttOn) playerPower = 0.0f;
	if(i == 1 && playerAttOn) {
	playerPower = playerAtt*10;
	}
	else {
		playerPower = 1.0f;
	}

	//diff and spec

	 vec3 lightDir = normalize(lightPos[i] - FragPos);
	 float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * vec3(0.3);
	diffuse  *= attenuation;
	diffuseTotal += diffuse*playerPower;

	//vec3 viewDir = normalize(viewPos - FragPos);
	//vec3 reflectDir = reflect(-lightDir, normal);

	//float spec = 0.0;
    //vec3 halfwayDir = normalize(lightDir + viewDir); 

	//spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
	//vec3 specular = spec * lightColor;
	//specularTotal += specular*attenuation;
	//shadow += ShadowCalculation(FragPosLightSpace, i);
	

	// specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    specularTotal += specularStrength * spec * lightColor * attenuation * playerPower;  
	//looping through all torches and adding up diffuse and specular lighting
	}
	}
	
	
	//if(sunColor)ambient = vec3(1.0);

	//add everything together
	
	//vec3 result = (ambient);
    //for debugging light: FragColor = vec4(result, 1);
	//add textures
	//FragColor = vec4(result, 1)*texture(texture1, TexCoord);}
	vec3 ambient = vec3(0.1);
	//if(sunColor) ambient * 
	if(sunColor) ambient = vec3(1);
	
	//ambient = 0.05 * vec3(1, 1, 1);

	//float shadow = ShadowCalculation(FragPosLightSpace);                      
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuseTotal + specularTotal)) * 0.5;

	//coloring
	vec3 result = (ambient + diffuseTotal + specularTotal);
	//result *= ambientColor;
	FragColor = vec4(result*ambientColor, 1);}