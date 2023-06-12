#include "ScrollWidget.hpp"

#include <OpenGL/gl.h>

void ScrollWidget::Draw(function<void (BoxCoord innerBox)> callback) {
  BoxCoord innerBox = GetInnerBox();

  glPushMatrix();
  glScissor(innerBox.x, innerBox.y, innerBox.width, innerBox.height);
  glEnable(GL_SCISSOR_TEST);

  callback(innerBox);

  glDisable(GL_SCISSOR_TEST);
  glPopMatrix();
}