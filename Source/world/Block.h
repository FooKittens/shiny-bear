#ifndef SHINYBEAR_BLOCK_H
#define SHINYBEAR_BLOCK_H

#include <d3dx9.h>

namespace shinybear
{

typedef unsigned int BlockType;
typedef unsigned int BlockColor;

struct BlockMaterial
{
  BlockColor diffuse;
  BlockColor specular;

  void GetColors(D3DXCOLOR * const pDiffuse, D3DXCOLOR * const pSpecular)
  {
    pDiffuse->a = static_cast<float>((diffuse >> 24) & 0xFF) / 255.0f;
    pDiffuse->r = static_cast<float>((diffuse >> 16) & 0xFF) / 255.0f;
    pDiffuse->g = static_cast<float>((diffuse >> 8) & 0xFF) / 255.0f;
    pDiffuse->b = static_cast<float>((diffuse >> 0) & 0xFF) / 255.0f;

    pSpecular->a = static_cast<float>((specular >> 24) & 0xFF) / 255.0f;
    pSpecular->r = static_cast<float>((specular >> 16) & 0xFF) / 255.0f;
    pSpecular->g = static_cast<float>((specular >> 8) & 0xFF) / 255.0f;
    pSpecular->b = static_cast<float>((specular >> 0) & 0xFF) / 255.0f;
  }
};

struct Block
{
  Block(BlockType bType)
  {
    blockType = bType;
  }

  BlockType blockType;
  BlockMaterial blockMaterial;
};

} // namespace shinybear

#endif // SHINYBEAR_BLOCK_H