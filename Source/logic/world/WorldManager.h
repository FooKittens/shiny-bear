#ifndef SHINYBEAR_WORLDMANAGER_H
#define SHINYBEAR_WORLDMANAGER_H

#include <vector>

namespace shinybear
{
  class IActivator; class Cluster;
}

namespace shinybear
{

// singleton class that handles the world.
// WorldManager is used to create, manage and remove clusters.
class WorldManager
{
public:
  typedef unsigned int ClusterSize;
  static const ClusterSize kClusterSizeX = 32;
  static const ClusterSize kClusterSizeY = 32;
  static const ClusterSize kClusterSizeZ = 32;

  WorldManager();
  virtual ~WorldManager();

  virtual void Update(double elapsedSeconds);

  virtual void AddActivator(IActivator *pActivator);
  virtual void RemoveActivator(IActivator *pActivator);
    
  virtual Cluster *GetCluster(int idxX, int idxY, int idxZ);

protected:
  virtual void RemoveCluster(Cluster *pCluster);
  virtual void AddCluster(Cluster *pCluster);

private:
  std::vector<IActivator*> m_activators;
  std::vector<Cluster*> m_clusters;
};


} // namespace shinybear

#endif