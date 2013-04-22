#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <deque>
#include <vector>
#include <map>
#include <list>
#include <memory>
#include "events\EventType.h"
#include "events\IEventListener.h"


namespace shinybear
{



class EventManager
{
public:

  // Creates the singleton instance.
  static void Initialize();

  static void CleanUp();

  // Registers or unregisters an eventType, this lets us
  // verify that our flow is correct and that we're not listening
  // for incorrect types.
  static void RegisterEventType(const EventType &evtType);
  static void UnRegisterEventType(const EventType &evtType);

  // Registers/UnRegisters listeners with the parameter eventtype.
  static void RegisterEventListener(const EventType &evtType,
    IEventListener *pListener);
  static void UnRegisterEventListener(const EventType &evtType,
    IEventListener *pListener);


  // Pushes an event to the queue to be handled later.
  static void PushEvent(EventPtr &evt);

  // Push an immediate event, an event that will be handled immediatly.
  static void PushImmediateEvent(EventPtr &evt);

  static void CancelEvent(const EventType &evtType, bool all = false);

  // Handles the events on the current queue.
  static void ProcessEvents();

private:
  static EventManager *pSingleton;

  EventManager();
  ~EventManager();

  // The class methods will forward to these instance methods to
  // do the actual work.
  void SafeRegisterEventType(const EventType &evtType);
  void SafeUnRegisterEventType(const EventType &evtType);
  
  void SafeRegisterEventListener(const EventType &evtType,
    IEventListener *pListener);
  void SafeUnRegisterEventListener(const EventType &evtType,
    IEventListener *pListener);

  void SafePushEvent(EventPtr evt);
  void SafePushImmediateEvent(EventPtr evt);
  void SafeCancelEvent(const EventType &evtType, bool all);

  void SafeProcessEvents();

  // Helper methods.
  bool IsEventTypeRegistered(const EventType &evtType) const;

  typedef std::list<IEventListener*> ListenerRegistry;
  typedef std::pair<EventID, ListenerRegistry> EventMapping;

  // Instance fields
  std::deque<EventPtr> m_eventQueue, m_activeEvents;
  std::vector<EventType> m_registeredEvents;
  std::map<EventID, ListenerRegistry> m_listenerMap;
};

// Inlines
inline void EventManager::RegisterEventType(const EventType &evtType)
{
  pSingleton->SafeRegisterEventType(evtType);
}

inline void EventManager::UnRegisterEventType(const EventType &evtType)
{
  pSingleton->SafeUnRegisterEventType(evtType);
}

inline void EventManager::RegisterEventListener(const EventType &evtType,
  IEventListener *pListener)
{
  pSingleton->SafeRegisterEventListener(evtType, pListener);
}

inline void EventManager::UnRegisterEventListener(const EventType &evtType,
  IEventListener *pListener)
{
  pSingleton->SafeUnRegisterEventListener(evtType, pListener);
}

inline void EventManager::PushEvent(EventPtr &evt)
{
  pSingleton->SafePushEvent(evt);
}

inline void EventManager::PushImmediateEvent(EventPtr &evt)
{
  pSingleton->SafePushImmediateEvent(evt);
}

inline void EventManager::CancelEvent(const EventType &evtType, bool all)
{
  pSingleton->SafeCancelEvent(evtType, all);
}

inline void EventManager::ProcessEvents()
{
  pSingleton->SafeProcessEvents();
}

} // namespace shinybear

#endif