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
    blockType = bType;
  }
  Block() { blockType = 0; }

  bool IsVisible() const { return m_isVisible; }
  void SetVisible(bool val) { m_isVisible = val; }

  BlockType GetType() const { return blockType; }
  void SetType(BlockType type) { blockType = type; }

  BlockMaterial GetMaterial() const { return blockMaterial; }
  void SetMaterial(const BlockMaterial &mat) { blockMaterial = mat; }

private:
  BlockType blockType;
  BlockMaterial blockMaterial;
  bool m_isVisible;
};

} // namespace shinybear

#endif // SHINYBEAR_BLOCK_H