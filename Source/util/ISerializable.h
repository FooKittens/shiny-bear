#ifndef SHINYBEAR_ISERIALIZABLE_H
#define SHINYBEAR_ISERIALIZABLE_H

#include <fstream>

namespace shinybear
{

enum SerializationFlags
{
  SF_BINARY = 0,
  SF_ASCII = 1,
};

typedef unsigned char ubyte;

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