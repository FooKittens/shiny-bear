#ifndef SHINYBEAR_MATH_H
#define SHINYBEAR_MATH_H

#include <d3dx9.h>

namespace shinybear
{

///////////////////////////////////////
// Vector 2 ///////////////////////////
///////////////////////////////////////
#pragma region Vector2

class Vector2 : public D3DXVECTOR2
{
public:
  static const Vector2 kZero;
  static const Vector2 kUnitX;
  static const Vector2 kUnitY;

  Vector2() : D3DXVECTOR2(0, 0) { }
  Vector2(float x, float y) : D3DXVECTOR2(x, y) { }
  Vector2(const D3DXVECTOR2 &v) : D3DXVECTOR2(v) { }

  void Normalize();
  float Length() const;
  float Dot(const Vector2 &v) const;
  float DistanceTo(const Vector2 &v) const;

  static float Dot(const Vector2 &lhs, const Vector2 &rhs);
  static float Distance(const Vector2 &lhs, const Vector2 &rhs);
  static Vector2 Normalize(const Vector2 &v);

private:

};

inline void Vector2::Normalize()
{
  D3DXVec2Normalize(this, this);
}
inline float Vector2::Length() const
{
  return D3DXVec2Length(this);
}

inline float Vector2::Dot(const Vector2 &v) const
{
  return D3DXVec2Dot(this, &v);
}

inline float Vector2::DistanceTo(const Vector2 &v) const
{
  return static_cast<Vector2>(v - *this).Length();
}

inline float Vector2::Dot(const Vector2 &lhs, const Vector2 &rhs)
{
  return lhs.Dot(rhs);
}

inline float Vector2::Distance(const Vector2 &lhs, const Vector2 &rhs)
{
  return lhs.DistanceTo(rhs);
}

inline Vector2 Vector2::Normalize(const Vector2 &v)
{
  Vector2 v2 = v;
  v2.Normalize();
  return v2;
}

#pragma endregion

///////////////////////////////////////
// Vector 4 ///////////////////////////
///////////////////////////////////////
#pragma region Vector4

class Vector4 : public D3DXVECTOR4
{
public:
  const static Vector4 kZero;
  const static Vector4 kUnitX;
  const static Vector4 kUnitY;
  const static Vector4 kUnitZ;
  const static Vector4 kUnitW;

  Vector4() { }
  Vector4(const D3DXVECTOR4 &v) :D3DXVECTOR4(v) { }
  Vector4(float x, float y, float z, float w)
    :D3DXVECTOR4(x, y, z, w) { }

  void Normalize();
  float Length() const;
  float Dot(const Vector4 &v) const;
  float DistanceTo(const Vector4 &v) const;

  static Vector4 Normalize(const Vector4 &v);
  static float Dot(const Vector4 &lhs, const Vector4 &rhs);
  static float Distance(const Vector4 &lhs, const Vector4 &rhs);

private:

};

inline float Vector4::DistanceTo(const Vector4 &v) const
{
  Vector4 diff = (v - (*this));
  return D3DXVec4Length(&diff);
}

inline float Vector4::Distance(const Vector4 &v, const Vector4 &u)
{
  return static_cast<Vector4>(u - v).Length();
}

inline float Vector4::Length() const
{
  return D3DXVec4Length(this);
}

inline void Vector4::Normalize()
{
  D3DXVec4Normalize(this, this);
}

inline float Vector4::Dot(const Vector4 &v) const
{
  return D3DXVec4Dot(this, &v);
}

inline float Vector4::Dot(const Vector4 &lhs, const Vector4 &rhs)
{
  return D3DXVec4Dot(&lhs, &rhs);
}

inline Vector4 Vector4::Normalize(const Vector4 &v)
{
  Vector4 v2 = v;
  v2.Normalize();
  return v2;
}

#pragma endregion

///////////////////////////////////////
// Vector 3 ///////////////////////////
///////////////////////////////////////
#pragma region Vector3

class Vector3 : public D3DXVECTOR3
{
public:
  static const Vector3 kZero;
  static const Vector3 kUnitX;
  static const Vector3 kUnitY;
  static const Vector3 kUnitZ;

