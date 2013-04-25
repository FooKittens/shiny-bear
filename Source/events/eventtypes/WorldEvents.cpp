#include "WorldEvents.h"

namespace shinybear
{

const EventType ClusterAddedEvent::kEventType = EventType("evt_clusteradded");
const Cluster &ClusterAddedEvent::GetCluster() const
{
  return *m_pCluster;
}
const EventType &ClusterAddedEvent::GetType() const
{
  return kEventType;
}

} // namespace shinybear