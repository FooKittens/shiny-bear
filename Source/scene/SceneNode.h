#ifndef SHINYBEAR_SCENENODE_H
#define SHINYBEAR_SCENENODE_H

#include <vector>
#include <d3dx9.h>

namespace shinybear
{

class SceneNode
{
public:
  SceneNode(SceneNode *pParent);
  virtual ~SceneNode();

  virtual void Attatch(SceneNode *pNode);
  virtual void Detatch(SceneNode *pNode);

  virtual void Update(double elapsedSeconds);
  virtual void Render();

  bool IsVisible() const;
  void SetVisible(bool val);

  void Translate(float x, float y, float z);
  void Rotate(float radX, float radY, float radZ);
  void Scale(float scale);
  void LoadIdentity();

protected:
  virtual void UpdateChildren(double elapsedSeconds);
  virtual void RenderChildren();

  SceneNode *m_pParent;
  std::vector<SceneNode *> m_children;

private:
  D3DXMATRIX m_world;
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
  D3DXMATRIX translation;
  D3DXMatrixTranslation(&translation, x, y, z);
  m_world *= translation;
}

inline void SceneNode::Rotate(float x, float y, float z)
{
  D3DXMATRIX rotation;
  D3DXMatrixRotationYawPitchRoll(&rotation, x, y, z);
  m_world *= rotation;
}

inline void SceneNode::Scale(float scale)
{
  D3DXMATRIX scaleMat;
  D3DXMatrixScaling(&scaleMat, scale, scale, scale);
  m_world *= scaleMat;
}

inline void SceneNode::LoadIdentity()
{
  D3DXMatrixIdentity(&m_world);
}

} // namespace shinybear

#endif // SHINYBEAR_SCENENODE_H