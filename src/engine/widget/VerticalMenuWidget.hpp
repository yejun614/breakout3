#ifndef VERTICAL_MENU_WIDGET_HPP
#define VERTICAL_MENU_WIDGET_HPP

#include <vector>
#include <string>
#include <functional>
using namespace std;

#include <FTGL/ftgl.h>

#include "widget.hpp"

typedef function<void (string title)> MenuActionFunc;

struct MenuItem {
  string title;
  MenuActionFunc func;
};

class VerticalMenuWidget : public Widget {
public:
  string Title;
  unsigned int Cursor;
  unsigned int Scroll;
  float ItemHeight;

  VerticalMenuWidget(string title);
  void Draw();
  unsigned int AddMenu(string title, MenuActionFunc action);
  void RemoveMenu(unsigned int id);
  void RemoveMenu(string title);
  void KeyboardControlSystem();

private:
  FTGLPixmapFont* fontNormal;
  FTGLPixmapFont* fontBold;
  vector<MenuItem> data;
};

#endif // VERTICAL_MENU_WIDGET_HPP