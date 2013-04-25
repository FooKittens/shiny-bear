#ifndef SHINYBEAR_WORLDEVENTS_H
#define SHINYBEAR_WORLDEVENTS_H

#include "events\IEvent.h"
#include "logic\world\Cluster.h"

namespace shinybear
{

class ClusterAddedEvent : public IEvent
{
public:
  static const EventType kEventType;

  ClusterAddedEvent(const Cluster * const pCluster)
    :m_pCluster(pCluster) { }

  const Cluster &GetCluster() const;

  const EventType &GetType() const;

private:
  const Cluster * const m_pCluster;
};

} // namespace shinybear

#endif // SHINYBEAR_WORLDEVENTS_H