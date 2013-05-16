#ifndef SHINYBEAR_SSAOSHADER_H
#define SHINYBEAR_SSAOSHADER_H


#include <d3d9.h>

// Forward declarations
namespace shinybear
{
  class GraphicsProvider; class Shader;
  class RenderTarget; class Texture;
  class Camera; class QuadRenderer;
}

namespace shinybear
{

class SSAOShader
{
public:
  SSAOShader(GraphicsProvider *pProvider);
  ~SSAOShader();

  // Called to initialize shader.
  void Initialize();

  // Renders the occlusion map from a depth target.
  void RenderOcclusionMap(RenderTarget *pDepthTarget,
    RenderTarget *pNormalTarget, Camera *pCamera);

  RenderTarget * const GetAOTarget() const;
private:
  GraphicsProvider *m_pProvider;
  RenderTarget *m_pAOTarget;
  Shader *m_pShader;

  // Holds a noise texture for sampling random values.
  Texture *m_pNoiseTexture;
  QuadRenderer *m_pQuadRenderer;
};

inline RenderTarget * const SSAOShader::GetAOTarget() const
{
  return m_pAOTarget;
}

} // namespace shinybear

#endif // SHINYBEAR_SSAOSHADER_H