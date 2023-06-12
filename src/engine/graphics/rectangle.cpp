#include "rectangle.hpp"

void DrawRectangle(float x1, float y1, float x2, float y2) {
  glBegin(GL_LINE_STRIP);

  glVertex2f(x1, y1);
  glVertex2f(x2, y1);
  glVertex2f(x2, y2);
  glVertex2f(x1, y2);

  glEnd();
}

void DrawFilledRectangle(float x1, float y1, float x2, float y2) {
  if (y2 < y1) {
    // swap
    float buf = y1;
    y1 = y2;
    y2 = buf;
  }

  glBegin(GL_LINES);

  for (float y = y1; y <= y2; y += 0.1) {
    glVertex2f(x1, y);
    glVertex2f(x2, y);
  }

  glEnd();
}