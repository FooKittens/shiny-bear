#ifndef SHINYBEAR_ISERIALIZABLE_H
#define SHINYBEAR_ISERIALIZABLE_H

#include <fstream>

namespace shinybear
{

// Interface for Serializable classes.
class ISerializable
{
public:
  // Used to serialize to a stream
  virtual void Serialize(std::ofstream&);
  virtual void DeSerialize(std::ifstream&);

};

} // namespace shinybear

#endif