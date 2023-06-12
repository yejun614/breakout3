#include "stb_image.h"
#include "image.hpp"
#include <utilities/format.cpp>

map<string, ImageData> Image::data;

ImageData& Image::Load(string key, string path) {
  if (data.find(key) != data.end()) {
    return data[key];
  }

  ImageData image = {};
  image.path = path;

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glGenTextures(1, &image.texture);
  glBindTexture(GL_TEXTURE_2D, image.texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  stbi_set_flip_vertically_on_load(true);
  image.data = stbi_load(path.c_str(), &image.width, &image.height, &image.channels, 0);

  if (image.data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    Log("Image::Iamge", Format("ERROR Failed to load texture (%s)", path.c_str()));
  }

  stbi_image_free(image.data);

  data[key] = image;
  return image;
}

ImageData& Image::GetData(string key) {
  return data[key];
}

void Image::Draw(string key, float x1, float y1, float x2, float y2) {
  if (data.find(key) == data.end()) {
    Log("Image::Draw", Format("ERROR can not loaded image data (%s)", key.c_str()));
    return;
  }

  const ImageData& image = data[key];

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, image.texture);

  glPushMatrix();
  glBegin(GL_QUADS);
  glColor3f(1, 1, 1);

  glTexCoord2d(0, 1);
  glVertex2f(x1, y2);
  glTexCoord2d(1, 1);
  glVertex2f(x2, y2);
  glTexCoord2d(1, 0);
  glVertex2f(x2, y1);
  glTexCoord2d(0, 0);
  glVertex2f(x1, y1);

  glEnd();
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
}