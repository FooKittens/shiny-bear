#ifndef SHINYBEAR_BLOCK_H
#define SHINYBEAR_BLOCK_H

#include "graphics\VertexFormats.h"
#include "util\Typedefs.h"
#include <d3dx9.h>

namespace shinybear
{

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