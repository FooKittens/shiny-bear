#ifndef SHINYBEAR_CLUSTER_H
#define SHINYBEAR_CLUSTER_H

#include "scene\SceneNode.h"
#include "world\Block.h"

namespace shinybear 
{
  class MeshNode; class Mesh;
}

namespace shinybear
{

class Cluster : public SceneNode
{
public:
  static const int kSizeX = 32;
  static const int kSizeY = 32;
  static const int kSizeZ = 32;

  Cluster();
  ~Cluster();

  void SetBlock(const Block &newBlock, int x, int y, int z);
  const Block &GetBlock(int x, int y, int z);

  void Render(SceneManager *pScene);

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
  MeshNode *m_pMeshNode;
  Block ***m_blocks;
};

void Cluster::SetBlock(const Block &newBlock, int x, int y, int z)
{
  m_blocks[x][y][z] = newBlock;
  m_recreateMesh = true;
}

const Block &Cluster::GetBlock(int x, int y, int z)
{
  return m_blocks[x][y][z];
}

} // namespace shinybear

#endif