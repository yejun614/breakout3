#include "ChallengeScene.hpp"

#include <cstdlib>
#include <ctime>

#include <game/scene/TitleScene.hpp>
#include <engine/widget/TextBox.hpp>

ChallengeScene::ChallengeScene(string key) : GameScene(key, 0) {
  srand(time(NULL));

  nextLevel = [this](){
    Engine::Scenes["title"] = new TitleScene("title");
    Engine::SceneKey = "title";
  };
  retryLevel = [this](){
    Engine::Scenes["challenge"] = new ChallengeScene("challenge");
    Engine::SceneKey = "challenge";
  };

  ADD_SYSTEM(9, reset);
  ADD_SYSTEM(5, addMoreBlocks);
  ADD_SYSTEM(5, timeover);
  ADD_SYSTEM(1, ui);
}

bool ChallengeScene::reset(json& scene) {
  int playTime = 90;
  scene["Player"]["time"] = playTime;
  scene["Player"]["total_time"] = playTime;
  scene["Stage"]["edges"] = rand() % 6 + 4;
}

bool ChallengeScene::addMoreBlocks(json& scene) {
  static bool isGen = true;
  static int index = 0;

  int blockCount = scene["Player"]["block_count"];

  if (blockCount > 25) {
    isGen = false;
    index = 0;

  } else if (blockCount <= 1) {
    isGen = true;
    Audio::Play("item-add-blocks");
    scene["Stage"]["edges"] = rand() % 6 + 4;
    scene["Blocks"] = {};
    index = 0;
  }

  if (isGen) {
    int xpos = index % 5;
    int ypos = index / 5;
    
    addBlock(xpos, ypos, 50, 10, 5, 5, {
      { "color", { 0, 0, 1 } },
      { "type", "normal" }
    });

    ++index;
  }

  return true;
}

bool ChallengeScene::ui(json& scene) {
  int score = scene["Player"]["score"];

  font->FaceSize(50);
  TextBox textbox(Format("SCORE: %05d", score), font);
  textbox.FontColor = { 0, 1, 0 };
  textbox.Box = { screen.right - 120, screen.top - 10, 100, 50};
  textbox.Layout.SetAlignment(FTGL::ALIGN_RIGHT);
  textbox.Draw();

  return true;
}

bool ChallengeScene::timeover(json& scene) {
  int time = scene["Player"]["time"];

  if (time <= 0) {
    scene["Blocks"] = {};
    scene["Player"]["time"] = 1;
    scene["Player"]["game_status"] = GAME_CLEAR;

    Audio::Stop("countdown");
    Audio::Stop("game-background");
    Audio::Play("game-clear", 1);
    ADD_SYSTEM(3, stageAnimation);
    ADD_SYSTEM(3, drawGameClear);

    return false;
  }

  return true;
}