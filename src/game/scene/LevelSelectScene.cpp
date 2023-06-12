#include "LevelSelectScene.hpp"

#include <game/scene/TitleScene.hpp>
#include <game/scene/GameScene.hpp>

LevelSelectScene::LevelSelectScene(string key) : Scene(key), menu("LEVELS") {
  menu.Box = { -100, 120, 200, 240 };

  menu.AddMenu("Back to home", [](string title){
    Engine::Scenes["title"] = new TitleScene("title");
    Engine::SceneKey = "title";
  });
  
  for (int i = 1; i <= 10; ++i) {
    menu.AddMenu(Format("Lv. %02d", i), [i](string title){
      Engine::Scenes["game"] = new GameScene("game", i);
      Engine::SceneKey = "game";
    });
  }

  ADD_SYSTEM(3, draw);
  ADD_SYSTEM(5, keyboard);
}

bool LevelSelectScene::draw(json& scene) {
  menu.Draw();
  
  return true;
}

bool LevelSelectScene::keyboard(json& scene) {
  menu.KeyboardControlSystem();

  return true;
}