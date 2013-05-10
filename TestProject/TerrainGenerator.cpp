#include "TerrainGenerator.h"


#include <scene\SceneNode.h>
#include <world\Cluster.h>
#include <util\SBUtil.h>

using namespace shinybear;

TerrainGenerator::TerrainGenerator(GraphicsProvider *pProvider, SceneNode *pRoot)
{
  m_pProvider = pProvider;

  // Create a root for our terrain.
  m_pTerrainNode = DBG_NEW SceneNode();

  // Attach the terrain to the root of the scene graph.
  pRoot->Attach(m_pTerrainNode);

  m_perlin.SetFrequency(.5);
  m_perlin.SetLacunarity(1.5);
  m_perlin.SetOctaveCount(4);
  m_perlin.SetPersistence(0.1);
  m_perlin.SetSeed(1337);

  m_grassMat  = 0x07117711;
  m_stoneMat  = 0x02555555;
  m_dirtMat   = 0x05534038;
  m_snowMat   = 0xFF888888;
}

TerrainGenerator::~TerrainGenerator()
{

}

void TerrainGenerator::Generate(const int genX, const int genZ)
{
  // Make sure we clear out any current clusters.
  m_pTerrainNode->RemoveChildren();

  float x = -(genX / 2.0f);
  float z = -(genZ / 2.0f);
  float y = 0.0f;

  for(int countX = 0; countX < genX; ++countX)
  {
    for(int countZ = 0; countZ < genZ; ++countZ)
    {
      Cluster *cluster = DBG_NEW Cluster(m_pProvider);
      m_pTerrainNode->Attach(cluster);
      SetupCluster(cluster, countX, countZ, 0, genX, genZ);
    }
  }

}

void TerrainGenerator::SetupCluster(Cluster *pCluster, int cx, int cz, int cy,
  const int genX, const int genZ)
{
  // Retrieve World-space Block index.
  int offsetX = cx * Cluster::kSizeX;
  int offsetZ = cz * Cluster::kSizeZ;
  int offsetY = cy * Cluster::kSizeY;

  // build higher or lower?
  // Used to avoid creating loads of clusters above or below.
  bool builtAbove = false, builtBelow = false;

  for(int x = 0; x < Cluster::kSizeX; ++x)
  {
    for(int z = 0; z < Cluster::kSizeZ; ++z)
    {
      int y = GetHeight(x + offsetX, z + offsetZ) - offsetY;
      
      // Height is above this clusters limit?
      if(y >= Cluster::kSizeY && !builtAbove)
      {
        // We've now created a cluster above us.
        builtAbove = true;

        // Create a new cluster..
        Cluster *pStacked = DBG_NEW Cluster(m_pProvider);

        // Attach the new cluster to this cluster.
        m_pTerrainNode->Attach(pStacked);

        // And configure it recursively..
        SetupCluster(pStacked, cx, cz, cy + 1, genX, genZ);
      }

      for(int h = 0; h < Cluster::kSizeY; ++h)
      {
        if(h >= y)
        {
          // Hide blocks above the visibility limit.
          pCluster->GetBlock(x, h, z)->SetVisible(false);
        }
        else
        {
          pCluster->GetBlock(x, h, z)->SetMaterial(GetMaterialHeight(h + offsetY));
        }
      }
    }
  }

  float tx = cx - genX / 2;
  float ty = cy;
  float tz = cz - genZ / 2;

  // Translate the cluster into the correct position.
  pCluster->Translate(tx * Cluster::kSizeX, ty * Cluster::kSizeY, tz * Cluster::kSizeZ);
}

int TerrainGenerator::GetHeight(int x, int z)
{
  double val = (m_perlin.GetValue(x / 32.0f, 0, z / 32.0f) + 1) * 4.0f;

  return static_cast<int>(max(val, 1));
}

BlockColor TerrainGenerator::GetMaterialHeight(int y)
{
  if(y < 5 || (y > 12 && y < 18))
  {
    return m_dirtMat;
  }
  else if(y < 15)
  {
    return m_grassMat;
  }
  else if(y < 23)
  {
    return m_stoneMat;
  }
  else
  {
    return m_snowMat;
  }
}