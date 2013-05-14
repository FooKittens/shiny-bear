#ifndef RANDOMMOVER_H
#define RANDOMMOVER_H

#include "graphics\lighting\Lighting.h"
#include <util\math\Math.h>


// Moves around randomly on the xz plane on a given height..
class RandomMover
{
public:
  RandomMover(shinybear::PointLight *pLight, const shinybear::Vector3 &center, float radius)
  {
    m_pLight = pLight;
    m_radius = radius;
    m_height = center.y;
    m_center = center;
    m_world = shinybear::Mat4x4::CreateTranslation(center.x, center.y, center.z);
    
    SelectNewTarget();
  }
  ~RandomMover() { delete m_pLight; }

  void Update(double elapsedTime) 
  {
    if(m_target.DistanceTo(m_world.GetPosition()) < 5.0f)
    {
      SelectNewTarget();
    }

    shinybear::Vector3 dir = m_target - m_world.GetPosition();
    dir.Normalize();
    dir *= m_speed * elapsedTime;
    m_world *= shinybear::Mat4x4::CreateTranslation(dir.x, dir.y, dir.z);
    m_pLight->m_position = m_world.GetPosition();
  }

private:
  void SelectNewTarget()
  {
    m_target = shinybear::Vector3(rand() % 1000 - 500, 0, rand() % 1000 - 500);
    m_target.Normalize();    
    m_target *= m_radius;
    m_target += m_center;
    m_target.y = m_center.y;
    m_speed = ((rand() % 91) + 10) / 10.0f;
  }

  shinybear::Vector3 m_target;
  shinybear::Vector3 m_center;
  float m_radius;
  float m_height;
  float m_speed;
  float m_currentX;
  float m_currentZ;

  shinybear::PointLight *m_pLight;
  shinybear::Mat4x4 m_world;
};

#endif // RANDOMMOVER_H