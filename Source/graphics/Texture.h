#ifndef SHINYBEAR_TEXTURE_H
#define SHINYBEAR_TEXTURE_H

#include "resource\types\IGraphicsResource.h"

#include <d3d9.h>

namespace shinybear
{
  class GraphicsProvider;
}

namespace shinybear
{
// Wrapper around IDirect3DTexture9
class Texture : public IGraphicsResource
{
public:
  ~Texture();

  static Texture *FromFile(GraphicsProvider *pProvider, const char *pPath);
  static Texture *Create(GraphicsProvider *pProvider);

  void OnDeviceLost();
  void OnDeviceReset(GraphicsProvider *pProvider);

  IDirect3DTexture9 *GetTexture() const;

private:
  enum CreationType
  {
    CT_NOTCREATED,
    CT_FROMFILE,
    CT_INMEMORY,
  } m_creationType;

  // Constructor for builder methods.
  Texture(const char *pFilePath);
  
  IDirect3DTexture9 *m_pTexture;
  
  // Used for textures that have been loaded from file.
  const char *m_pFilePath;

};

inline IDirect3DTexture9 *Texture::GetTexture() const { return m_pTexture; }

} // namespace shinybear

#endif // SHINYBEAR_TEXTURE_H