#include "IntroScene.hpp"

#include <engine/engine.hpp>
#include <engine/image/image.hpp>
#include <engine/widget/VerticalMenuWidget.hpp>
#include <game/scene/TitleScene.hpp>

IntroScene::IntroScene(string key) : Scene(key) {
  ADD_SYSTEM(10, reset);
  ADD_SYSTEM(5, update);
  ADD_SYSTEM(2, draw);
}

bool IntroScene::reset(json& scene) {
  progressBarWidget.Reset();
  progressBarWidget.Value = 0;
  progressBarWidget.Box = { -100, 15, 200, 25 };

  Image::Load("title-background", "res/img/TitleBackground.jpg");

  return false;
}

bool IntroScene::draw(json& scene) {
  progressBarWidget.Draw();

  return true;
}

bool IntroScene::update(json& scene) {
  ++progressBarWidget.Value;

  if (progressBarWidget.Value >= 100) {
    Engine::Scenes["title"] = new TitleScene("title");
    Engine::SceneKey = "title";
    return false;
  }

  return true;
}