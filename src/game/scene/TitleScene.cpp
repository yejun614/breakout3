#include "TitleScene.hpp"

#include <engine/graphics/circle.hpp>
#include <engine/image/image.hpp>
#include <engine/widget/TextBox.hpp>
#include <game/scene/GameScene.hpp>
#include <game/scene/DeveloperScene.hpp>
#include <game/scene/LevelSelectScene.hpp>
#include <game/scene/ChallengeScene.hpp>

TitleScene::TitleScene(string key) : Scene(key) {
  ADD_SYSTEM(10, reset);
  ADD_SYSTEM(5, keyboard);
  ADD_SYSTEM(5, stageAnimation);
  ADD_SYSTEM(5, checkMode);
  ADD_SYSTEM(3, draw);
}

bool TitleScene::reset(json& scene) {
  fontD2Coding = new FTGLPixmapFont("res/font/D2Coding.ttf");
  if (fontD2Coding->Error()) {
    Log("TitleScene::draw", "ERROR Failed load font");
  }
  
  Audio::Load("title-song", "res/audio/penguinmusic-modern-chillout-future-calm-12641.mp3");
  ma_sound_set_looping(Audio::GetData("title-song").sound, true);
  Audio::Play("title-song");

  scene["Stage"] = {
    { "color", { 1, 1, 1 } },
    { "angle", M_PI / 4.0f },
    { "speed", 0.01f },
    { "scale", 3.0f },
    { "outline_scale1", 1.0f },
    { "outline_scale2", 1.0f },
    { "animation_reverse", false },
    { "title", "HELLO" }
  };

  scene["Mode"] = -1;

  scene["Key"] = {
    { "A", false },
    { "D", false }
  };

  return false;
}

bool TitleScene::draw(json& scene) {
  Image::Draw(
    "title-background",
    -screenScale / 1.5f,
    -screenScale / 1.5f,
    screenScale / 1.5f,
    screenScale / 1.5f
  );

  int mode = scene["Mode"];
  float scale = scene["Stage"]["scale"];
  float outlineScale1 = scene["Stage"]["outline_scale1"];
  float outlineScale2 = scene["Stage"]["outline_scale2"];
  float angle = scene["Stage"]["angle"];
  float step = (2 * M_PI) / 4;
  float rectScale = screenScale / scale;

  fontD2Coding->FaceSize(70);
  TextBox textBox1("A\nKEY", fontD2Coding);
  textBox1.Box = { screen.left + 20, 0, 60, 60 };
  textBox1.BorderColor = { 1, 1, 1 };
  textBox1.BackgroundColor = { 0, 0, 0 };
  textBox1.Layout.SetAlignment(FTGL::ALIGN_CENTER);
  textBox1.Draw(scene["Key"]["A"]);

  fontD2Coding->FaceSize(70);
  TextBox textBox2("D\nKEY", fontD2Coding);
  textBox2.Box = { screen.right - 80, 0, 60, 60 };
  textBox2.BorderColor = { 1, 1, 1 };
  textBox2.BackgroundColor = { 0, 0, 0 };
  textBox2.Layout.SetAlignment(FTGL::ALIGN_CENTER);
  textBox2.Draw(scene["Key"]["D"]);

  fontD2Coding->FaceSize(50);
  TextBox textBox3("Press 'SPACE' key to continue ...", fontD2Coding);
  textBox3.Box = { -150, screen.bottom + 50, 300, 30 };
  textBox3.BorderColor = { 1, 1, 1 };
  textBox3.BackgroundColor = { 0, 0, 0 };
  textBox3.Layout.SetAlignment(FTGL::ALIGN_CENTER);
  textBox3.Draw();

  fontD2Coding->FaceSize(100);
  TextBox textBox4(scene["Stage"]["title"], fontD2Coding);
  textBox4.Box = { -rectScale / 2.0f, rectScale / 2.0f, rectScale, rectScale };
  textBox4.BorderColor = { 1, 1, 1 };
  textBox4.BackgroundColor = { 0, 0, 0 };
  textBox4.Layout.SetAlignment(FTGL::ALIGN_LEFT);
  textBox4.Layout.SetLineSpacing(1.5f);
  textBox4.Draw(false);

  glPushMatrix();
  glLineWidth(5);

  auto color = scene["Stage"]["color"];
  glColor3f(color[0], color[1], color[2]);
  DrawCircle(angle, 0, 0, rectScale, 4);
  
  DrawCircle(angle, 0, 0, rectScale * outlineScale1, 4);
  DrawCircle(angle, 0, 0, rectScale * outlineScale2, 4);
  
  glPopMatrix();

  return true;
}

