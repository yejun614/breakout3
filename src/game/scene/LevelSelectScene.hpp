#ifndef LEVEL_SELECT_SCENE_HPP
#define LEVEL_SELECT_SCENE_HPP

#include <engine/core/scene.hpp>
#include <engine/widget/VerticalMenuWidget.hpp>

class LevelSelectScene : public Scene {
public:
  LevelSelectScene(string key);

private:
  VerticalMenuWidget menu;

  bool draw(json& scene);
  bool keyboard(json& scene);
};

#endif // LEVEL_SELECT_SCENE_HPP