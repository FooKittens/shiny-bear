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
IDirectSoundBuffer8* SoundManager::secondaryBuffer1;
bool SoundManager::isInitialized = false;

void SoundManager::Initialize(HWND hwnd)
{
  if(!isInitialized)
  {
    directSound = 0;
    primaryBuffer = 0;
    secondaryBuffer1 = 0;

    // Initialize direct sound and the primary sound buffer.
	  InitializeDirectSound(hwnd);
 
	  // Load a wave audio file onto a secondary buffer.
    wchar_t *pBuffer;
    GetAbsolutePath(L"res\\shaders\\LightShader.fx", &pBuffer);
	  LoadWaveFile(pBuffer, &secondaryBuffer1);
 
	  // Play the wave file now that it has been loaded.
	  //PlayWaveFile();

    isInitialized = true;
  }
}

void SoundManager::Shutdown()
{
  // Automatically releases all secondary buffers as well
  ShutdownDirectSound();
}

void SoundManager::InitializeDirectSound(HWND hwnd)
{
  // Not used, because main device will suffice for now
  // Enumerate devices
  //while(DirectSoundEnumerate(&DSEnumProc, 0) == TRUE)
  //{
  //}

  HRESULT result;
  DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;

  // Initialize direct sound interface the primary sound buffer
  result = DirectSoundCreate8(NULL, &directSound, NULL);
  if(FAILED(result))
  {
    assert(false && "Failed to create IDirectSound8 object!");
  }

  // Set cooperative level
  result = directSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
  if(FAILED(result))
  {
    assert(false && "Failed to set application prioritized sound cooperative level!");
  }

  //// Get device capabilities
  //dscaps.dwSize = sizeof(DSCAPS);
  //result = directSound->GetCaps(&dscaps); 
  //if(FAILED(result))
  //{
  //  assert(false && "Failed to get sound device capabilities!");
  //}
  //// More info on device caps:
  //// http://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.reference.dscaps(v=vs.85).aspx

  //// Get the speaker configuration
  //result = directSound->GetSpeakerConfig(&dwSpeakerConfig);
  //if(result == DSERR_INVALIDPARAM)
  //{
  //  assert(false && "Invalid parameter sent to get speaker configuration!");
  //}
  //else if(result == DSERR_UNINITIALIZED)
  //{
  //  assert(false && "Uninitialized sent to get speaker configuration!");
  //}
  //// More info on speaker configuration:
  //// http://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.idirectsound8.idirectsound8.setspeakerconfig(v=vs.85).aspx

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
}

void SoundManager::ShutdownDirectSound()
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

