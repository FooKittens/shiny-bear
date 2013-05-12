#ifndef SHINYBEAR_ISOUNDRESOURCE_H
#define SHINYBEAR_ISOUNDRESOURCE_H

#include "resource\IResource.h"

namespace shinybear { class SoundManager; }

namespace shinybear
{

class ISoundResource : public IResource
{
public:
  virtual ~ISoundResource() = 0 { }

  // Called when there's a new sound device available.
  // Passes in the sound manager to help re-create any resources.
  virtual void OnDeviceReset(SoundManager *pSoundManager) = 0;

  // Called when the sound device has been destroyed.
  virtual void OnDeviceLost() = 0;

  ResourceType::Enum GetType() const { return ResourceType::RT_SOUND; }
};

class SoundResource : public ISoundResource
{
public:
  SoundResource(SoundManager *pSoundManager)
  {
    m_pSoundManager = pSoundManager;
  }

  void OnDeviceReset(SoundManager *pSoundManager) { }
  void OnDeviceLost() { }
protected:
  SoundManager * const GetSoundManager() const { return m_pSoundManager; }

private:
  SoundManager *m_pSoundManager;
};

} // namespace shinybear

#endif // SHINYBEAR_ISOUNDRESOURCE