  Vector3() { }
  Vector3(const D3DXVECTOR3 &v) :D3DXVECTOR3(v) { }
  Vector3(const D3DXVECTOR4 &v) { x = v.x; y = v.y; z = v.z; }
  Vector3(float x, float y, float z) :D3DXVECTOR3(x, y, z) { }
  Vector3(const Vector4 &v4) { x = v4.x; y = v4.y; z = v4.z; }

  void Normalize();
  float Length() const;
  float Dot(const Vector3 &vec) const;
  Vector3 Cross(const Vector3 &rhs) const;
  float DistanceTo(const Vector3 &v) const;

  static float Dot(const Vector3 &lhs, const Vector3 &rhs);
  static Vector3 Cross(const Vector3 &lhs, const Vector3 &rhs);
  static float Distance(const Vector3 &lhs, const Vector3 &rhs);
  static Vector3 Normalize(const Vector3 &v);

private:

};

inline void Vector3::Normalize()
{
  D3DXVec3Normalize(this, this);
}

inline float Vector3::Length() const
{
  return D3DXVec3Length(this);
}

inline float Vector3::Dot(const Vector3 &rhs) const
{
  return D3DXVec3Dot(this, &rhs);
}

inline Vector3 Vector3::Cross(const Vector3 &rhs) const
{
  Vector3 cross;
  D3DXVec3Cross(&cross, this, &rhs);
  return cross;
}

inline float Vector3::DistanceTo(const Vector3 &rhs) const
{
  return static_cast<Vector3>(rhs - *this).Length();
}

inline float Vector3::Dot(const Vector3 &lhs, const Vector3 &rhs)
{
  return lhs.Dot(rhs);
}

inline Vector3 Vector3::Cross(const Vector3 &lhs, const Vector3 &rhs)
{
  return lhs.Cross(rhs);
}

inline float Vector3::Distance(const Vector3 &lhs, const Vector3 &rhs)
{
  return lhs.DistanceTo(rhs);
}

inline Vector3 Vector3::Normalize(const Vector3 &vec)
{
  Vector3 v = vec;
  v.Normalize();
  return v;  
}

#pragma endregion

///////////////////////////////////////
// MATRIX 4X4 /////////////////////////
///////////////////////////////////////
#pragma region Mat4x4

class Mat4x4 : public D3DXMATRIX
{
public:
  static const Mat4x4 kIdentity;

  Mat4x4() { *this = kIdentity; }

  Mat4x4(const Mat4x4 &mat) { memcpy(&m, &mat.m, sizeof(mat.m)); }
  Mat4x4(const D3DXMATRIX &mat) { memcpy(&m, &mat.m, sizeof(mat.m)); }

  Mat4x4(float m_0_0, float m_0_1, float m_0_2, float m_0_3,
         float m_1_0, float m_1_1, float m_1_2, float m_1_3,
         float m_2_0, float m_2_1, float m_2_2, float m_2_3,
         float m_3_0, float m_3_1, float m_3_2, float m_3_3);

  static Mat4x4 CreateRotationX(float rad);
  static Mat4x4 CreateRotationY(float rad);
  static Mat4x4 CreateRotationZ(float rad);

  static Mat4x4 CreateYawPitchRoll(float yaw, float pitch, float roll);
  static Mat4x4 CreateAxisAngle(const Vector3 &axis, float angle);

  static Mat4x4 CreateQuaternionRotation(const class Quaternion &q);

  static Mat4x4 CreateTranslation(float x, float y, float z);
  static Mat4x4 CreateTranslation(const Vector3 &vec);

  static Mat4x4 CreateScale(float rad);
  static Mat4x4 CreateScale(float scaleX, float scaleY, float scaleZ);

  static Mat4x4 CreateLookAt(const Vector3 &eye, const Vector3 &target, const Vector3 &up);

