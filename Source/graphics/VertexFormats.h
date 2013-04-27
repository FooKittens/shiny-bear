#ifndef SHINYBEAR_VERTEXFORMATS_H
#define SHINYBEAR_VERTEXFORMATS_H

#include <d3dx9.h>

namespace shinybear
{

struct BlockVertexFormat
{
  D3DXVECTOR3 position;
  //D3DXVECTOR3 normal;
  D3DXCOLOR diffuse;
  D3DXCOLOR specular;
};

} // namepspace shinybear

#endif // SHINYBEAR_VERTEXFORMATS_H