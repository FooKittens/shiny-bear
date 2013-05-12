#include "sound\SoundProvider.h"
#include "sound\WaveFile.h"
#include "util\SBUtil.h"
#include <dsound.h>
#include <string>
#include <cassert>
#include <fstream>

namespace shinybear
{

SoundProvider::SoundProvider()
{
  m_isInitialized = false;
  memset(&m_dscaps, 0, sizeof(DSCAPS));
  m_dwSpeakerConfig = 0;
  m_pDirectSound = nullptr;
	m_pPrimaryBuffer = nullptr;
}

SoundProvider::~SoundProvider()
{
  // Release the primary sound buffer pointer.
	if(m_pPrimaryBuffer)
	{
		m_pPrimaryBuffer->Release();
		m_pPrimaryBuffer = nullptr;
	}
  
	// Release the direct sound interface pointer.
	if(m_pDirectSound)
	{
		m_pDirectSound->Release();
		m_pDirectSound = nullptr;
	}
}

void SoundProvider::Initialize(HWND hwnd)
{
  if(!m_isInitialized)
  {
    m_pDirectSound = nullptr;
    m_pPrimaryBuffer = nullptr;

    // Initialize direct sound and the primary sound buffer.
    // Enumerate devices
    while(DirectSoundEnumerate(&DSEnumProc, 0) == TRUE)
    {
    }

    HRESULT result;
    DSBUFFERDESC bufferDesc;
	  WAVEFORMATEX waveFormat;

    // Initialize direct sound interface the primary sound buffer
    result = DirectSoundCreate8(NULL, &m_pDirectSound, NULL);
    if(FAILED(result))
    {
      assert(false && "Failed to initalize Direct Sound!");
    }

    // Set cooperative level
    result = m_pDirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
    if(FAILED(result))
    {
      assert(false && "Failed to set application to prioritized sound accessor!");
    }

    // Get device capabilities
    m_dscaps.dwSize = sizeof(DSCAPS);
    result = m_pDirectSound->GetCaps(&m_dscaps); 
    if(FAILED(result))
    {
      assert(false && "Failed to get the sound device capabilities!");
    }
    // More info on device caps:
    // http://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.reference.dscaps(v=vs.85).aspx

    // Get the speaker configuration
    result = m_pDirectSound->GetSpeakerConfig(&m_dwSpeakerConfig);
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
	  result = m_pDirectSound->CreateSoundBuffer(&bufferDesc, &m_pPrimaryBuffer, NULL);
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
	  result = m_pPrimaryBuffer->SetFormat(&waveFormat);
	  if(FAILED(result))
	  {
		  assert(false && "Unable to set primary buffer wave format!");
	  }

    m_isInitialized = true;
  }
  else
  {
    assert(false && "Direct Sound was already initialized, but another attempt was made!");
  }
}

BOOL CALLBACK SoundProvider::DSEnumProc(LPGUID lpGUID,
  LPCTSTR lpszDesc, LPCTSTR lpszDrvName, LPVOID lpContext)
{
  lpGUID;
  std::string description(lpszDesc);
  std::string driverName(lpszDrvName);

  return TRUE;
}

} // namespace shinybear