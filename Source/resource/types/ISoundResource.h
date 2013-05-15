#ifndef SHINYBEAR_ISOUNDRESOURCE_H
#define SHINYBEAR_ISOUNDRESOURCE_H

#include "resource\IResource.h"
#include "sound\SoundProvider.h"

#include <dsound.h>

namespace shinybear { class SoundProvider; }

namespace shinybear
{

class ISoundResource : public IResource
{
public:
  ~ISoundResource();
  // Called when there's a new sound device available.
  // Passes in the sound manager to help re-create any resources.
  //virtual void OnDeviceReset(SoundManager *pSoundManager) = 0;

  // Called when the sound device has been destroyed.
  //virtual void OnDeviceLost() = 0;

  ResourceType::Enum GetType() const { return ResourceType::RT_SOUND; }

  virtual void Play();
protected:
  ISoundResource();
  SoundProvider *m_pSoundProvider;
  IDirectSoundBuffer8 *m_pSecondaryBuffer;
  IDirectSoundBuffer8 *m_pExtraBuffer;

  static DSBUFFERDESC *CreateBufferDescription(WAVEFORMATEX*, unsigned long bufferSize);
  static IDirectSoundBuffer8 *CreateSecondaryBuffer(char *pPCMData, DSBUFFERDESC*, SoundProvider*);
};

} // namespace shinybear

#endif // SHINYBEAR_ISOUNDRESOURCE