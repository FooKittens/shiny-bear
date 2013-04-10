#ifndef GRAPHICSSETTINGS_H
#define GRAPHICSSETTINGS_H

typedef unsigned int UINT;

namespace shinybear {

struct MultiSampleMode {
  MultiSampleMode() : samples(1), quality(0) { }
  MultiSampleMode(UINT sa, UINT qa) : samples(sa), quality(qa) { }

  UINT samples;
  UINT quality;
};

struct DisplayMode {
  DisplayMode() {
    width = 640;
    height = 480;
    refreshRate = 60;
    fullscreen = false;
  }
  
  DisplayMode(UINT w, UINT h, UINT hz, bool fullscreen) {
      width = w;
      height = h;
      refreshRate = hz;
      fullscreen = fullscreen;
  }

  UINT width;
  UINT height;
  UINT refreshRate;
  bool fullscreen;
};


} // namepsace shinybear

#endif