#ifndef SHINYBEAR_BLOCK_H
#define SHINYBEAR_BLOCK_H

#include "graphics\VertexFormats.h"
#include "util\Typedefs.h"
#include <d3dx9.h>

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

struct Block
{
  Block(BlockType bType)
  {
    m_blockType = bType;
  }
  Block() { m_blockType = 0; }

  bool IsVisible() const { return m_isVisible; }
  void SetVisible(bool val) { m_isVisible = val; }

  BlockType GetType() const { return m_blockType; }
  void SetType(BlockType type) { m_blockType = type; }

  BlockColor GetMaterial() const { return m_color; }
  void SetMaterial(BlockColor col) { m_color = col; }

private:
  BlockType m_blockType;
  BlockColor m_color;
  bool m_isVisible;
};

} // namespace shinybear

#endif // SHINYBEAR_BLOCK_H