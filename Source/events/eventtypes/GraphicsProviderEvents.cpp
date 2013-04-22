#include "events\eventtypes\GraphicsProviderEvents.h"


namespace shinybear
{

const EventType DeviceLostEvent::kEventType = EventType("evt_devicelost");

const EventType &DeviceLostEvent::GetType() const
{
  return kEventType;
}

const EventType DeviceResetEvent::kEventType = EventType("evt_devicereset");

const EventType &DeviceResetEvent::GetType() const
{
  return kEventType;
}

} // namespace shinybear