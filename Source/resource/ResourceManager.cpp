#include "resource\ResourceManager.h"
#include "resource\types\IGraphicsResource.h"
#include "base\BaseGame.h"
#include "util\SBUtil.h"

#include <cassert>

namespace shinybear
{

BaseGame *ResourceManager::pGame = nullptr;
ResourceManager::ResourceMap ResourceManager::resources;

void ResourceManager::Initialize(BaseGame *pGame)
{
  ResourceManager::pGame = pGame;
}

bool ResourceManager::ResourceExists(const ResourceMap &map, const ResourceKey &key)
{
  if(map.find(key) == map.cend())
  {
    return false;
  }

  return true;
}

void ResourceManager::RegisterResource(IResource *pResource,
  ResourceKey name)
{
  // Make sure resource does not exist.
  assert(!ResourceExists(resources, name) && "Resource already exists!");

  ResourceMapping mapping(name, pResource);
  resources.insert(mapping);  
  OutputDbgFormat("Resource [%s] was registered.", name.c_str());
}


void ResourceManager::OnGraphicsDeviceLost()
{
  auto it = resources.begin();
  auto end = resources.end();

  while(it != end)
  {
    if((*it).second->GetType() == ResourceType::RT_GRAPHICS)
    {
      dynamic_cast<IGraphicsResource *>((*it).second)->OnDeviceLost();
    }
    ++it;
  }
}

void ResourceManager::OnGraphicsDeviceReset(GraphicsProvider *pProvider)
{
  auto it = resources.begin();
  auto end = resources.end();

  while(it != end)
  {
    if((*it).second->GetType() == ResourceType::RT_GRAPHICS)
    {
      dynamic_cast<IGraphicsResource *>((*it).second)->OnDeviceReset(pProvider);
    }
    ++it;
  }
}

void ResourceManager::DeleteResource(IResource *pResource)
{
  assert(pResource && "Invalid input!");

  auto it = resources.begin();
  auto end = resources.end();

  while(it != end)
  {
    if(it->second == pResource)
    {
      delete it->second;
      resources.erase(it);
      break;
    }
    ++it;
  }
}

void ResourceManager::DeleteResource(ResourceKey name)
{
  DeleteResource(resources.find(name)->second);
}

void ResourceManager::Cleanup()
{
  auto it = resources.begin();
  auto end = resources.end();

  while(it != end)
  {
    delete it->second;
    ++it;
  }

  resources.clear();
}

} // namespace shinybear