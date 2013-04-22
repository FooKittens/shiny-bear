
#include "events\eventtypes\WindowEvents.h"

namespace shinybear {

const EventType WindowClosedEvent::kEventType = EventType("evt_windowclosed");


const EventType SessionStateChangedEvent::kEventType = EventType("evt_sessionchanged");
SessionState SessionStateChangedEvent::GetNewState() const {
  return m_newState;
}

const EventType FocusChangedEvent::kEventType = EventType("evt_focuschanged");

} // namespace shinybear