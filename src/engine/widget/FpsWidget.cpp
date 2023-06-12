#include "FpsWidget.hpp"

#include <OpenGL/gl.h>
#include <FTGL/ftgl.h>
#include <utilities/format.cpp>

bool FpsWidgetSystem(int x, int y) {
  static int frameCount = 0;
  static int lastFrameCount = 0;
  static FTPixmapFont font("res/font/D2Coding.ttf");
  static chrono::time_point<chrono::steady_clock> checkTime;

  auto now = chrono::steady_clock::now();
  auto diff = chrono::duration_cast<chrono::microseconds>(now - checkTime);

  if (diff.count() >= 1000000) {
    lastFrameCount = frameCount;
    frameCount = 0;
    checkTime = now;

  } else {
    ++frameCount;
  }

  glColor3f(0, 1, 0);
  glRasterPos2d(x, y);
  font.FaceSize(50);
  font.Render(Format("FPS: %d", lastFrameCount).c_str());
  
  return true;
}