#ifndef SHINYBEAR_BLOCK_H
#define SHINYBEAR_BLOCK_H

#include <d3dx9.h>

namespace shinybear
{

typedef unsigned int BlockType;
typedef unsigned int UIntColor;

struct BlockMaterial
{
  BlockMaterial(UIntColor diffuse, UIntColor specular)
    :diffuse(diffuse), specular(specular) { }
  BlockMaterial() { }

  UIntColor diffuse;
  UIntColor specular;

  void GetColors(D3DXCOLOR * const pDiffuse, D3DXCOLOR * const pSpecular)
  {
    pDiffuse->a = static_cast<float>((diffuse) >> 24) / 255.0f;
    pDiffuse->r = static_cast<float>((diffuse) >> 16) / 255.0f;
    pDiffuse->g = static_cast<float>((diffuse) >> 8) / 255.0f;
    pDiffuse->b = static_cast<float>((diffuse) >> 0) / 255.0f;

    pSpecular->a = static_cast<float>((specular) >> 24) / 255.0f;
    pSpecular->r = static_cast<float>((specular) >> 16) / 255.0f;
    pSpecular->g = static_cast<float>((specular) >> 8) / 255.0f;
    pSpecular->b = static_cast<float>((specular) >> 0) / 255.0f;
  }
};

struct Block
{
public:
  Block(BlockType btype)
  {
    blockType = btype;
  }
  Block() { blockType = 0; }

  BlockType blockType;
  BlockMaterial material;  
};

}
#endif // SHINYBEAR_BLOCK_H