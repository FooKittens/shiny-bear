#ifndef SHINYBEAR_SCENECOMPOSER_H
#define SHINYBEAR_SCENECOMPOSER_H


#include "events\IEventListener.h"

namespace shinybear { class GraphicsProvider; }

namespace shinybear 
{

class SceneComposer : public IEventListener
{
public:
  SceneComposer(GraphicsProvider *pProvider);
  ~SceneComposer();

  void RenderScene();

  // For IEventListener interface.
  const char *GetName() const { return "SceneComposer"; }
  bool HandleEvent(const EventPtr &evt);
private:
  GraphicsProvider *m_pProvider;
};

} // namespace shinybear

#endif // SHINYBEAR_SCENECOMPOSER_H
