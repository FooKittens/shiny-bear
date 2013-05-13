#ifndef SHINYBEAR_VERTEXFORMATS_H
#define SHINYBEAR_VERTEXFORMATS_H

#include "util\Typedefs.h"
#include "util\math\Math.h"
#include <d3dx9.h>



namespace shinybear
{

namespace VertexDecl
{
  enum E
  {
    VD_POSITION_COLOR_NORMAL = 1,
    VD_POSITION_COLOR,
    VD_POSITION,
    VD_POSITION_NORMAL_TEXTURE,
    VD_POSITION_TEXTURE,
    
    // Only used internally for render targets.
    VD_DFT,
    VD_DF
  };
}

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

// Vertex Position
struct VertexP
{
  Vector3 position;
};

// Vertex Position-Color
struct VertexPC
{
  Vector3 position;
  D3DXCOLOR color;
};

// Vertex position-texcoords
struct VertexPTx
{
  Vector3 position;
  Vector2 texcoords;
};

// Vertex Position-normal-texcoords.
struct VertexPNTx
{
  Vector3 position;
  Vector3 normal;
  Vector2 texcoords;
};

// Vertex for deferred full-screen target.
struct VertexDFT
{
  // Already transformed position.
  Vector4 position;
  Vector2 texcoords;
};

// Used to pass in transformed coordinates for a fullscreen quad.
struct VertexDF
{
  Vector4 position;
};


} // namepspace shinybear

#endif // SHINYBEAR_VERTEXFORMATS_H