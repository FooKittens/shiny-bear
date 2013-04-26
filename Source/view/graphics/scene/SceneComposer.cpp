#include "view\graphics\scene\SceneComposer.h"
#include "base\system\GraphicsProvider.h"

namespace shinybear
{

SceneComposer::SceneComposer(GraphicsProvider *pProvider)
{
  m_pProvider = pProvider;
}

SceneComposer::~SceneComposer() 
{

}

bool SceneComposer::HandleEvent(const EventPtr &evt) 
{


  return false;
}

void SceneComposer::RenderScene() 
{

}

} // namespace shinybear