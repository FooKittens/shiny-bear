#ifndef SHINYBEAR_IGRAPHICSRESOURCE_H
#define SHINYBEAR_IGRAPHICSRESOURCE_H

#include "resource\IResource.h"

namespace shinybear { class GraphicsProvider; }

namespace shinybear
{
class IGraphicsResource : public IResource
{
public:
  virtual ~IGraphicsResource() = 0 { }

  // Called when theres a new graphicsdevice availble.
  // Passes in the graphics provider to help re-create any resources.
  virtual void OnDeviceReset(GraphicsProvider *pProvider) = 0;

  // Called when the graphics device has been destroyed.
  virtual void OnDeviceLost() = 0;

  ResourceType::Enum GetType() const { return ResourceType::RT_GRAPHICS; }
};


class GraphicsResource : public IGraphicsResource
{
public:
  GraphicsResource(GraphicsProvider *pProvider)
  {
    m_pProvider = pProvider;
  }

  void OnDeviceReset(GraphicsProvider *pProvider) { }
  void OnDeviceLost() { }
protected:
  GraphicsProvider * const GetProvider() const { return m_pProvider; }

private:
  GraphicsProvider *m_pProvider;
};

} // namespace shinybear

#endif // SHINYBEAR_IGRAPHICSRESOURCE_H