#ifndef SHINYBEAR_IDRAWABLE_H
#define SHINYBEAR_IDRAWABLE_H

namespace shinybear
{

class IDrawable
{
public:
  virtual float GetBoundingRadius() = 0;
  virtual void Render() = 0;
};

} // namespace shinybear

#endif