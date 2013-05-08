#ifndef SHINYBEAR_SOUNDMANAGER_H
#define SHINYBEAR_SOUNDMANAGER_H

// Link to Dsound.lib and Dxguid.lib in external project
#include <dsound.h>
#include <Windows.h>

// Make sure that behaviour and performance are
// acceptable on both Dsound.vxd and the kernel mixer
// http://msdn.microsoft.com/en-us/library/windows/desktop/ee416769(v=vs.85).aspx

namespace shinybear
{

class SoundManager
{
public:
  static void Initialize(HWND hwnd);
  static void Shutdown();

private:
  static void InitializeDirectSound(HWND);
	static void ShutdownDirectSound();
 
	static void LoadWaveFile(wchar_t*, IDirectSoundBuffer8**);
  static void PlayWaveFile();
	static void ShutdownWaveFile(IDirectSoundBuffer8**);

    // Enumerates all available devices
  static BOOL CALLBACK DSEnumProc(LPGUID lpGUID,
    LPCTSTR lpszDesc, LPCTSTR lpszDrvName, LPVOID lpContext);

  static DSCAPS dscaps;
  static DWORD dwSpeakerConfig;
  static IDirectSound8* directSound;
	static IDirectSoundBuffer* primaryBuffer;
	static IDirectSoundBuffer8* secondaryBuffer1; // Will need more
  static bool isInitialized;
};
} // namespace shinybear

#endif // SHINYBEAR_SOUNDMANAGER_H