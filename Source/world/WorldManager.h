#ifndef SHINYBEAR_WORLDMANAGER_H
#define SHINYBEAR_WORLDMANAGER_H

#include "scene\SceneNode.h"
#include <vector>

namespace shinybear 
{ 
  class Cluster; class GraphicsProvider;
  class Renderer;
}

namespace shinybear
{

class WorldManager
{
public:
  WorldManager(GraphicsProvider *pProvider, Renderer *pRenderer);

  ~WorldManager();

  void Generate(int sizeX, int sizeZ);
  void Update(double elapsedSeconds);
  

private:
  void DeleteAll();

  GraphicsProvider *m_pProvider;
  Renderer *m_pRenderer;
  std::vector<Cluster*> m_clusters;
};

} // namespace shinybear

#endif