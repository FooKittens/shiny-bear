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

} // namespace shinybear