#ifndef RANDOMMOVER_H
#define RANDOMMOVER_H

#include <scene\SceneNode.h>
#include <util\math\Math.h>


// Moves around randomly on the xz plane on a given height..
class RandomMover : public shinybear::SceneNode
{
public:
  RandomMover(const shinybear::Vector3 &center, float radius)
  {
    m_radius = radius;
    m_height = center.y;
    m_center = center;
    Translate(center.x, center.y, center.z);
    SelectNewTarget();
  }
  ~RandomMover() { }

  void Update(double elapsedTime) 
  {
    if(m_target.DistanceTo(GetTransform().GetPosition()) < 5.0f)
    {
      SelectNewTarget();
    }

    shinybear::Vector3 dir = m_target - GetTransform().GetPosition();
    dir.Normalize();
    dir *= m_speed * elapsedTime;
    Translate(dir.x, dir.y, dir.z);

    UpdateChildren(elapsedTime);
  }

private:
  void SelectNewTarget()
  {
    m_target = shinybear::Vector3(rand() % 1000 - 500, m_height, rand() % 1000 - 500);
    m_target.Normalize();
    m_target *= m_radius;
    m_target += m_center;
    m_speed = ((rand() % 91) + 10) / 10.0f;
  }

  shinybear::Vector3 m_target;
  shinybear::Vector3 m_center;
  float m_radius;
  float m_height;
  float m_speed;
  float m_currentX;
  float m_currentZ;
};

#endif // RANDOMMOVER_H