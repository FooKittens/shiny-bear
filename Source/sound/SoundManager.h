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

struct WaveHeader
{
	char chunkId[4];
	unsigned long chunkSize;
	char format[4];
	char subChunkId[4];
	unsigned long subChunkSize;
	unsigned short audioFormat;
	unsigned short numChannels;
	unsigned long sampleRate;
	unsigned long bytesPerSecond;
	unsigned short blockAlign;
	unsigned short bitsPerSample;
	char dataChunkId[4];
	unsigned long dataSize;
};

class SoundManager
{
public:
  static void Initialize(HWND hwnd);
  static void Shutdown();
  static void LoadWaveFile(wchar_t *filename);
  static void PlaySound(ResourceKey resource);

private: 
  static void ReleaseAllBuffers(); // Neccessary?

  // Enumerates all available devices
  static BOOL CALLBACK DSEnumProc(LPGUID lpGUID,
    LPCTSTR lpszDesc, LPCTSTR lpszDrvName, LPVOID lpContext);

  static DSCAPS dscaps; // Device capabilities
  static DWORD dwSpeakerConfig;
  static IDirectSound8* directSound;
	static IDirectSoundBuffer* primaryBuffer;
  static bool isInitialized;
  static std::vector<WaveFile> loadedSounds;
};
} // namespace shinybear

#endif // SHINYBEAR_SOUNDMANAGER_H