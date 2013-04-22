#ifndef SHINYBEAR_WINDOWEVENTS_H
#define SHINYBEAR_WINDOWEVENTS_H

#include "events\IEvent.h"

namespace shinybear
{

class WindowClosedEvent : public IEvent
{
public:
  static const EventType kEventType;

  const EventType& GetType() const { return kEventType; }
};

enum SessionState
{
  SS_UNLOCKED = 0,
  SS_LOCKED = 1,
};

class SessionStateChangedEvent : public IEvent
{
public:
  static const EventType kEventType;
  
  SessionStateChangedEvent(SessionState newState)
    :m_newState(newState) { }

  const EventType &GetType() const { return kEventType; }
  SessionState GetNewState() const;

private:
  SessionState m_newState;
};

enum FocusState
{
  FS_LOSTFOCUS = 0,
  FS_GAINEDFOCUS = 1
};

class FocusChangedEvent : public IEvent
{
public:
  static const EventType kEventType;

  FocusChangedEvent(FocusState newState)
    :m_newState(newState) { }
  
  const EventType &GetType() const { return kEventType; }
  FocusState GetNewState() const { return m_newState; }
private:
  FocusState m_newState;
};

} // namespace shinybear


#endif // SHINYBEAR_WINDOWEVENTS_H