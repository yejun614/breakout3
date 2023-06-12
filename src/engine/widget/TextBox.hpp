#ifndef TEXT_BOX_HPP
#define TEXT_BOX_HPP

#include <string>
using namespace std;

#include <FTGL/ftgl.h>

#include "widget.hpp"

class TextBox : public Widget {
public:
  string Text;
  Color FontColor;
  FTSimpleLayout Layout;

  TextBox(string text, FTFont* font);
  void Draw(bool isDrawRect = false);

private:
};

#endif // TEXT_BOX_HPP