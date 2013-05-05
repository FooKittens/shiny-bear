#ifndef SHINYBEAR_VERTEXFORMATS_H
#define SHINYBEAR_VERTEXFORMATS_H

#include "util\Typedefs.h"
#include <d3dx9.h>
#include <math.h>


namespace shinybear
{

struct BlockMaterial
{
  BlockColor diffuse;
  BlockColor specular;

  void GetColors(D3DXCOLOR * const pDiffuse, D3DXCOLOR * const pSpecular) const
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

#pragma pack(push, 1)

struct BlockVertex
{
  D3DXVECTOR3 position;
  D3DXVECTOR3 normal;
  D3DXCOLOR diffuse;
  D3DXCOLOR specular;
};

#pragma pack(pop)


} // namepspace shinybear

#endif // SHINYBEAR_VERTEXFORMATS_H