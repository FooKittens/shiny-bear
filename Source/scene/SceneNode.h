#ifndef SHINYBEAR_SCENENODE_H
#define SHINYBEAR_SCENENODE_H

#include "util\math\Math.h"
#include <vector>
#include <d3dx9.h>

namespace shinybear { class SceneManager; class SceneView; }

namespace shinybear
{

class SceneNode
{
public:
  SceneNode();
  virtual ~SceneNode();

  virtual void Attach(SceneNode *pNode);
  virtual void Detach(SceneNode *pNode);

  virtual void Update(double elapsedSeconds);

  virtual void PreRender(SceneManager *pScene);
  virtual void Render(SceneManager *pScene);
  virtual void PostRender(SceneManager *pScene);

  virtual void OnDeviceLost();
  virtual void OnDeviceReset();

  bool IsVisible() const;
  void SetVisible(bool val);

  void Translate(float x, float y, float z);
  void Rotate(float radX, float radY, float radZ);
  void RotateX(float rad);
  void RotateZ(float rad);
  void RotateY(float rad);
  void Scale(float scale);
  void LoadIdentity();

  void SetParent(SceneNode *pNewParent);
  const SceneNode *GetParent() const;
  virtual const Mat4x4 &GetTransform() const;

  virtual void RemoveChildren();

protected:
  virtual void UpdateChildren(double elapsedSeconds);
  virtual void RenderChildren(SceneManager *pScene);

  SceneNode *m_pParent;
  std::vector<SceneNode *> m_children;

private:
  Mat4x4 m_scale;
  Mat4x4 m_rotation;
  Mat4x4 m_translation;
  Mat4x4 m_world;

  bool m_isVisible;
};

inline bool SceneNode::IsVisible() const
{
  return m_isVisible;
}

inline void SceneNode::SetVisible(bool val)
{
  m_isVisible = val;
}

inline void SceneNode::Translate(float x, float y, float z)
{
  Vector3 p = m_translation.GetPosition();

  p += m_rotation.Transform(Vector3(x, y, z));

  m_translation = Mat4x4::CreateTranslation(p);
}

inline void SceneNode::Rotate(float x, float y, float z)
{
  m_rotation *= Mat4x4::CreateYawPitchRoll(x, y, z);
}

inline void SceneNode::RotateX(float rad)
{
  m_rotation *= Mat4x4::CreateRotationX(rad);
}

inline void SceneNode::RotateY(float rad)
{
  m_rotation *= Mat4x4::CreateRotationY(rad);
}

inline void SceneNode::RotateZ(float rad)
{
  m_rotation *= Mat4x4::CreateRotationZ(rad);
}

inline void SceneNode::Scale(float scale)
{
  m_scale *= Mat4x4::CreateScale(scale);
}

inline void SceneNode::LoadIdentity()
{
  m_translation = Mat4x4::kIdentity;
  m_rotation    = Mat4x4::kIdentity;
  m_scale       = Mat4x4::kIdentity;
}

inline void SceneNode::SetParent(SceneNode *pNewParent)
{
  m_pParent = pNewParent;
}

inline const SceneNode *SceneNode::GetParent() const
{
  return m_pParent;
}

inline const Mat4x4 &SceneNode::GetTransform() const
{
  return m_world;
}

} // namespace shinybear

#endif // SHINYBEAR_SCENENODE_H