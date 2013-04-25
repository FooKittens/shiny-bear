#ifndef SHINYBEAR_BASEGAMELOGIC_H
#define SHINYBEAR_BASEGAMELOGIC_H

#include "logic\ILogic.h"

namespace shinybear
{

class BaseGameLogic : public ILogic
{
public:
  BaseGameLogic();
  virtual ~BaseGameLogic();

  void Update(double elapsedSeconds) = 0;

protected:
  
private:

};

} // namespace shinybear


#endif // SHINYBEAR_BASEGAMELOGIC_H