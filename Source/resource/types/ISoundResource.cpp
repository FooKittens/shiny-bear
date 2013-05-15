#include "resource\types\ISoundResource.h"

#include <cassert>

namespace shinybear
{

ISoundResource::ISoundResource()
{
  m_pSoundProvider = nullptr;
  m_pSecondaryBuffer = nullptr;
  m_pExtraBuffer = nullptr;
}

ISoundResource::~ISoundResource()
{
  delete m_pSoundProvider;
  delete m_pSecondaryBuffer;
  delete m_pExtraBuffer;
}

void ISoundResource::Play()
{
  // Info on the third parameter, the play flags
  // http://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.idirectsoundbuffer8.idirectsoundbuffer8.play(v=vs.85).aspx
  unsigned long pStatus;
  m_pSecondaryBuffer->GetStatus(&pStatus);
  if(pStatus == DSBSTATUS_PLAYING)
  {
    LPDIRECTSOUNDBUFFER pTempBuffer = nullptr;
    HRESULT hr = m_pSoundProvider->GetDevice()->DuplicateSoundBuffer(m_pSecondaryBuffer, &pTempBuffer);
    if(FAILED(hr)) 
    { 
        assert(false && "Could not duplicate the sound buffer!");
    }
    hr = pTempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&m_pExtraBuffer);
    if(FAILED(hr)) 
    { 
        assert(false && "Could not query the duplicated sound buffer interface!");
    }
    pTempBuffer->Release();
    m_pExtraBuffer->Play(0, 0, 0);
  }

  m_pSecondaryBuffer->Play(0, 0, 0);
}

DSBUFFERDESC *ISoundResource::CreateBufferDescription(WAVEFORMATEX *waveFormat, unsigned long bufferSize)
{
   // A listing of flags for the buffer description:
  // http://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.reference.dsbcaps(v=vs.85).aspx
  DSBUFFERDESC bufDesc;
  bufDesc.dwSize = sizeof(DSBUFFERDESC);
  bufDesc.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME |
    DSBCAPS_CTRLFREQUENCY;
  bufDesc.dwReserved = 0;
  bufDesc.lpwfxFormat = waveFormat;
  bufDesc.dwBufferBytes = bufferSize;
  bufDesc.guid3DAlgorithm = GUID_NULL; // unless DSBCAPS_CTRL3D

  return &bufDesc;
}

IDirectSoundBuffer8 *ISoundResource::CreateSecondaryBuffer(char *pPCMData, DSBUFFERDESC *bufferDescription, SoundProvider *pSoundProvider)
{
  IDirectSoundBuffer8* resultBuffer = nullptr;
  LPDIRECTSOUNDBUFFER pTempBuffer = nullptr;
  HRESULT hr = pSoundProvider->GetDevice()->CreateSoundBuffer(bufferDescription, &pTempBuffer, NULL);
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

  return resultBuffer;
}

} // namespace shinybear