#include "logic\world\WorldManager.h"
#include "logic\world\Cluster.h"
#include "events\EventManager.h"
#include "events\eventtypes\WorldEvents.h"
#include "util\SBUtil.h"
#include <cassert>

namespace shinybear 
{
 
 WorldManager::WorldManager()
 {
   EventManager::RegisterEventType(ClusterAddedEvent::kEventType);

   Cluster *pCluster = DBG_NEW Cluster();
   
   Block block;
   block.blockType = 0;
   block.material.diffuse = 0xFFFFFFFF;
   block.material.specular = 0xFFFFFFFF;
   
   pCluster->Fill(block);
   AddCluster(pCluster);
 }
 
 WorldManager::~WorldManager()
 {

 }

 void WorldManager::Update(double elapsedSeconds)
 {

 }

 void WorldManager::AddCluster(Cluster *pCluster)
 {
   assert(pCluster != nullptr && "Attempted to add null cluster!");
   m_clusters.push_back(pCluster);
   
   EventPtr event(new ClusterAddedEvent(pCluster));
   EventManager::PushEvent(event);
 }

} // namespace shinybear