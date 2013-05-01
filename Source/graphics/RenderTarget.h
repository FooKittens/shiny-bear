#ifndef SHINYBEAR_RENDERTARGET_H
#define SHINYBEAR_RENDERTARGET_H

#include "util\Typedefs.h"
#include <d3d9.h>

struct IDirect3DSurface9;
struct IDirect3DTexture9;

namespace shinybear { class GraphicsProvider; }

namespace shinybear
{

class RenderTarget
{
public:
  RenderTarget(GraphicsProvider *pProvider, D3DFORMAT format);
  ~RenderTarget();
  void Initialize();

  bool IsValid() const;

  void OnDeviceReset();
  void OnDeviceLost();

  IDirect3DTexture9 *GetTexture() const;

  void Activate(UINT slot);
  void Deactivate();

private:
  UINT m_currentSlot;
  bool m_isValid;
  bool m_isActive;
  D3DFORMAT m_format;
  GraphicsProvider *m_pProvider;
  IDirect3DSurface9 *m_pSurface;
  IDirect3DTexture9 *m_pTexture;

  // We hold a copy of this so we can reset.
  IDirect3DSurface9 *m_pBackBuffer;
};

inline bool RenderTarget::IsValid() const
{
  return m_isValid;
}

inline IDirect3DTexture9 *RenderTarget::GetTexture() const
{
  return m_pTexture;
}

} // namespace shinybear

#endif // SHINYBEAR_RENDERTARGET_H
