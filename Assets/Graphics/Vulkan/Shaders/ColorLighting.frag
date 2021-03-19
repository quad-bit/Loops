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
    mat4 lightSpaceMat;
} light;

layout(set = 2, binding = 1) uniform sampler2D combined_shadowSampler;


float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    
         ////////  HACK HACK HACK HACK REMOVE this..!!!!! ////////
    projCoords = vec3(projCoords.x, 1-projCoords.y, projCoords.z);

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(combined_shadowSampler, projCoords.xy).r; 
    
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(normal);
    vec3 lightDir = normalize(light.lightPos - fragPos).xyz;
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(combined_shadowSampler, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(combined_shadowSampler, projCoords.xy + vec2(x, y) * texelSize).r; 
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
   // ambient
   vec3 ambient = light.ambient.xyz;
   vec3 diffuse = light.diffuse.xyz; 
   vec3 specular = light.specular.xyz; 
   vec3 lightPos = light.lightPos.xyz; 

   vec4 fragPosLightSpace = light.lightSpaceMat * vec4(fragPos.xyz, 1.0);
   float shadow = ShadowCalculation(fragPosLightSpace); 

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
   
   vec3 result = (ambient + (1.0 - shadow) * (diffuseVal + specularVal)) * color.xyz;
    
   outColor = vec4(result, 1.0); 
}
