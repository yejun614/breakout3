#include "widget.hpp"

#include <OpenGL/gl.h>

Widget::Widget() {
  Box = { 0, 0, 100, 100 };
  Margin = { 0, 0, 0, 0 };
  Padding = { 0, 0, 0, 0 };
  BorderColor = { 0.8f, 0.8f, 0.8f };
  BackgroundColor = { 0.2f, 0.2f, 0.2f };
}

BoxCoord Widget::GetInnerBox() {
  return {
    Box.x - Margin.left,
    Box.y - Margin.top,
    Box.width - Margin.right,
    Box.height + Margin.bottom
  };
}

void Widget::Draw() {
  glColor3f(BackgroundColor.red, BackgroundColor.green, BackgroundColor.blue);
  glBegin(GL_QUADS);
  glVertex2f(Box.x, Box.y);
  glVertex2f(Box.x + Box.width, Box.y);
  glVertex2f(Box.x + Box.width, Box.y - Box.height);
  glVertex2f(Box.x, Box.y - Box.height);
  glEnd();

  glLineWidth(5);
  glColor3f(BorderColor.red, BorderColor.green, BorderColor.blue);
  glBegin(GL_LINE_LOOP);
  glVertex2f(Box.x, Box.y);
  glVertex2f(Box.x + Box.width, Box.y);
  glVertex2f(Box.x + Box.width, Box.y - Box.height);
  glVertex2f(Box.x, Box.y - Box.height);
  glEnd();
}