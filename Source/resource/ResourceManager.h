#ifndef SHINYBEAR_RESOURCEMANAGER_H
#define SHINYBEAR_RESOURCEMANAGER_H

#include "resource\IResource.h"
#include <map>
#include <string>
#include <cassert>

namespace shinybear 
{ 
  class BaseGame; class IGraphicsResource;
  class GraphicsProvider;
}



namespace shinybear
{
typedef std::string ResourceKey;

class ResourceManager
{
public:
  friend class BaseGame;

  static void Initialize(BaseGame *pGame);

  // Registers a resource with a key.
  static void RegisterResource(IResource  *pResource,
    ResourceKey name);

  template<class RType>
  static RType * const GetResource(const ResourceKey &key);

  static void DeleteResource(IResource *pResource);
  static void DeleteResource(ResourceKey name);

  // Deletes all resources.
  static void Cleanup();

private:
  typedef std::map<ResourceKey, IResource *> ResourceMap;
  typedef std::pair<ResourceKey, IResource *> ResourceMapping;

  static void OnGraphicsDeviceLost();
  static void OnGraphicsDeviceReset(GraphicsProvider *pProvider);

  // used to check if a particular key is already in use.
  static bool ResourceExists(const ResourceMap &map, const ResourceKey &key);

  static BaseGame *pGame;
  static ResourceMap resources;
};

template<class RType>
RType * const ResourceManager::GetResource(const ResourceKey &key)
{
  assert(ResourceExists(resources, key) && "Resource does not exist!");

  return dynamic_cast<RType*>(resources.find(key)->second);
}

} // namespace shinybear

#endif // SHINYBEAR_RESOURCEMANAGER_H