bool TitleScene::keyboard(json& scene) {
  if (WindowGetKey(GLFW_KEY_SPACE, 150000)) {
    Audio::Stop("title-song");

    int mode = scene["Mode"];
    if (mode == 0) {
      Engine::Scenes["game"] = new GameScene("game", 1);
      Engine::SceneKey = "game";
      
    } else if (mode == 1) {
      Engine::Scenes["challenge"] = new ChallengeScene("challenge");
      Engine::SceneKey = "challenge";
      
    } else if (mode == 2) {
      Engine::Scenes["level-select"] = new LevelSelectScene("level-select");
      Engine::SceneKey = "level-select";
      
    } else if (mode == 3) {
      Engine::Scenes["developers"] = new DeveloperScene("developers");
      Engine::SceneKey = "developers";
    }

  } else if (WindowGetKey(GLFW_KEY_A)) {
    float angle = scene["Stage"]["angle"];
    float speed = scene["Stage"]["speed"];

    angle += speed;
    if (angle >= 2 * M_PI) angle -= 2 * M_PI;

    scene["Stage"]["angle"] = angle;
    scene["Key"]["A"] = true;

  } else if (WindowGetKey(GLFW_KEY_D)) {
    float angle = scene["Stage"]["angle"];
    float speed = scene["Stage"]["speed"];

    angle -= speed;
    if (angle < 0) angle = (2 * M_PI) - angle;

    scene["Stage"]["angle"] = angle;
    scene["Key"]["D"] = true;

  } else {
    scene["Key"]["A"] = false;
    scene["Key"]["D"] = false;
  }

  return true;
}

bool TitleScene::stageAnimation(json& scene) {
  bool isReverse = scene["Stage"]["animation_reverse"];
  float outlineScale1 = scene["Stage"]["outline_scale1"];
  float outlineScale2 = scene["Stage"]["outline_scale2"];

  if (isReverse) {
    outlineScale1 += 0.01f;
    outlineScale2 += 0.05f;
    if (outlineScale1 >= 1.5f) isReverse = false;

  } else {
    outlineScale1 -= 0.01f;
    outlineScale2 -= 0.05f;
    if (outlineScale1 <= 1.0f) isReverse = true;
  }

  scene["Stage"]["animation_reverse"] = isReverse;
  scene["Stage"]["outline_scale1"] = outlineScale1;
  scene["Stage"]["outline_scale2"] = outlineScale2;

  return true;
}

bool TitleScene::checkMode(json& scene) {
  int mode = scene["Mode"];
  float angle = scene["Stage"]["angle"];
  float step = (2 * M_PI) / 4;
  float intensity = 0.5f;

  int newMode;
  string newTitle;
  json newColor;

  if (angle < step) {
    newMode = 0;
    newTitle = "THE\nBREAKOUT3\nGAME START";
    newColor = { intensity, intensity, intensity };

  } else if (angle < step * 2) {
    newMode = 1;
    newTitle = "PLAY\nCHALLENGE\nMODE";
    newColor = { intensity, 0, 0 };
    
  } else if (angle < step * 3) {
    newMode = 2;
    newTitle = "PLAY\nANY LEVEL\nYOU WANT";
    newColor = { 0, intensity, 0 };

  } else {
    newMode = 3;
    newTitle = "SHOW\nGAME\nDEVELOPERS";
    newColor = { 0, 0, intensity };
  }

  if (newMode != mode) {
    scene["Mode"] = newMode;
    scene["Stage"]["title"] = newTitle;
    scene["Stage"]["color"] = newColor;
  }

  return true;
}