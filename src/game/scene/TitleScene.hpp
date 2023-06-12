#ifndef TITLE_SCENE_HPP
#define TITLE_SCENE_HPP

#include <FTGL/ftgl.h>
#include <engine/engine.hpp>
#include <engine/audio/audio.hpp>
#include <utilities/format.cpp>

#include <nlohmann/json.hpp>
using namespace nlohmann;

class TitleScene : public Scene {
public:
  TitleScene(string key);

private:
  FTGLPixmapFont* fontD2Coding;
  
  bool reset(json& scene);
  bool draw(json& scene);
  bool keyboard(json& scene);
  bool stageAnimation(json& scene);
  bool checkMode(json& scene);
};

#endif // TITLE_SCENE_HPP