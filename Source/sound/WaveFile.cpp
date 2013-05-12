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
  m_pPCMData = nullptr;
  m_pSecondaryBuffer = nullptr;
}

WaveFile::~WaveFile()
{
  delete[] m_pPCMData;
}

WaveFile *WaveFile::LoadFromFile(wchar_t *filename, SoundProvider *pSoundProvider)
{
  WaveFile *pResult = DBG_NEW WaveFile();
  WaveHeader &wh = pResult->m_waveHeader;

  std::wifstream file(filename, std::ios::binary);
  if(!file.is_open())
  {
    assert(false && "Unable to open the specified audio file");
  }

  wchar_t *tempBuffer = new wchar_t[sizeof(WaveHeader)];
  file.read(tempBuffer, sizeof(WaveHeader));

  char *realBuffer = reinterpret_cast<char*>(&wh);

  int i = 0;
  for(auto it = realBuffer; it != realBuffer + sizeof(WaveHeader); ++it)
  {
    *it = static_cast<char>(tempBuffer[i++]);
  }

  //TODO header data should be satisfactory now, make sanity checks!
  if((wh.chunkId[0] != 'R') || (wh.chunkId[1] != 'I') || 
	   (wh.chunkId[2] != 'F') || (wh.chunkId[3] != 'F'))
	{
		assert(false && "Bad wave file!");
	}

  // all is good, so we load the actual data!
  pResult->m_pPCMData = DBG_NEW char[pResult->m_waveHeader.dataSize];
  file.read((wchar_t*)pResult->m_pPCMData, pResult->m_waveHeader.dataSize);

  // TODO convert to char array. =(

  if(file.eof())
  {
    file.close();
  }
  else
  {
    assert(false && "bad wave file!");
  }

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
  memcpy(lpvWrite, pResult->m_pPCMData, dwLength);
  resultBuffer->Unlock(
    lpvWrite,
    dwLength,
    NULL,
    NULL
  );
  
  return pResult;
}

void WaveFile::Play()
{
  // Info on the third parameter, the play flags
  // http://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.idirectsoundbuffer8.idirectsoundbuffer8.play(v=vs.85).aspx
  m_pSecondaryBuffer->Play(0, 0, 0);
}

} // namespace shinybear