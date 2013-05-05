#ifndef SHINYBEAR_SOUNDMANAGER_H
#define SHINYBEAR_SOUNDMANAGER_H

// Link to Dsound.lib and Dxguid.lib in external project
#include <dsound.h>
#include <Windows.h>

// Make sure that behaviour and performance are
// acceptable on both Dsound.vxd and the kernel mixer
// http://msdn.microsoft.com/en-us/library/windows/desktop/ee416769(v=vs.85).aspx

// For playback:

// 1. Call DirectSoundCreate8 to create an object that supports the IDirectSound8 interface
// It manages the device and creates sound buffers
// Main interface: IDirectSound8

// Primary buffer
// Mixes and plays sounds from secondary buffers and
// controls global 3D parameters. Main interfaces:
// IDirectSoundBuffer, IDirectSound3DListener8

// Effect
// Transforms the sound in a secondary buffer
// Interaces: One for each particular effect
// For instance, IDirectSoundFXChorus8

// 2. Create a secondary buffer
// It manages a static or streaming sound and plays it into
// the primary buffer
// Main interfaces: IDirectSoundBuffer8,
// IDirectSound3DBuffer8, IDirectSoundNotify8

// 3. Obtain PCM data

// 4. Put data in the buffer

// 5. Play the buffer

// IDirectSound3DListener8

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

  static HRESULT CreateBasicBuffer(LPDIRECTSOUND8 lpDirectSound,
    LPDIRECTSOUNDBUFFER8* ppDsb8);

  // Enumerates all available devices
  static BOOL CALLBACK DSEnumProc(LPGUID lpGUID,
    LPCTSTR lpszDesc, LPCTSTR lpszDrvName, LPVOID lpContext);

private:
  static void InitializeDirectSound(HWND);
	static void ShutdownDirectSound();
 
	static void LoadWaveFile(char*, IDirectSoundBuffer8**);
	static void ShutdownWaveFile(IDirectSoundBuffer8**);
 
	static void PlayWaveFile();

  static DSCAPS dscaps;
  static DWORD dwSpeakerConfig;
  static IDirectSound8* directSound;
	static IDirectSoundBuffer* primaryBuffer;
	static IDirectSoundBuffer8* secondaryBuffer1; // Will need more

};
} // namespace shinybear

#endif // SHINYBEAR_SOUNDMANAGER_H