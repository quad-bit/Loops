#pragma once

#include <Component.h>
#include <glm/glm.hpp>

class Texture;
class Shader;

class Material : public Component<Material>
{
public:
    glm::vec4 color;
    bool enableInstancing = false;
    Texture * mainTexture; // main color map
    glm::vec2 mainTextureScale;
    glm::vec2 mainTextureOffset;
    Texture * textures; // normal, heightmap, gloss map.. etc   
    Shader * shaderObj;
};

// unity ref
/*

color	The main color of the Material.
doubleSidedGI	Gets and sets whether the Double Sided Global Illumination setting is enabled for this material.
enableInstancing	Gets and sets whether GPU instancing is enabled for this material.
globalIlluminationFlags	Defines how the material should interact with lightmaps and lightprobes.
mainTexture	The main texture.
mainTextureOffset	The offset of the main texture.
mainTextureScale	The scale of the main texture.
passCount	How many passes are in this material (Read Only).
renderQueue	Render queue of this material.
shader	The shader used by the material.
shaderKeywords	Additional shader keywords set by this material.

*/