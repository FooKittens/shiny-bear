#ifndef SHINYBEAR_IRESOURCE_H
#define SHINYBEAR_IRESOURCE_H

#include <memory>

namespace shinybear
{

namespace ResourceType
{
enum Enum
{
  RT_UNDEFINED = 0,
  RT_GRAPHICS = 1,
  RT_SOUND,
  RT_MISC
};

}

class IResource
{
public:
  virtual ~IResource() { }
  virtual ResourceType::Enum GetType() const = 0;
};


} // namespace shinybear

#endif