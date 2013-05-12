#ifndef SHINYBEAR_SOUNDMANAGER_H
#define SHINYBEAR_SOUNDMANAGER_H

#include "resource\ResourceManager.h"

// Link to Dsound.lib and Dxguid.lib in external project
#include <dsound.h>
#include <Windows.h>
#include <vector>

// Make sure that behaviour and performance are
// acceptable on both Dsound.vxd and the kernel mixer
// http://msdn.microsoft.com/en-us/library/windows/desktop/ee416769(v=vs.85).aspx

namespace shinybear
{

class SoundProvider
{
public:
  SoundProvider();
  ~SoundProvider();

  void Initialize(HWND hwnd);

  IDirectSound8 *GetDevice() { return m_pDirectSound; }

private: 
  void ReleaseAllBuffers(); // Neccessary?

  // Enumerates all available devices
  static BOOL CALLBACK DSEnumProc(LPGUID lpGUID,
    LPCTSTR lpszDesc, LPCTSTR lpszDrvName, LPVOID lpContext);

  DSCAPS m_dscaps; // Device capabilities
  unsigned long m_dwSpeakerConfig;
  IDirectSound8* m_pDirectSound;
	IDirectSoundBuffer* m_pPrimaryBuffer;
  bool m_isInitialized;
};

} // namespace shinybear

#endif // SHINYBEAR_SOUNDMANAGER_H