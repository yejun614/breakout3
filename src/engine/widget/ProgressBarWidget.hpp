#ifndef PROGRESS_BAR_WIDGET_HPP
#define PROGRESS_BAR_WIDGET_HPP

#include <FTGL/ftgl.h>
#include "widget.hpp"

class ProgressBarWidget : public Widget {
public:
  void Reset();
  void Draw(bool isText = true);
  float Value = 50.0f;

private:
  FTGLPixmapFont* font;
};

#endif // PROGRESS_BAR_WIDGET_HPP