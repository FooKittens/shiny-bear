#ifndef GAMETIMER_H
#define GAMETIMER_H

#include <Windows.h>

namespace shinybear
{

// Measures time in seconds.
class GameTimer
{
public:
  GameTimer();
  ~GameTimer();

  void Start();
  void Stop();
  void Pause();
  void Resume();
  void Tick();
  inline double GetElapsedTime() { return m_thisSecond - m_lastSecond; }
  inline double GetTimeSinceStart() { return m_thisSecond - m_startSecond; }

private:
  LARGE_INTEGER m_frequency;
  double m_secPerTick, m_startSecond, m_thisSecond, m_lastSecond;
  bool m_isRunning, m_isPaused, m_isHighPerformance;
};

} // namespace framework

#endif