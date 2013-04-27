#ifndef SHINYBEAR_CLUSTER_H
#define SHINYBEAR_CLUSTER_H

#include "scene\MeshNode.h"

namespace shinybear { struct Block; }

namespace shinybear
{

class Cluster : public MeshNode
{
public:
  static const int kSizeX = 32;
  static const int kSizeY = 32;
  static const int kSizeZ = 32;

  Cluster(SceneNode *pParent);
  ~Cluster();

  void SetBlock(const Block &newBlock, int x, int y, int z);
  const Block &GetBlock(int x, int y, int z);

private:

  Block **m_blocks;
};

} // namespace shinybear

#endif