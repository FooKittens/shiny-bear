#ifndef SHINYBEAR_ILOGIC_H
#define SHINYBEAR_ILOGIC_H

namespace shinybear
{

// Abstract interface for logic objects.
class ILogic
{
public:
  virtual void Update(double) = 0;
};

} // namespace shinybear 


#endif // SHINYBEAR_ILOGIC_H
