#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <map>
#include <string>
using namespace std;

#include <OpenGL/gl.h>

#include <engine/core/logger.hpp>
#include <utilities/format.cpp>

struct ImageData {
  string path;
  GLuint texture;
  int width;
  int height;
  int channels;
  unsigned char *data;
};

static class Image {
public:
  static ImageData& Load(string key, string path);
  static void Draw(string key, float x1, float y1, float x2, float y2);
  static ImageData& GetData(string key);

private:
  static map<string, ImageData> data;
};

#endif // IMAGE_HPP