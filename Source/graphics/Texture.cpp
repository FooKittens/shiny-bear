#include "graphics\Texture.h"
#include "base\system\GraphicsProvider.h"
#include "util\SBUtil.h"

#include <d3d9.h>
#include <d3dx9.h>

namespace shinybear
{

Texture::Texture(const char *pFilePath)
{
  m_pFilePath = pFilePath;
  m_creationType = CT_FROMFILE;
  m_pTexture = nullptr;
}

Texture::~Texture()
{
  switch(m_creationType)
  {
  case CT_FROMFILE:
    //delete[] m_pFilePath;
    break;
  }

  RELEASECOM(m_pTexture);
}

Texture *Texture::FromFile(GraphicsProvider *pProvider, const char *pPath)
{
  Texture *pText = DBG_NEW Texture(pPath);

  // Loads the texture.
  pText->OnDeviceReset(pProvider);
  
  return pText;
}

void Texture::OnDeviceLost()
{
  RELEASECOM(m_pTexture);
}

void Texture::OnDeviceReset(GraphicsProvider *pProvider)
{
  switch(m_creationType)
  {
  case CT_FROMFILE:
    HR(D3DXCreateTextureFromFile(pProvider->GetDevice(), m_pFilePath, &m_pTexture));
    break;
  }
}

} // namespace shinybear