#ifndef IGRAPHICSPROVIDER_H
#define IGRAPHICSPROVIDER_H
#include "graphics\DisplayMode.h"

typedef unsigned int UINT;

namespace shinybear {

class IGraphicsProvider {
public:

  // Graphicsproviders should initialize all their functionality here.
  virtual bool Initialize() = 0;

  // This method should return true if the hardware
  // is capable of running the engine.
  virtual bool CheckHardware() = 0;

  virtual bool IsFullscreen() = 0;
  virtual bool ToggleFullscreen(bool value) = 0;

  // Returns the DisplayMode currently in use.
  virtual const DisplayMode& GetDisplayMode() = 0;
  // Set the current Displaymode
  virtual bool SetDisplayMode(const DisplayMode &displayMode) = 0; 
 
  // Return current multisample mode.
  virtual const MultiSampleMode& GetMultiSampleMode() = 0;
  // Set the current MultiSampleMode
  virtual bool SetMultiSampleMode(const MultiSampleMode &mode) = 0;

  // Returns an array of [numModes] entries of valid Displaymodes.
  virtual DisplayMode* GetValidDisplayModes(UINT *numModes) = 0;
  
  // returns an array of [numModes] entries of valid MultiSample configs.
  virtual MultiSampleMode *GetValidMultiSampleModes(UINT *numModes) = 0;
  


  // Should implement functionality to discover if the graphics device
  // has been lost.
  virtual bool IsDeviceLost() = 0;

  // Should implement functionality to recover the device on demand,
  // and notify any listeners of the event.
  virtual bool ResetDevice() = 0;

  // Should update any queued changes made to the device.
  virtual void ApplyChanges() = 0;
protected:

private:

};

} // namespace shinybear
#endif
