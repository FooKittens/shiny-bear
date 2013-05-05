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

  struct WaveHeaderType
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

  // Enumerates all available devices
  static BOOL CALLBACK DSEnumProc(LPGUID lpGUID,
    LPCTSTR lpszDesc, LPCTSTR lpszDrvName, LPVOID lpContext);

private:
  static void InitializeDirectSound(HWND);
	static void ShutdownDirectSound();
 
	static void LoadWaveFile(char*, IDirectSoundBuffer8**);
  static void PlayWaveFile();
	static void ShutdownWaveFile(IDirectSoundBuffer8**);

  static DSCAPS dscaps;
  static DWORD dwSpeakerConfig;
  static IDirectSound8* directSound;
	static IDirectSoundBuffer* primaryBuffer;
	static IDirectSoundBuffer8* secondaryBuffer1; // Will need more

};
} // namespace shinybear

#endif // SHINYBEAR_SOUNDMANAGER_H