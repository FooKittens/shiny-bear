#include "sound\SoundManager.h"
#include "sound\WaveFile.h"
#include "util\SBUtil.h"
#include <dsound.h>
#include <string>
#include <cassert>
#include <fstream>

namespace shinybear
{

// Forward declarations
DSCAPS SoundManager::dscaps; 
DWORD SoundManager::dwSpeakerConfig;
IDirectSound8* SoundManager::directSound;
IDirectSoundBuffer* SoundManager::primaryBuffer;
bool SoundManager::isInitialized = false;

void SoundManager::Initialize(HWND hwnd)
{
  if(!isInitialized)
  {
    directSound = 0;
    primaryBuffer = 0;

    // Initialize direct sound and the primary sound buffer.
    // Enumerate devices
    while(DirectSoundEnumerate(&DSEnumProc, 0) == TRUE)
    {
    }

    HRESULT result;
    DSBUFFERDESC bufferDesc;
	  WAVEFORMATEX waveFormat;

    // Initialize direct sound interface the primary sound buffer
    result = DirectSoundCreate8(NULL, &directSound, NULL);
    if(FAILED(result))
    {
      assert(false && "Failed to initalize Direct Sound!");
    }

    // Set cooperative level
    result = directSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
    if(FAILED(result))
    {
      assert(false && "Failed to set application to prioritized sound accessor!");
    }

    // Get device capabilities
    dscaps.dwSize = sizeof(DSCAPS);
    result = directSound->GetCaps(&dscaps); 
    if(FAILED(result))
    {
      assert(false && "Failed to get the sound device capabilities!");
    }
    // More info on device caps:
    // http://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.reference.dscaps(v=vs.85).aspx

    // Get the speaker configuration
    result = directSound->GetSpeakerConfig(&dwSpeakerConfig);
    if(FAILED(result))
    {
      assert(false && "Could not get the speaker configuration!");
    }
    // More info on speaker configuration:
    // http://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.idirectsound8.idirectsound8.setspeakerconfig(v=vs.85).aspx

    // Setup the primary buffer description
    bufferDesc.dwSize = sizeof(DSBUFFERDESC); 
    bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
    bufferDesc.dwBufferBytes = 0;
    bufferDesc.dwReserved = 0;
    bufferDesc.lpwfxFormat = NULL;
    bufferDesc.guid3DAlgorithm = GUID_NULL;
    // More info on DSBUFFERDESC:
    // http://msdn.microsoft.com/en-us/library/windows/desktop/ee416240(v=vs.85).aspx

    // Get control of the primary sound buffer on the default sound device.
	  result = directSound->CreateSoundBuffer(&bufferDesc, &primaryBuffer, NULL);
	  if(FAILED(result))
	  {
		  assert(false && "Unable to create primary sound buffer!");
	  }

    // Setup the format of the primary sound bufffer.
	  // In this case it is a .WAV file recorded at 44,100 samples per second in 16-bit stereo (cd audio format).
	  waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	  waveFormat.nSamplesPerSec = 44100;
	  waveFormat.wBitsPerSample = 16;
	  waveFormat.nChannels = 2;
	  waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	  waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	  waveFormat.cbSize = 0;
    
	  // Set the primary buffer to be the wave format specified.
	  result = primaryBuffer->SetFormat(&waveFormat);
	  if(FAILED(result))
	  {
		  assert(false && "Unable to set primary buffer wave format!");
	  }

    isInitialized = true;
  }
  else
  {
    assert(false && "Direct Sound was already initialized, but another attempt was made!");
  }
}

void SoundManager::Shutdown()
{
  // Release the primary sound buffer pointer.
	if(primaryBuffer)
	{
		primaryBuffer->Release();
		primaryBuffer = 0;
	}
 
	// Release the direct sound interface pointer.
	if(directSound)
	{
		directSound->Release();
		directSound = 0;
	}
}

BOOL CALLBACK SoundManager::DSEnumProc(LPGUID lpGUID,
  LPCTSTR lpszDesc, LPCTSTR lpszDrvName, LPVOID lpContext)
{
  lpGUID;
  std::string description(lpszDesc);
  std::string driverName(lpszDrvName);

  return TRUE;
}

} // namespace shinybear