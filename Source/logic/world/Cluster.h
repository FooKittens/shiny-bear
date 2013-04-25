#ifndef SHINYBEAR_CLUSTER_H
#define SHINYBEAR_CLUSTER_H

#include "logic\world\Block.h"
#include "logic\world\WorldManager.h"
#include "logic\actor\IActor.h"
#include <d3dx9.h>

namespace shinybear
{
  class Mesh;
}

namespace shinybear
{

class Cluster : public IActor
{
public:
  friend class shinybear::WorldManager;

  const Block &GetBlock(int idxX, int idxY, int idxZ);
  void SetBlock(const Block &newBlock, int idxX, int idxY, int idxZ);
  void RemoveBlock(int indexX, int indexY, int indexZ);
  

  // For settings the clusters position.
  void SetPosition(const D3DXVECTOR3 &pos);
  void SetPosition(float x, float y, float z);

  // Fills the cluster with a certain type of blocks.
  void Fill(const Block &block);
  
  // For actor interface
  const D3DXMATRIX *GetWorldMatrix() const;
  const Mesh *GetMesh() const;

private:
  // Sizes are in block counts.
  Cluster(const D3DXVECTOR3 &position);
  Cluster();
  ~Cluster(); 

  void Setup();

  D3DXVECTOR3 m_position;
  D3DXMATRIX m_worldMat;

  Block ***m_blocks;
  Mesh *m_pMesh;
};

inline void Cluster::SetPosition(const D3DXVECTOR3 &pos)
{
  m_position = pos;
  D3DXMatrixTranslation(&m_worldMat, pos.x, pos.y, pos.z);
}

inline void Cluster::SetPosition(float x, float y, float z)
{
  SetPosition(D3DXVECTOR3(x, y, z));  
}

inline void Cluster::SetBlock(const Block &newBlock,
  int idxX, int idxY, int idxZ)
{
  m_blocks[idxX][idxY][idxZ] = newBlock;
}

inline const Mesh *Cluster::GetMesh() const
{
  return m_pMesh;
}

inline const D3DXMATRIX *Cluster::GetWorldMatrix() const
{
  return &m_worldMat;
}


} // namespace shinybear

#endif // SHINYBEAR_CLUSTER_H