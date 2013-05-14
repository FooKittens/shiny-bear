//#ifndef TERRAINGENERATOR_H
//#define TERRAINGENERATOR_H
//
//#include <noise\noise.h>
//#include <graphics\VertexFormats.h>
//
//namespace shinybear
//{
//  class SceneNode; class GraphicsProvider;
//  class Cluster;
//}
//
//class TerrainGenerator
//{
//public:
//  TerrainGenerator(shinybear::GraphicsProvider *pProvider,
//    shinybear::SceneNode *pRootNode);
//  ~TerrainGenerator();
//
//  void Generate(const int x, const int z);
//private:
//  int GetHeight(int x, int z);
//
//  shinybear::BlockColor GetMaterialHeight(int y);
//
//  // Configures a cluster with a certain height.
//  // If the height is above the cluster it will 
//  // attach a new cluster to this cluster.
//  void SetupCluster(shinybear::Cluster *pCluster,
//    int clusterX, int clusterZ, int clusterY,
//    const int genX, const int genZ);
//
//  shinybear::SceneNode *m_pTerrainNode;
//  shinybear::GraphicsProvider *m_pProvider;
//  noise::module::Perlin m_perlin;
//  
//  shinybear::BlockColor m_grassMat;
//  shinybear::BlockColor m_stoneMat;
//  shinybear::BlockColor m_dirtMat;
//  shinybear::BlockColor m_snowMat;
//};

//#endif // TERRAINGENERATOR_H