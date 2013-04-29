#ifndef SHINYBEAR_SCENENODE_H
#define SHINYBEAR_SCENENODE_H

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
  virtual const D3DXMATRIX &GetTransform() const;

protected:
  virtual void UpdateChildren(double elapsedSeconds);
  virtual void RenderChildren(SceneManager *pScene);

  SceneNode *m_pParent;
  std::vector<SceneNode *> m_children;

private:
  D3DXMATRIX scaleMat;
  D3DXMATRIX rotation;
  D3DXMATRIX translation;
  D3DXMATRIX m_local;
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
  D3DXMatrixTranslation(&translation, x, y, z);
  m_local *= translation;
}

inline void SceneNode::Rotate(float x, float y, float z)
{
  D3DXMatrixRotationYawPitchRoll(&rotation, x, y, z);
  m_local *= rotation;
}

inline void SceneNode::Scale(float scale)
{
  D3DXMatrixScaling(&scaleMat, scale, scale, scale);
  m_local *= scaleMat;
}

inline void SceneNode::LoadIdentity()
{
  D3DXMatrixIdentity(&translation);
  D3DXMatrixIdentity(&rotation);
  D3DXMatrixIdentity(&scaleMat);
}

inline void SceneNode::SetParent(SceneNode *pNewParent)
{
  m_pParent = pNewParent;
}

inline const SceneNode *SceneNode::GetParent() const
{
  return m_pParent;
}

inline const D3DXMATRIX &SceneNode::GetTransform() const
{
  return m_world;
}

} // namespace shinybear

#endif // SHINYBEAR_SCENENODE_H