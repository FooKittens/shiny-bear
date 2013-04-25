#ifndef SHINYBEAR_IACTIVATOR_H
#define SHINYBEAR_IACTIVATOR_H

namespace shinybear
{

// This interface is used by the worldmanager to keep clusters alive.
class IActivator
{
public:
  // Cubic and centered range in all dimensions.
  virtual int GetRange() = 0;
protected:

private:

};

} // namespace shinybear

#endif