#ifndef SHINYBEAR_WORLDMANAGER_H
#define SHINYBEAR_WORLDMANAGER_H

#include "scene\SceneNode.h"
#include <vector>

namespace shinybear { class Cluster; }

namespace shinybear
{

class WorldManager : public SceneNode
{
public:
  WorldManager();

  ~WorldManager();

  void AddCluster(Cluster *pCluster);
  void Update(double elapsedSeconds);

private:
  //std::vector<Cluster*> m_clusters;
};

} // namespace shinybear

#endif