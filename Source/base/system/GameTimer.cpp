#include "base\system\GameTimer.h"
#include <cassert>

namespace shinybear
{

GameTimer::GameTimer()
{
  m_isHighPerformance = false;
  m_isRunning = false;
  // Fall back to timeGetTime() if there is not performance timer.
  if(QueryPerformanceFrequency(&m_frequency))
  {
    m_secPerTick = static_cast<double>(1.0 / m_frequency.QuadPart);
    m_isHighPerformance = true;
  }
  else
  {
    m_secPerTick = 1.0 / 1000.0;
  }
}

void GameTimer::Start()
{

  assert(!m_isRunning && "Attempted to start a running timer!");

  if(m_isHighPerformance)
  {
    LARGE_INTEGER tick;
    QueryPerformanceCounter(&tick);

    m_startSecond = static_cast<double>(tick.QuadPart * m_secPerTick);
  }
  else
  {
    m_startSecond = static_cast<double>(timeGetTime() * m_secPerTick);
  }
  
  m_thisSecond = m_lastSecond = m_startSecond;
  m_isRunning = true;
}

void GameTimer::Stop()
{

  assert(m_isRunning && "Attempted to stop a non-running timer!");

  m_isRunning = false;
}

void GameTimer::Pause()
{
  m_isPaused = true;
}

void GameTimer::Resume()
{
  m_isPaused = false;
}

void GameTimer::Tick()
{
  
   m_lastSecond = m_thisSecond;

  if(m_isHighPerformance)
  {
    LARGE_INTEGER tick;
    QueryPerformanceCounter(&tick);

    m_thisSecond = static_cast<double>(tick.QuadPart * m_secPerTick);
  }
  else
  {
    m_thisSecond = static_cast<double>(timeGetTime() * m_secPerTick);
  }
}


GameTimer::~GameTimer()
{

}



} // namespace framework