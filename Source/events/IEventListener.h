#ifndef SHINYBEAR_IEVENTLISTENER_H
#define SHINYBEAR_IEVENTLISTENER_H

#include "events\IEvent.h"

namespace shinybear {

// Interface used by eventlisteners
class IEventListener {
public:
  virtual const char *GetName() const = 0;
  virtual bool HandleEvent(const EventPtr &event) = 0;

private:

};

} // namespace shinybear

#endif // SHINYBEAR_IEVENTLISTENER_H