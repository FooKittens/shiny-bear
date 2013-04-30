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
  void Scale(float scale);
  void LoadIdentity();

  void SetParent(SceneNode *pNewParent);
  const SceneNode *GetParent() const;
  virtual const Mat4x4 &GetTransform() const;

protected:
  virtual void UpdateChildren(double elapsedSeconds);
  virtual void RenderChildren(SceneManager *pScene);

  SceneNode *m_pParent;
  std::vector<SceneNode *> m_children;

private:
  Mat4x4 scaleMat;
  Mat4x4 rotation;
  Mat4x4 translation;
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
  translation.SetPosition(x, y, z);
}

inline void SceneNode::Rotate(float x, float y, float z)
{
  rotation = Mat4x4::CreateYawPitchRoll(x, y, z);
}

inline void SceneNode::Scale(float scale)
{
  scaleMat = Mat4x4::CreateScale(scale);
}

inline void SceneNode::LoadIdentity()
{
  translation = Mat4x4::kIdentity;
  rotation    = Mat4x4::kIdentity;
  scaleMat    = Mat4x4::kIdentity;
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