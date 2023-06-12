#include "TextBox.hpp"

#include <OpenGL/gl.h>

TextBox::TextBox(string text, FTFont* font) : Text(text) {
  Layout.SetFont(font);
  FontColor = { 1, 1, 1 };
}

void TextBox::Draw(bool isDrawRect) {
  if (isDrawRect) {
    Widget::Draw();
  }

  glColor3f(1, 1, 1);

  FTFont* font = Layout.GetFont();
  unsigned int faceSize = font->FaceSize();

  BoxCoord innerBox = GetInnerBox();
  Layout.SetLineLength(Box.width * 3.0f);

  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  
  glRasterPos2f(innerBox.x, innerBox.y - (faceSize / 3.0f));
  glPixelTransferf(GL_RED_BIAS, FontColor.red - 1.0f);
  glPixelTransferf(GL_GREEN_BIAS, FontColor.green - 1.0f);
  glPixelTransferf(GL_BLUE_BIAS, FontColor.blue - 1.0f);
  glColor3f(FontColor.red, FontColor.green, FontColor.blue);
  
  Layout.Render(Text.c_str());
}