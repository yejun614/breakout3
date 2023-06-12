#ifndef INTRO_SCENE_HPP
#define INTRO_SCENE_HPP

#include <engine/core/scene.hpp>
#include <engine/widget/ProgressBarWidget.hpp>

class IntroScene : public Scene {
public:
  IntroScene(string key);

private:
  ProgressBarWidget progressBarWidget;

  bool reset(json& scene);
  bool draw(json& scene);
  bool update(json& scene);
};

#endif // INTRO_SCENE_HPP