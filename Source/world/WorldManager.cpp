#include "world\WorldManager.h"
#include "world\Cluster.h"
#include "util\SBUtil.h"

namespace shinybear
{

WorldManager::WorldManager(GraphicsProvider *pProvider, Renderer *pRenderer)
{
  m_pProvider = pProvider;
  m_pRenderer = pRenderer;
}

WorldManager::~WorldManager()
{
  DeleteAll();
}

void WorldManager::DeleteAll()
{
  for(auto it = m_clusters.begin(); it != m_clusters.end(); ++it)
  {
    delete *it;
  }

  m_clusters.clear();
}

void WorldManager::Generate(int sizeX, int sizeZ)
{
  for(int countX = 0; countX < sizeX; ++countX)
  {
    for(int countZ = 0; countZ < sizeZ; ++countZ)
    {
      float tx = countX - sizeX / 2;
      float tz = countZ - sizeZ / 2;

      Cluster *cluster = DBG_NEW Cluster(m_pProvider);
      cluster->m_world = Mat4x4::CreateTranslation(
        tx * Cluster::kSizeX,
        0,
        tz * Cluster::kSizeZ
      );

      // Add cluster to drawable list.
      m_pRenderer->AddDrawable(cluster);

      m_clusters.push_back(cluster);
    }
  }
}


void WorldManager::Update(double elapsedSeconds)
{

}

} // namespace shinybear