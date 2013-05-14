#ifndef SHINYBEAR_CLUSTER_H
#define SHINYBEAR_CLUSTER_H

#include "resource\types\IGraphicsResource.h"
#include "graphics\render\IDrawable.h"
#include "world\Block.h"

namespace shinybear 
{
  class Mesh; class WorldManager;
  class GraphicsProvider;
}

namespace shinybear
{

class Cluster : public IDrawable, public IGraphicsResource
{
public:
  friend class WorldManager;

  static const int kSizeX = 16;
  static const int kSizeY = 16;
  static const int kSizeZ = 16;

  Cluster(GraphicsProvider *pProvider);
  ~Cluster();

  void SetBlock(const Block &newBlock, int x, int y, int z);
  Block *GetBlock(int x, int y, int z);

  void Render(GraphicsProvider *pProvider);

  // For IGraphicsResource interface.
  void OnDeviceLost();
  void OnDeviceReset(GraphicsProvider *pProvider);

  // For IDrawable Interface
  const Mat4x4 &GetWorldMatrix() const { return m_world; }
  float GetBoundingRadius() const { return static_cast<float>(kSizeX); }
  RenderPass::E GetRenderPass() const { return RenderPass::DEFERRED; }

private:
  enum HideFlags
  {
    HF_FRONT = 1,
    HF_BACK = 2,
    HF_LEFT = 4,
    HF_RIGHT = 8,
    HF_TOP = 16,
    HF_BOTTOM = 32
  };

  void RecreateMesh();
  void CreateCube(float x, float y, float z,
    const Block &block, Mesh *pMesh, int hideFlags);

  bool m_recreateMesh;
  Mesh *m_pMesh;
  Block ***m_blocks;

  // Cluster world matrix.
  Mat4x4 m_world;
};

inline void Cluster::SetBlock(const Block &newBlock, int x, int y, int z)
{
  m_blocks[x][y][z] = newBlock;
  m_recreateMesh = true;
}

inline Block *Cluster::GetBlock(int x, int y, int z)
{
  return &m_blocks[x][y][z];
}

} // namespace shinybear

#endif