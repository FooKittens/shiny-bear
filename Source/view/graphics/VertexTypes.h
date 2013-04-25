#ifndef SHINYBEAR_VERTEXTYPES_H
#define SHINYBAER_VERTEXTYPES_H

#include <d3dx9.h>

namespace shinybear
{

struct BlockVertex
{
  BlockVertex(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &diffuse,
    const D3DXVECTOR3 &specular)
    :position(pos), diffuse(diffuse), specular(specular) { }

  D3DXVECTOR3 position;
  D3DXCOLOR diffuse;
  D3DXCOLOR specular;
};

} // namespace shinybear

#endif // SHINYBEAR_VERTEXTYPES_H
