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
    vec4 lightPos; 
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
} light;

layout(set = 2, binding = 1) uniform sampler2D combined_shadowSampler;

void main()
{
   // ambient
   vec3 ambient = light.ambient.xyz;
   vec3 diffuse = light.diffuse.xyz; 
   vec3 specular = light.specular.xyz; 
   vec3 lightPos = light.lightPos.xyz; 

   
   vec3 norm = normalize(normal);
   vec3 lightDir = normalize(lightPos.xyz - fragPos.xyz);
   vec3 viewDir = normalize(viewPos - fragPos.xyz);

   // diffuse 
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuseVal = diff * diffuse.xyz;  
   
   //specular       
   vec3 reflectDir = reflect(-lightDir, norm);  
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
   vec3 specularVal = spec * specular.xyz;
    
   vec2 inTextureCoordinate = vec2(0.5, 0.5);
   vec4 shadowColor = vec4(texture(combined_shadowSampler, inTextureCoordinate).rgb, 1.0);
    
   vec3 result = shadowColor.xyz + (ambient + diffuseVal + specularVal) * color.xyz;
   //vec3 result = (ambient + diffuseVal + specularVal) * color.xyz;
    
   outColor = vec4(result, 1.0); 
}
