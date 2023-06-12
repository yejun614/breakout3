#include "ProgressBarWidget.hpp"

#include <OpenGL/gl.h>
#include <engine/core/logger.hpp>
#include <engine/widget/TextBox.hpp>
#include <utilities/format.cpp>

void ProgressBarWidget::Reset() {
  font = new FTGLPixmapFont("res/font/D2Coding.ttf");
  if (font->Error()) {
    Log("ProgressBarWidget::Reset", "ERROR Failed load font");
  }
}

void ProgressBarWidget::Draw(bool isText) {
  Widget::Draw();

  BoxCoord innerBox = GetInnerBox();
  innerBox.width = innerBox.width * (Value / 100.0f);

  glBegin(GL_QUADS);
  glVertex2f(innerBox.x, innerBox.y);
  glVertex2f(innerBox.x + innerBox.width, innerBox.y);
  glVertex2f(innerBox.x + innerBox.width, innerBox.y - innerBox.height);
  glVertex2f(innerBox.x, innerBox.y - innerBox.height);
  glEnd();

  TextBox textBox(Format("%d%%", (int)Value), font);
  textBox.Box = Box;
  if (Value < 50) {
    textBox.FontColor = { 1, 1, 1 };
  } else {
    textBox.FontColor = { 0, 0, 0 };
  }
  textBox.Layout.SetAlignment(FTGL::ALIGN_CENTER);
  unsigned int faceSize = abs(innerBox.height) * 2;
  font->FaceSize(faceSize);
  textBox.Draw();
}