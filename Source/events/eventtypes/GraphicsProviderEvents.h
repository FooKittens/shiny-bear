#ifndef SHINYBEAR_GRAPHICSPROVIDEREVENTS_H
#define SHINYBEAR_GRAPHICSPROVIDEREVENTS_H

#include "events\IEvent.h"

namespace shinybear
{

class DeviceLostEvent : public IEvent
{
public:
  static const EventType kEventType;

  const EventType &GetType() const;
};

class DeviceResetEvent : public IEvent
{
public:
  static const EventType kEventType;
  const EventType &GetType() const;
};

} // namespace shinybear

#endif