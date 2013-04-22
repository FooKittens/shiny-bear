#include "events\EventManager.h"
#include <cassert>
#include <algorithm>
#include <Windows.h>
#include "util\SBUtil.h"

namespace shinybear {

EventManager *EventManager::pSingleton = nullptr;

EventManager::EventManager() {

}

EventManager::~EventManager() {
  m_activeEvents.clear();
  m_activeEvents.shrink_to_fit();
  m_eventQueue.clear();
  m_eventQueue.shrink_to_fit();
  m_listenerMap.clear();  
}

void EventManager::CleanUp() {
  delete pSingleton;
}

void EventManager::Initialize() {
  assert(pSingleton == nullptr && "EventManager already initialized!");

  pSingleton = DBG_NEW EventManager();
}

// Registers an eventtype to make it valid for listening and pushing.
void EventManager::SafeRegisterEventType(const EventType &evtType) {
  // We want to notice redundant attempts to register the same type.
  assert(!IsEventTypeRegistered(evtType) && "EventType already registered!");
  
  m_registeredEvents.push_back(evtType);
  OutputDbgFormat("Event[%s] was registered with id[%i]",
    evtType.GetName(), evtType.GetID());
}

// UnRegisters an eventtype so the EventManager won't recognize it anymore.
void EventManager::SafeUnRegisterEventType(const EventType &evtType) {
  // Make sure the eventtype is registered.
  assert(IsEventTypeRegistered(evtType) && "EventType not registered!");

  // Find the eventtype.
  auto it = m_registeredEvents.begin();
  while(it != m_registeredEvents.end()) {
    if(it->m_hashCode == evtType.m_hashCode) {
      // remove it from the vector.
      m_registeredEvents.erase(it);
      break;
    }
    ++it;
  }

  OutputDbgFormat("EventType %s was unregistered!", evtType.GetName());
}

// Adds an EventListener to the list for a certain event.
void EventManager::SafeRegisterEventListener(const EventType &evtType,
  IEventListener *pListener) {

  // Make sure the eventtype is registered.
  assert(IsEventTypeRegistered(evtType) && "EventType not registered!");

  // See if a mapping currently exists for the eventtype
  auto lMap = m_listenerMap.find(evtType.GetID());
  if(lMap != m_listenerMap.end()) {
    lMap->second.push_back(pListener);
  } else {
    // No mapping exists, we have to create one.
    EventMapping evtMap(evtType.GetID(), ListenerRegistry());
    evtMap.second.push_back(pListener);
    m_listenerMap.insert(evtMap);
  }

  OutputDbgFormat("Listener[%s] registered for [%s]",
    pListener->GetName(), evtType.GetName());
}

// Removes an EventListener from the list of the eventType
void EventManager::SafeUnRegisterEventListener(const EventType &evtType,
  IEventListener *pListener) {
  
  // Make sure the eventtype is registered.
  assert(IsEventTypeRegistered(evtType) && "EventType not registered!");

  auto lMap = m_listenerMap.find(evtType.GetID());
  assert(lMap != m_listenerMap.end() && "No mapping exists for eventtype!");

  auto it = lMap->second.begin();
  while(it != lMap->second.end()) {
    if(*it == pListener) {
      // Remove the listener from the list.
      lMap->second.erase(it);

      // Check if this was the last listener.
      // In that case we can remove the mapping,
      // which should make searches faster.
      if(lMap->second.size() == 0) {
        m_listenerMap.erase(lMap);
      }
      break;
    }
    ++it;
  }

  OutputDbgFormat("Listener[%s] was removed from [%s]", pListener->GetName(),
    evtType.GetName());
}

void EventManager::SafePushEvent(EventPtr evt) {
  // Make sure the eventtype is registered.
  assert(IsEventTypeRegistered(evt->GetType()) && "EventType not registered!");

  m_eventQueue.push_back(evt);
  OutputDbgFormat("Event[%s] was pushed.", evt->GetType().GetName());
}

void EventManager::SafePushImmediateEvent(EventPtr evt) {
  // Make sure the eventtype is registered.
  assert(IsEventTypeRegistered(evt->GetType()) && "EventType not registered!");
  
  auto lMap = m_listenerMap.find(evt->GetType().GetID());
  
  // If there are is not mapping for the event,
  // it makes no sense to process it further.
  if(lMap != m_listenerMap.end()) {

    // Iterate over the listener registry for this eventtype.
    auto it = lMap->second.begin();
    while(it != lMap->second.end()) {

      // If a listener swallows an event, no others will be notified.
      if((*it)->HandleEvent(evt)) {
        OutputDbgFormat("Event[%s] was sent to listener [%s]", 
          evt->GetType().GetName(), (*it)->GetName());
        break;
      }

      ++it;
    }
  }

  // Pass all events to wildcard listeners.
  auto wildMap = m_listenerMap.find(EventType::kWildCard.GetID());
  // If there are is not mapping for the event,
  // it makes no sense to process it further.
  if(wildMap != m_listenerMap.end()) {

    // Iterate over the listener registry for wildcards.
    auto it = wildMap->second.begin();
    while(it != wildMap->second.end()) {

      // If a listener swallows an event, no others will be notified.
      if((*it)->HandleEvent(evt)) {
        break;
      }
    }
  }
}

void EventManager::SafeCancelEvent(const EventType &evtType, bool all) {
  // Make sure the eventtype is registered.
  assert(IsEventTypeRegistered(evtType) && "EventType not registered!");
  
  // Remove all events of the matching type if specified.
  if(all) {
    std::remove_if(m_eventQueue.begin(), m_eventQueue.end(),
      [&evtType](EventPtr ptr) {
      return ptr->GetType() == evtType;
    });

    OutputDbgFormat("All instances of [%s] were removed from the event queue.",
      evtType.GetName());

    return;
  } else {
    // Remove the last event pushed to the queue of the correct type.
    auto it = m_eventQueue.rbegin();
    while(it != m_eventQueue.rend()) {
      if((*it)->GetType() == evtType) {
        m_eventQueue.erase(it.base());
        break;
      }
      ++it;
    }
  }
}

void EventManager::SafeProcessEvents() {

  // Swap the event lists, since new events will likely
  // trigger from our processing, we dont want an infinite loop.
  m_activeEvents.swap(m_eventQueue);

  while(m_activeEvents.size() > 0) {
    // Get the element that has been on the queue for the longest time.
    EventPtr ptr = m_activeEvents.front();

    // Handle it immediatly.
    PushImmediateEvent(ptr);

    // Remove it from the queue.
    m_activeEvents.pop_front();
  }
}

bool EventManager::IsEventTypeRegistered(const EventType &evtType) const {
  
  // If there are no events, no event could be registered..
  if(m_registeredEvents.size() == 0) {
    return false;
  }

  auto it = m_registeredEvents.begin();
  while(it != m_registeredEvents.end()) {
    if(*it++ == evtType) {
      return true;
    }
  }

  return false;
}



} // namespace shinybear