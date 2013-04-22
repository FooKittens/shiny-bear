#ifndef SHINYBEAR_WINDOWEVENTS_H
#define SHINYBEAR_WINDOWEVENTS_H

#include "events\IEvent.h"

namespace shinybear {

class WindowClosedEvent : public IEvent {
public:
  static const EventType kEventType;

  const EventType& GetType() const { return kEventType; }
};

} // namespace shinybear


#endif // SHINYBEAR_WINDOWEVENTS_H