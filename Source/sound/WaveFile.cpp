#include "sound\WaveFile.h"
#include "sound\SoundProvider.h"
#include "util\SBUtil.h"

#include <dsound.h>
#include <cassert>
#include <stdio.h>
#include <iostream>
#include <fstream>

namespace shinybear
{

WaveFile::WaveFile()
{
  memset(&m_waveHeader, 0, sizeof(WaveHeader));
}

WaveFile *WaveFile::LoadFromFile(wchar_t *filename, SoundProvider *pSoundProvider)
{
  WaveFile *pResult = DBG_NEW WaveFile();
  pResult->m_pSoundProvider = pSoundProvider;
  WaveHeader &wh = pResult->m_waveHeader;

  // Open file stream
  std::wifstream file(filename, std::ios::binary);
  if(!file.is_open())
  {
    assert(false && "Unable to open the specified audio file");
  }
  
  // Read file
  size_t bufSz = sizeof(WaveHeader);
  wchar_t *tempBuffer = DBG_NEW wchar_t[bufSz];
  file.read(tempBuffer, bufSz);

  unsigned int pos = file.tellg();
  file.seekg(bufSz);

  // Reinterpret into header data
  UnpadWideChar(tempBuffer, bufSz, reinterpret_cast<char*>(&wh), bufSz);
  delete[] tempBuffer;

  // Make sure it's a wave file
  if((wh.chunkId[0] != 'R') || (wh.chunkId[1] != 'I') || 
	   (wh.chunkId[2] != 'F') || (wh.chunkId[3] != 'F'))
	{
		assert(false && "Bad wave file! (not RIFF chunk format)");
  }
  if((wh.format[0] != 'W') || (wh.format[1] != 'A') ||
	   (wh.format[2] != 'V') || (wh.format[3] != 'E'))
	{
		assert(false && "Bad wave file! (not WAVE format)");
	}
  if((wh.subChunkId[0] != 'f') || (wh.subChunkId[1] != 'm') ||
	   (wh.subChunkId[2] != 't') || (wh.subChunkId[3] != ' '))
	{
		assert(false && "Bad wave file! (not fmt sub chunk format)");
	}
  if(wh.audioFormat != WAVE_FORMAT_PCM)
	{
		assert(false && "Bad wave file! (not PCM format)");
	}

  // All is good, so we load the actual data!
  bufSz = pResult->m_waveHeader.dataSize;
  char *pPCMData = DBG_NEW char[bufSz];
  tempBuffer = DBG_NEW wchar_t[bufSz];
  file.read(tempBuffer, bufSz);
  UnpadWideChar(tempBuffer, bufSz,
    reinterpret_cast<char*>(pPCMData), bufSz
  );

  delete[] tempBuffer;

  file.close();

  WAVEFORMATEX waveFormat;
  waveFormat.cbSize = sizeof(WAVEFORMATEX);
  waveFormat.wFormatTag = WAVE_FORMAT_PCM;
  waveFormat.nChannels = wh.numChannels;
  waveFormat.nBlockAlign = wh.blockAlign;
  waveFormat.nSamplesPerSec = wh.sampleRate;
  waveFormat.nAvgBytesPerSec = wh.blockAlign * wh.sampleRate;
  waveFormat.wBitsPerSample = wh.bitsPerSample;

  // A listing of flags for the buffer description:
  // http://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.reference.dsbcaps(v=vs.85).aspx
  DSBUFFERDESC bufDesc;
  bufDesc.dwSize = sizeof(DSBUFFERDESC);
  bufDesc.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME |
    DSBCAPS_CTRLFREQUENCY;
  bufDesc.dwReserved = 0;
  bufDesc.lpwfxFormat = &waveFormat;
  bufDesc.dwBufferBytes = wh.dataSize;
  bufDesc.guid3DAlgorithm = GUID_NULL; // unless DSBCAPS_CTRL3D

  // ... and create the sound buffer with the description above
  IDirectSoundBuffer8* &resultBuffer = pResult->m_pSecondaryBuffer;

  LPDIRECTSOUNDBUFFER pTempBuffer = nullptr;
  HRESULT hr = pSoundProvider->GetDevice()->CreateSoundBuffer(&bufDesc, &pTempBuffer, NULL);
  if(FAILED(hr))
  { 
     assert(false && "Could not create the temporary sound buffer!");
  }
  hr = pTempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&resultBuffer);
  if(FAILED(hr))
  { 
     assert(false && "Could not query the created sound buffer interface!");
  }
  pTempBuffer->Release();

  LPVOID lpvWrite;
  DWORD dwLength;

  resultBuffer->Lock(
    0,
    0,
    &lpvWrite,
    &dwLength,
    NULL,
    NULL,
    DSBLOCK_ENTIREBUFFER
  );
  memcpy(lpvWrite, pPCMData, dwLength);
  delete[] pPCMData;
  resultBuffer->Unlock(
    lpvWrite,
    dwLength,
    NULL,
    NULL
  );
  
  return pResult;
}

} // namespace shinybear