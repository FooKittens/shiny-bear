#ifndef SHINYBEAR_PLAYERVIEW_H
#define SHINYBEAR_PLAYERVIEW_H

#include "view\IGameView.h"
#include "base\system\GraphicsProvider.h"
#include "events\IEventListener.h"
#include "view\graphics\SceneComposer.h"

namespace shinybear {

// A view that renders the gamelogic.
// Potential playerviews can derive from this.
class RenderView : public IGameView, public IEventListener {
public:
  RenderView(GraphicsProvider *provider);

  void Update();

  const char *GetName() const { return "RenderView"; }
  bool HandleEvent(const EventPtr &evt);

protected:
  GraphicsProvider *m_pProvider;
  SceneComposer m_composer;

  virtual void StopRendering(bool val);

private:
  bool m_doRender;
};

} // namespace shinybear

#endif // SHINYBEAR_PLAYERVIEW_H