void SoundManager::LoadWaveFile(wchar_t *filename, IDirectSoundBuffer8 **secondaryBuffer)
{
	WaveHeader waveFileHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	HRESULT result;
	IDirectSoundBuffer* tempBuffer;
	unsigned char* waveData;
	unsigned char *bufferPtr;
	unsigned long bufferSize;
  std::wifstream file;
  //FILE *filePtr;
  int error;

  // Open the wave file in binary.
  file.open(filename, std::ios::in | std::ios::binary | std::ios::ate);
  if(!file.is_open())
  {
    assert(false && "Could not open the specified wave file!");
  }
  std::wifstream::pos_type size;
  wchar_t *headerData;
  file.seekg(0, std::ios::beg);
  file.readsome(headerData, sizeof(waveFileHeader));
  
	// Read in the wave file header.
	count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if(count != 1)
	{
		assert(false && "Could not read the specified wave file header!");
	}
  
	// Check that the chunk ID is the RIFF format.
	if((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') || 
	   (waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
	{
		assert(false && "Wave file chunk ID is not file format RIFF!");
	}
  
	// Check that the file format is the WAVE format.
	if((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
	   (waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
	{
		assert(false && "Wave file format is not WAVE!");
	}
  
	// Check that the sub chunk ID is the fmt format.
	if((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
	   (waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
	{
		assert(false && "Wave file sub chunk ID is not fmt format!");
	}
  
	// Check that the audio format is WAVE_FORMAT_PCM.
	if(waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
	{
		assert(false && "Wave file audio format is not WAVE_FORMAT_PCM!");
	}
  
	// Check that the wave file was recorded in stereo format.
	if(waveFileHeader.numChannels != 2)
	{
		assert(false && "Wave file is not stereo!");
	}
  
	// Check that the wave file was recorded at a sample rate of 44.1 KHz.
	if(waveFileHeader.sampleRate != 44100)
	{
		assert(false && "Wave file sample rate is not 44100 Hz!");
	}
  
	// Ensure that the wave file was recorded in 16 bit format.
	if(waveFileHeader.bitsPerSample != 16)
	{
		assert(false && "Wave file is not in 16 bit format!");
	}
  
	// Check for the data chunk header.
	if((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
	   (waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
	{
		assert(false && "Invalid wave file data header!");
	}
  
  // Set the wave format of secondary buffer that this wave file will be loaded onto.
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;
  
	// Set the buffer description of the secondary sound buffer that the wave file will be loaded onto.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

  // Create a temporary sound buffer with the specific buffer settings.
	result = directSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
	if(FAILED(result))
	{
		assert(false && "Unable to create a temporary sound buffer!");
	}
  
	// Test the buffer format against the direct sound 8 interface and create the secondary buffer.
	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer);
	if(FAILED(result))
	{
		assert(false && "Unable to create a secondary sound buffer!");
	}
  
	// Release the temporary buffer.
	tempBuffer->Release();
	tempBuffer = 0;

  // Move to the beginning of the wave data which starts at the end of the data chunk header.
	fseek(filePtr, sizeof(WaveHeader), SEEK_SET);
  
	// Create a temporary buffer to hold the wave file data.
	waveData = new unsigned char[waveFileHeader.dataSize];
	if(!waveData)
	{
		assert(false && "Unable to create a temporary wave file data buffer!");
	}
  
	// Read in the wave file data into the newly created buffer.
	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if(count != waveFileHeader.dataSize)
	{
		assert(false && "Unable to read wave file data into the buffer!");
	}
  
	// Close the file once done reading.
	error = fclose(filePtr);
	if(error != 0)
	{
		assert(false && "Unable to close the wave file!");
	}
  
	// Lock the secondary buffer to write wave data into it.
	result = (*secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	if(FAILED(result))
	{
		assert(false && "Unable to lock the secondary buffer!");
	}
  
	// Copy the wave data into the buffer.
	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);
  
	// Unlock the secondary buffer after the data has been written to it.
	result = (*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	if(FAILED(result))
	{
		assert(false && "Unable to unlock the secondary buffer!");
	}
	
	// Release the wave data since it was copied into the secondary buffer.
	delete [] waveData;
	waveData = 0;
}

void SoundManager::PlayWaveFile()
{
	HRESULT result;
 
	// Set position at the beginning of the sound buffer.
	result = secondaryBuffer1->SetCurrentPosition(0);
	if(FAILED(result))
	{
		assert(false && "Unable to set the position in the secondary sound buffer!");
	}
 
	// Set volume of the buffer to 100%.
	result = secondaryBuffer1->SetVolume(DSBVOLUME_MAX);
	if(FAILED(result))
	{
		assert(false && "Unable to set the volume of the secondary sound buffer!");
	}
 
	// Play the contents of the secondary sound buffer.
	result = secondaryBuffer1->Play(0, 0, 0);
	if(FAILED(result))
	{
		assert(false && "Unable to play the contents of the secondary sound buffer!");
	}
}

void SoundManager::ShutdownWaveFile(IDirectSoundBuffer8 **secondaryBuffer)
{
  // Release the secondary sound buffer.
	if(*secondaryBuffer)
	{
		(*secondaryBuffer)->Release();
		*secondaryBuffer = 0;
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

//HRESULT CreateBasicBuffer(LPDIRECTSOUND8 lpDirectSound, LPDIRECTSOUNDBUFFER8* ppDsb8) 
//{ 
//  WAVEFORMATEX wfx; 
//  DSBUFFERDESC dsbdesc; 
//  LPDIRECTSOUNDBUFFER pDsb = NULL;
//  HRESULT hr; 
//  
//  // Set up WAV format structure. 
//  memset(&wfx, 0, sizeof(WAVEFORMATEX)); 
//  wfx.wFormatTag = WAVE_FORMAT_PCM; 
//  wfx.nChannels = 2; 
//  wfx.nSamplesPerSec = 22050; 
//  wfx.nBlockAlign = 4; 
//  wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign; 
//  wfx.wBitsPerSample = 16;
// 
//  // Set up DSBUFFERDESC structure. 
//  memset(&dsbdesc, 0, sizeof(DSBUFFERDESC)); 
//  
// 
//  // Create buffer. 
//  hr = lpDirectSound->CreateSoundBuffer(&dsbdesc, &pDsb, NULL); 
//  if(SUCCEEDED(hr)) 
//  { 
//     hr = pDsb->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*) ppDsb8);
//     pDsb->Release();
//  }
//  return hr;
//} 

} // namespace shinybear