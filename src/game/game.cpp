#include "game.hpp"

#include "scene/IntroScene.hpp"
#include "scene/TitleScene.hpp"
#include "scene/GameScene.hpp"
#include <window/glfw.hpp>

Game::Game() {
  Scenes["intro"] = new IntroScene("intro");
  Engine::SceneKey = "intro";
}