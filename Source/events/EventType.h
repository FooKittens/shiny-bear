#ifndef SHINYBEAR_EVENTTYPE_H
#define SHINYBEAR_EVENTTYPE_H

#include <string>

namespace shinybear { class EventManager; }

namespace shinybear {

typedef _Uint32t EventID;

class EventType {
public:
  static const EventType kWildCard;
  friend class EventManager;

  EventType(const char *pName) : m_name(""){
    size_t len = strlen(pName);
    for(size_t i = 0; i < len; ++i) {
      m_name += tolower(pName[i]);
    }
    SetHashCode();
  }

  // Operators for making it simpler to compare eventtypes.
  bool operator==(const EventType &rhs) const;
  bool operator!=(const EventType &rhs) const;

  const char *GetName() const;
  EventID GetID() const;

private:
  // Generates a hashcode to identify the EventType with.
  void SetHashCode();

  int m_hashCode;
  std::string m_name;  
};

inline const char *EventType::GetName() const {
  return m_name.c_str();
}

inline EventID EventType::GetID() const {
  return m_hashCode;
}

inline bool EventType::operator==(const EventType &rhs) const {
  return m_hashCode == rhs.m_hashCode;
}

inline bool EventType::operator!=(const EventType &rhs) const {
  return !(*this == rhs);
}

// Generates a hashcode and sets it as the id for the eventtype.
inline void EventType::SetHashCode() {
  static const _Uint32t offset = 2166136261;
  static const _Uint32t prime = 16777619;

  if(m_name.compare(kWildCard.m_name) == 0)
  {
    m_hashCode = 0;
    return;
  }

  m_hashCode = offset;
  for(size_t i = 0; i < m_name.length(); ++i)
  {
    m_hashCode ^= m_name[i];
    m_hashCode *= prime;
  }
}

} // namespace shinybear

#endif // SHINYBEAR_EVENTTYPE_H