  static Mat4x4 CreatePerspectiveFovLH(float aspect, float fov, float znear, float zfar);

  Vector3 GetPosition() const;
  void SetPosition(const Vector3 &pos);
  void SetPosition(const Vector4 &pos);
  void SetPosition(float x, float y, float z);

  void Transform(Vector3 * const vec) const;
  Vector3 Transform(const Vector3 &vec) const;

  void Transform(Vector4 * const vec) const;
  Vector4 Transform(const Vector4 &vec) const;

  Mat4x4 Inverse() const;
  float Determinant() const;

  Mat4x4 Transpose() const;

private:

};


inline Mat4x4 operator*(const Mat4x4 &lhs, float s)
{
  Mat4x4 res = lhs;
  res *= s;
  return res;
}

inline Mat4x4 Mat4x4::Transpose() const
{
  Mat4x4 m;
  D3DXMatrixTranspose(&m, this);
  return m;
}

inline Mat4x4 Mat4x4::CreateLookAt(const Vector3 &eye, const Vector3 &target,
  const Vector3 &up)
{
  Mat4x4 m;
  D3DXMatrixLookAtLH(&m, &eye, &target, &up);
  return m;
}

inline Mat4x4 Mat4x4::CreatePerspectiveFovLH(float aspect, float fov, float zn, float zf)
{
  Mat4x4 m;
  D3DXMatrixPerspectiveFovLH(&m, fov, aspect, zn, zf);
  return m;
}

inline float Mat4x4::Determinant() const
{
  return D3DXMatrixDeterminant(this);
}

inline Mat4x4 Mat4x4::Inverse() const
{
  Mat4x4 mat;
  D3DXMatrixInverse(&mat, NULL, this);
  return mat;
}

inline void Mat4x4::Transform(Vector3 * const vec) const
{
  Vector4 v;
  D3DXVec3Transform(&v, vec, this);
  vec->x = v.x;
  vec->y = v.y;
  vec->z = v.z;
}

inline Vector3 Mat4x4::Transform(const Vector3 &vec) const
{
  Vector4 v;
  D3DXVec3Transform(&v, &vec, this);
  return Vector3(v.x, v.y, v.z);
}

inline void Mat4x4::Transform(Vector4 * const vec) const
{
  D3DXVec4Transform(vec, vec, this);
}

inline Vector4 Mat4x4::Transform(const Vector4 &vec) const
{
  Vector4 v;
  D3DXVec4Transform(&v, &vec, this);
  return v;
}

inline Vector3 Mat4x4::GetPosition() const
{
  return Vector3(m[3][0], m[3][1], m[3][2]);
}

inline void Mat4x4::SetPosition(const Vector3 &pos)
{
  m[3][0] = pos.x;
  m[3][1] = pos.y;
  m[3][2] = pos.z;
  m[3][3] = 1.0f;
}

inline void Mat4x4::SetPosition(const Vector4 &pos)
{
  m[3][0] = pos.x;
  m[3][1] = pos.y;
  m[3][2] = pos.z;
  m[3][3] = pos.w;
}

inline void Mat4x4::SetPosition(float x, float y, float z)
{
  m[3][0] = x;
  m[3][1] = y;
  m[3][2] = z;
  m[3][3] = 1.0f;
}

inline Mat4x4 Mat4x4::CreateRotationX(float rad)
{
  Mat4x4 mat;
  D3DXMatrixRotationX(&mat, rad);
  return mat;
}

inline Mat4x4 Mat4x4::CreateRotationY(float rad)
{
  Mat4x4 mat;
  D3DXMatrixRotationY(&mat, rad);
  return mat;
}

inline Mat4x4 Mat4x4::CreateRotationZ(float rad)
{
  Mat4x4 mat;
  D3DXMatrixRotationZ(&mat, rad);
  return mat;
}

inline Mat4x4 Mat4x4::CreateYawPitchRoll(float yaw, float pitch, float roll)
{
  Mat4x4 mat;
  D3DXMatrixRotationYawPitchRoll(&mat, yaw, pitch, roll);
  return mat;
}

inline Mat4x4 Mat4x4::CreateAxisAngle(const Vector3 &axis, float angle)
{
  Mat4x4 mat;
  D3DXMatrixRotationAxis(&mat, &axis, angle);
  return mat;
}

inline Mat4x4 Mat4x4::CreateScale(float scale)
{
  Mat4x4 m;
  D3DXMatrixScaling(&m, scale, scale, scale);
  return m;
}

inline Mat4x4 Mat4x4::CreateScale(float sx, float sy, float sz)
{
  Mat4x4 m;
  D3DXMatrixScaling(&m, sx, sy, sz);
  return m;
}

inline Mat4x4 Mat4x4::CreateTranslation(const Vector3 &vec)
{
  Mat4x4 mat;
  D3DXMatrixTranslation(&mat, vec.x, vec.y, vec.z);
  return mat;
}

inline Mat4x4 Mat4x4::CreateTranslation(float x, float y, float z)
{
  Mat4x4 mat;
  D3DXMatrixTranslation(&mat, x, y, z);
  return mat;
}

#pragma endregion

///////////////////////////////////////
// Quaternion /////////////////////////
///////////////////////////////////////
#pragma region Quaternion

class Quaternion: public D3DXQUATERNION
{
public:
  static const Quaternion kIdentity;

