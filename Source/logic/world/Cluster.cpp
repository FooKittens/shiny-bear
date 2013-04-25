#include "logic\world\Cluster.h"
#include "logic\world\WorldManager.h"
#include "util\SBUtil.h"
#include "view\graphics\Mesh.h"

namespace shinybear
{

struct Material
{
  const char *pName;
  D3DCOLOR specular;
  D3DCOLOR diffuse;
  D3DCOLOR vertex;
};

Cluster::Cluster(const D3DXVECTOR3 &position)
{
#define BLOCKTYPE_SAND 23
  SetPosition(position);
  Setup();
}

Cluster::Cluster()
{
  SetPosition(0, 0, 0);
  Setup();
}

Cluster::~Cluster()
{
  for(int x = 0; x < WorldManager::kClusterSizeX; ++x)
  {
    for(int y = 0; y < WorldManager::kClusterSizeY; ++y)
    {
      delete[] m_blocks[x][y];
    }
    delete[] m_blocks[x];
  }

  delete[] m_blocks;

  delete m_pMesh;
}

void Cluster::Setup()
{
  m_blocks = DBG_NEW Block**[WorldManager::kClusterSizeX];
  for(int x = 0; x < WorldManager::kClusterSizeX; ++x)
  {
    m_blocks[x] = DBG_NEW Block*[WorldManager::kClusterSizeY];
    for(int y = 0; y < WorldManager::kClusterSizeY; ++y)
    {
      m_blocks[x][y] = DBG_NEW Block[WorldManager::kClusterSizeZ];
    }
  }

  m_pMesh = DBG_NEW Mesh();
}

void Cluster::Fill(const Block &block)
{
  for(int x = 0; x < WorldManager::kClusterSizeX; ++x)
    for(int y = 0; y < WorldManager::kClusterSizeY; ++y)
      for(int z = 0; z < WorldManager::kClusterSizeZ; ++z)
      {
        m_blocks[x][y][z] = block;
      }

}






} // namespace shinybear