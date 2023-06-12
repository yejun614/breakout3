#ifndef DEVELOPER_SCENE_HPP
#define DEVELOPER_SCENE_HPP

#include <FTGL/ftgl.h>
#include <engine/core/scene.hpp>

class DeveloperScene : public Scene {
public:
  DeveloperScene(string key);

private:
  vector<string> texts;
  float yscroll = 0;
  float xscroll = 0;
  FTGLPixmapFont* font;

  bool draw(json& scene);
  bool keyboard(json& scene);
};

#endif // DEVELOPER_SCENE_HPP