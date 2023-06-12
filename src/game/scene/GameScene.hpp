#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP

#include <FTGL/ftgl.h>

#include <engine/engine.hpp>
#include <engine/audio/audio.hpp>
#include <engine/widget/VerticalMenuWidget.hpp>
#include <utilities/format.cpp>

#include <nlohmann/json.hpp>
using namespace nlohmann;

enum GameStatus {
  PLAYING,
  GAME_OVER,
  GAME_CLEAR
};

class GameScene : public Scene {
public:
  GameScene(string key, int level);
  ~GameScene();

protected:
  FTGLPixmapFont* font;
  bool isPause;
  VerticalMenuWidget pauseMenu;
  function<void ()> nextLevel;
  function<void ()> retryLevel;

  void addBlock(float x, float y, float width, float height, int totalWidth, int totalHeight, json data);
  void addGameSystems();
  bool ballCollision(float x1, float y1, float x2, float y2);
  bool reset(json& scene);
  bool addBlocks(json& scene);
  bool draw(json& scene);
  bool timer(json& scene);
  bool stageKeyboard(json& scene);
  bool ballMoving(json& scene);
  bool stageCollsion(json& scene);
  bool blockCollision(json& scene);
  bool screenCollision(json& scene);
  bool gameOverCheck(json& scene);
  bool gameStartCounter(json& scene);
  bool drawGameClear(json& scene);
  bool drawGameOver(json& scene);
  bool stageAnimation(json& scene);
  bool ui(json& scene);
  bool pause(json& scene);
};

#endif // GAME_SCENE_HPP