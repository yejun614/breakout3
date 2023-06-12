#ifndef SCROLL_WIDGET_HPP
#define SCROLL_WIDGET_HPP

#include <functional>
using namespace std;

#include "widget.hpp"

class ScrollWidget : public Widget {
public:
  void Draw(function<void (BoxCoord innerBox)> callback);
};

#endif // SCROLL_WIDGET_HPP