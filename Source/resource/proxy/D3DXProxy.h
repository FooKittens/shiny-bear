#ifndef SHINYBEAR_D3DXPROXY_H
#define SHINYBEAR_D3DXPROXY_H

#include "base\system\GraphicsProvider.h"
#include "resource\types\IGraphicsResource.h"
#include "util\SBUtil.h"

#include <d3dx9.h>

namespace shinybear
{

struct FontProxy : IGraphicsResource
{
  FontProxy(ID3DXFont *pFont, const D3DXFONT_DESC &desc)
    :pFont(pFont), desc(desc) { }

  FontProxy() : pFont(nullptr) { }

  ID3DXFont *pFont;
  D3DXFONT_DESC desc;

  void OnDeviceLost()
  {
    RELEASECOM(pFont);
  }

  void OnDeviceReset(GraphicsProvider *pProvider)
  {
    D3DXCreateFontIndirect(pProvider->GetDevice(), &desc, &pFont);
  }
};

} // namespace shinybear

#endif // SHINYBEAR_D3DXPROXY_H