  Quaternion(const D3DXQUATERNION &q) :D3DXQUATERNION(q) { }
  Quaternion() { *this = kIdentity; }

  static Quaternion Slerp(const Quaternion &start,
    const Quaternion &end, float weight);

  static Quaternion Normalize(const Quaternion &quat);
  static Quaternion CreateYawPitchRoll(float yaw, float pitch, float roll);
  static Quaternion CreateAxisAngle(const Vector3 &axis, float angle);
  static Quaternion CreateFromMatrix(const Mat4x4 &mat);

  void Normalize();
  float Length() const;
 
  void GetAxisAngle(Vector3 * const axis, float *angle) const;


private:

};

inline Quaternion operator*(const Quaternion &lhs, const Quaternion &rhs)
{
  Quaternion q;
  D3DXQuaternionMultiply(&q, &lhs, &rhs);
  return q;
}

inline Quaternion Quaternion::CreateYawPitchRoll(float yaw, float pitch, float roll)
{
  Quaternion quat;
  D3DXQuaternionRotationYawPitchRoll(&quat, yaw, pitch, roll);
  return quat;
}

inline Quaternion Quaternion::CreateAxisAngle(const Vector3 &axis, float angle)
{
  Quaternion quat;
  D3DXQuaternionRotationAxis(&quat, &axis, angle);
  return quat;
}

inline Quaternion Quaternion::CreateFromMatrix(const Mat4x4 &mat)
{
  Quaternion quat;
  D3DXQuaternionRotationMatrix(&quat, &mat);
  return quat;
}

inline Quaternion Quaternion::Slerp(const Quaternion &start, const Quaternion &end,
  float weight)
{
  Quaternion quat;
  D3DXQuaternionSlerp(&quat, &start, &end, weight);
  return quat;
}

inline Quaternion Quaternion::Normalize(const Quaternion &quat)
{
  Quaternion q;
  D3DXQuaternionNormalize(&q, &quat);
  return q;
}

inline void Quaternion::Normalize()
{
  D3DXQuaternionNormalize(this, this);
}

inline float Quaternion::Length() const
{
  return D3DXQuaternionLength(this);
}

#pragma endregion


// Put here because of dependancy on Quaternion definition.
inline Mat4x4 Mat4x4::CreateQuaternionRotation(const Quaternion &q)
{
  Mat4x4 mat;
  D3DXMatrixRotationQuaternion(&mat, &q);
  return mat;
}

} // namespace shinybear

#endif // SHINYBEAR_MATH_H