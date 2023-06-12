#ifndef WIDGET_HPP
#define WIDGET_HPP

struct BoxCoord {
  float x, y, width, height;
};

struct BoxDir {
  float top, right, bottom, left;
};

struct Color {
  float red, green, blue;
};

class Widget {
public:
  BoxCoord Box;
  BoxDir Margin;
  BoxDir Padding;
  Color BorderColor;
  Color BackgroundColor;

  Widget();
  BoxCoord GetInnerBox();
  void Draw();
};

#endif // WIDGET_HPP