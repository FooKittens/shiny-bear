#ifndef SHINYBEAR_IEVENT_H
#define SHINYBEAR_IEVENT_H

#include "events\EventType.h"
#include <string>
#include <memory>

namespace shinybear {

// Interface implemented by anything that should act as an event.
class IEvent {
public:
  virtual const EventType& GetType() const = 0;
};

typedef std::shared_ptr<IEvent> EventPtr;

} // namespace shinybear


#endif // SHINYBEAR_IEVENT_H