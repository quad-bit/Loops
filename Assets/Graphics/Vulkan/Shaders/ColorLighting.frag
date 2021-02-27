#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec4 color;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 fragPos;
layout (location = 3) in vec3 viewPos;

layout (location = 0) out vec4 outColor;

layout (std140, set = 2, binding = 0) uniform Lights
{
    vec3 lightPos; 
    vec3 lightColor;
} light;

void main()
{
   // ambient
   float ambientStrength = 0.7;
   vec3 ambient = ambientStrength * light.lightColor;
	
   // diffuse 
   vec3 norm = normalize(normal);
   vec3 lightDir = normalize(light.lightPos - fragPos.xyz);
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = diff * light.lightColor;
   
   //specular       
   float specularStrength = 0.5;
   vec3 viewDir = normalize(viewPos - fragPos.xyz);
   vec3 reflectDir = reflect(-lightDir, norm);  
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
   vec3 specular = specularStrength * spec * light.lightColor;  
    
   vec3 result = (ambient + diffuse + specular) * color.xyz;
    
   outColor = vec4(result, 1.0); 
}
