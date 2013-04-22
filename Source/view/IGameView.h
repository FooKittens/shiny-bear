#ifndef SHINYBEAR_IGAMEVIEW_H
#define SHINYBEAR_IGAMEVIEW_H

namespace shinybear
{

class IGameView 
{
public:
  virtual void Update() = 0;
  virtual const char *GetName() const = 0;
};

} // namespace shinybear

#endif // SHINYBEAR_IGAMEVIEW_H