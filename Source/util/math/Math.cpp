#include "util\math\Math.h"

namespace shinybear
{
  const Vector2 Vector2::kZero(0, 0);
  const Vector2 Vector2::kUnitX(1, 0);
  const Vector2 Vector2::kUnitY(0, 1);
  const Vector2 Vector2::kOne(1, 1);

  const Vector3 Vector3::kZero(0, 0, 0);
  const Vector3 Vector3::kUnitX(1, 0, 0);
  const Vector3 Vector3::kUnitY(0, 1, 0);
  const Vector3 Vector3::kUnitZ(0, 0, 1);

  const Vector4 Vector4::kZero(0, 0, 0, 0);
  const Vector4 Vector4::kUnitX(1, 0, 0, 0);
  const Vector4 Vector4::kUnitY(0, 1, 0, 0);
  const Vector4 Vector4::kUnitZ(0, 0, 1, 0);
  const Vector4 Vector4::kUnitW(0, 0, 0, 1);

  const Quaternion Quaternion::kIdentity(D3DXQUATERNION(0, 0, 0, 1));

  const Mat4x4 Mat4x4::kIdentity(1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 0, 0, 0, 1);


Mat4x4::Mat4x4(float m_0_0, float m_0_1, float m_0_2, float m_0_3,
               float m_1_0, float m_1_1, float m_1_2, float m_1_3,
               float m_2_0, float m_2_1, float m_2_2, float m_2_3,
               float m_3_0, float m_3_1, float m_3_2, float m_3_3)
{
  m[0][0] = m_0_0;
  m[0][1] = m_0_1;
  m[0][2] = m_0_2;
  m[0][3] = m_0_3;

  m[1][0] = m_1_0;
  m[1][1] = m_1_1;
  m[1][2] = m_1_2;
  m[1][3] = m_1_3;

  m[2][0] = m_2_0;
  m[2][1] = m_2_1;
  m[2][2] = m_2_2;
  m[2][3] = m_2_3;

  m[3][0] = m_3_0;
  m[3][1] = m_3_1;
  m[3][2] = m_3_2;
  m[3][3] = m_3_3;

}

} // namespace shinybear