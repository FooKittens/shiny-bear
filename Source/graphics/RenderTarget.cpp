#include "graphics\RenderTarget.h"
#include "base\system\GraphicsProvider.h"
#include "util\SBUtil.h"

#include <d3d9.h>
#include <cassert>

namespace shinybear
{

RenderTarget::RenderTarget(GraphicsProvider *pProvider, D3DFORMAT format)
{
  m_format = format;
  m_pProvider = pProvider;
  m_pTexture = nullptr;
  m_pSurface = nullptr;
  m_isValid = false;
  m_isActive = false;
  m_currentSlot = 0;
}

RenderTarget::~RenderTarget()
{
  RELEASECOM(m_pSurface);
  RELEASECOM(m_pTexture);
}

void RenderTarget::Initialize()
{
  OnDeviceReset(m_pProvider);
}

void RenderTarget::Activate(UINT slot)
{
  assert(!m_isActive && "RenderTarget is already active!");
  if(slot == 0)
  {
    m_pProvider->GetDevice()->GetRenderTarget(0, &m_pBackBuffer);
  }
  m_pProvider->GetDevice()->SetRenderTarget(slot, m_pSurface);
  m_currentSlot = slot;
  m_isActive = true;
}

void RenderTarget::Deactivate()
{
  assert(m_isActive && "RenderTarget is not active!");
  m_isActive = false;
  if(m_currentSlot == 0)
  {
    m_pProvider->GetDevice()->SetRenderTarget(m_currentSlot, m_pBackBuffer);
    RELEASECOM(m_pBackBuffer);
  }
  else
  {
    HR(m_pProvider->GetDevice()->SetRenderTarget(m_currentSlot, nullptr));
  }
  
}

void RenderTarget::OnDeviceLost()
{
  RELEASECOM(m_pSurface);
  RELEASECOM(m_pTexture);
}

void RenderTarget::OnDeviceReset(GraphicsProvider *pProvider)
{
  DisplayMode mode = m_pProvider->GetDisplayMode();

  IDirect3DDevice9 *pDevice = m_pProvider->GetDevice();

  HR(pDevice->GetRenderTarget(0, &m_pBackBuffer));

  HR(pDevice->CreateTexture(
    mode.width,
    mode.height,
    1,
    D3DUSAGE_RENDERTARGET,
    m_format,
    D3DPOOL_DEFAULT,
    &m_pTexture,
    NULL
  ));

  HR(m_pTexture->GetSurfaceLevel(0, &m_pSurface));
  RELEASECOM(m_pBackBuffer);
}

} // namespace shinybear