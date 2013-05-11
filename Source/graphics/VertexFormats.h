#ifndef SHINYBEAR_VERTEXFORMATS_H
#define SHINYBEAR_VERTEXFORMATS_H

#include "util\Typedefs.h"
#include "util\math\Math.h"
#include <d3dx9.h>



namespace shinybear
{

struct BlockVertex
{
  D3DXVECTOR3 position;
  D3DXVECTOR3 normal;
  D3DXCOLOR color;
};

// Vertex Position-Normal-Color
struct VertexPNC
{
  Vector3 position;
  Vector3 normal;
  D3DXCOLOR color;
};

// Vertex Position-Color
struct VertexPC
{
  Vector3 position;
  D3DXCOLOR color;
};

struct VertexPositionColor
{
  D3DXVECTOR3 position;
  D3DXCOLOR color;
};


} // namepspace shinybear

#endif // SHINYBEAR_VERTEXFORMATS_H