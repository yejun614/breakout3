#include "GameScene.hpp"

#include <chrono>
using namespace std;

#include <glm/vec4.hpp>

#include <engine/graphics/circle.hpp>
#include <engine/graphics/rectangle.hpp>
#include <engine/math/math.hpp>
#include <engine/widget/TextBox.hpp>
#include <game/scene/TitleScene.hpp>
#include <game/scene/LevelSelectScene.hpp>

GameScene::GameScene(string key, int level) : Scene(key), pauseMenu("PAUSE") {
  isPause = false;
  scene["Player"]["level"] = level;

  pauseMenu.Box = { -75, 60, 150, 120 };
  pauseMenu.AddMenu("Resume", [this](string title){
    this->isPause = false;
    this->addGameSystems();
  });
  pauseMenu.AddMenu("Levels", [](string title){
    Audio::Stop("game-background");

    Engine::Scenes["level-select"] = new LevelSelectScene("level-select");
    Engine::SceneKey = "level-select";
  });
  pauseMenu.AddMenu("QUIT", [this](string title){
    Audio::Stop("game-background");

    Engine::Scenes["title"] = new TitleScene("title");
    Engine::SceneKey = "title";
  });

  nextLevel = [this](){
    int level = scene["Player"]["level"];

    if (level < 10) {
      Engine::Scenes["game"] = new GameScene("game", level + 1);
      Engine::SceneKey = "game";
    } else {
      Engine::Scenes["title"] = new TitleScene("title");
      Engine::SceneKey = "title";
    }
  };
  retryLevel = [this](){
    int level = scene["Player"]["level"];
    Engine::Scenes["game"] = new GameScene("game", level);
    Engine::SceneKey = "game";
  };

  ADD_SYSTEM(10, reset);
  ADD_SYSTEM(9, addBlocks);
  ADD_SYSTEM(4, draw);
  ADD_SYSTEM(2, gameStartCounter);
  ADD_SYSTEM(1, ui);
}

GameScene::~GameScene() {
  delete font;
}

void GameScene::addGameSystems() {
  ADD_SYSTEM(5, stageKeyboard);
  ADD_SYSTEM(5, stageCollsion);
  ADD_SYSTEM(5, blockCollision);
  ADD_SYSTEM(5, screenCollision);
  ADD_SYSTEM(3, ballMoving);
  ADD_SYSTEM(2, gameOverCheck);
  ADD_SYSTEM(2, timer);
}

bool GameScene::reset(json& scene) {
  font = new FTGLPixmapFont("res/font/D2Coding.ttf");

  if (font->Error()) {
    Log("GameScene::ui", "ERROR Failed load font");
  }

  Audio::Load("game-background", "res/audio/space_line-27593.mp3");
  ma_sound_set_looping(Audio::GetData("game-background").sound, true);
  Audio::Load("game-count", "res/audio/correct-2-46134.mp3");
  Audio::Load("game-start", "res/audio/start-computeraif.mp3");
  Audio::Load("block-broken", "res/audio/scale-e6-14577.mp3");
  Audio::Load("block-error", "res/audio/error-call-to-attention.mp3");
  Audio::Load("game-clear", "res/audio/new-level.mp3");
  Audio::Load("game-over", "res/audio/incorrect-buzzer-sound-147336.mp3");
  Audio::Load("item-time-up", "res/audio/message-incoming-132126.mp3");
  Audio::Load("item-time-down", "res/audio/beep-warning-6387.mp3");
  Audio::Load("item-add-blocks", "res/audio/buzzer-of-car-wash-107990.mp3");
  Audio::Load("countdown", "res/audio/female-robotic-countdown-5-to-1-47653.mp3");

  int level = scene["Player"]["level"];
  int playTime = 0;
  int edges = 3;

  if (level == 1) {
    playTime = 20;
    edges = 4;
  } else if (level == 2) {
    playTime = 30;
    edges = 6;
  } else if (level == 3) {
    playTime = 40;
    edges = 6;
  } else if (level == 4) {
    playTime = 60;
    edges = 8;
  } else if (level == 5) {
    playTime = 10;
    edges = 4;
  } else if (level == 6) {
    playTime = 100;
    edges = 6;
  } else if (level == 7) {
    playTime = 20;
    edges = 4;
  } else if (level == 8) {
    playTime = 30;
    edges = 8;
  } else if (level == 9) {
    playTime = 50;
    edges = 6;
  } else if (level == 10) {
    playTime = 60;
    edges = 5;
  }

  scene["Player"]["time"] = playTime;
  scene["Player"]["total_time"] = playTime;
  scene["Player"]["game_status"] = PLAYING;
  scene["Player"]["block_count"] = 0;
  scene["Player"]["score"] = 0;

  scene["StartCounter"] = {
    { "count", 4 },
    { "frame", 0 }
  };

  scene["Stage"] = {
    { "edges", edges },
    { "angle", 0.0f },
    { "speed", 0.01f },
    { "radius", 200 }
  };

  scene["Ball"] = {
    {
      "position",
      {
        { "xpos", 0 },
        { "ypos", 80 },
      }
    },
    {
      "velocity",
      {
        { "xpos", 1 },
        { "ypos", 1 },
      }
    },
    { "speed", 3 }
  };

  return false;
}

void GameScene::addBlock(float x, float y, float width, float height, int totalWidth, int totalHeight, json data) {
  static int index = 0;
  static float margin = 5;

  float xpos = x * (width + margin);
  float ypos = y * (height + margin);

  xpos -= ((width + margin) * totalWidth) / 2;
  ypos -= ((height + margin) * totalHeight) / 2;

  string key = Format("%d", ++index);

  scene["Blocks"][key] = {};
  scene["Blocks"][key]["xpos"] = xpos;
  scene["Blocks"][key]["ypos"] = ypos;
  scene["Blocks"][key]["width"] = width;
  scene["Blocks"][key]["height"] = height;
  scene["Blocks"][key]["type"] == "normal";

  for (json::iterator it = data.begin(); it != data.end(); ++it) {
    scene["Blocks"][key][it.key()] = it.value();
  }

  if (scene["Blocks"][key]["type"] == "normal") {
    scene["Player"]["block_count"] = scene["Player"]["block_count"].get<int>() + 1;
  }
}

bool GameScene::addBlocks(json& scene) {
  scene["Blocks"] = {};
  int level = scene["Player"]["level"];

  if (level == 1) {
    for (int y = 0; y < 3; ++y) {
      for (int x = 0; x < 5; ++x) {
        addBlock(x, y, 50, 20, 5, 3,
        {
          { "color", { 0, 0, 1 } },
          { "type", "normal" }
        });
      }
    }
  } else if (level == 2) {
    for (int y = 0; y < 3; ++y) {
      for (int x = 0; x < 5; ++x) {
        if (y == 0) {
          addBlock(x, y, 50, 20, 5, 3,
          {
            { "color", { 1, 1, 0 } },
            { "type", "time-down" }
          });
        } else {
          addBlock(x, y, 50, 20, 5, 3,
          {
            { "color", { 0, 0, 1 } },
            { "type", "normal" }
          });
        }
      }
    }
  } else if (level == 3) {
    for (int y = 0; y < 3; ++y) {
      for (int x = 0; x < 5; ++x) {
        if (x == 0 || x == 4) {
          addBlock(x, y, 50, 20, 5, 3,
          {
            { "color", { 1, 1, 0 } },
            { "type", "time-down" }
          });
        } else {
          addBlock(x, y, 50, 20, 5, 3,
          {
            { "color", { 0, 0, 1 } },
            { "type", "normal" }
          });
        }
      }
    }
  } else if (level == 4) {
    for (int y = 0; y < 3; ++y) {
      for (int x = 0; x < 5; ++x) {
        if (x % 2 == 0) {
          addBlock(x, y, 50, 20, 5, 3,
          {
            { "color", { 1, 1, 0 } },
            { "type", "time-down" }
          });
        } else {
          addBlock(x, y, 50, 20, 5, 3,
          {
            { "color", { 0, 0, 1 } },
            { "type", "normal" }
          });
        }
      }
    }
  } else if (level == 5) {
    for (int y = 0; y < 5; ++y) {
      for (int x = 0; x < 12; ++x) {
        if (y == 2 && x >= 4 && x <= 7) {
          addBlock(x, y, 15, 15, 12, 5,
          {
            { "color", { 1, 1, 1 } },
            { "type", "time-up" }
          });
        } else {
          addBlock(x, y, 15, 15, 12, 5,
          {
            { "color", { 0, 0, 1 } },
            { "type", "normal" }
          });
        }
      }
    }
  } else if (level == 6) {
    for (int y = 0; y < 5; ++y) {
      for (int x = 0; x < 12; ++x) {
        if (y == 0 || y == 4 || x == 0 || x == 11) {
          addBlock(x, y, 15, 15, 12, 5,
          {
            { "color", { 1, 1, 0 } },
            { "type", "time-down" }
          });
        } else if (y == 2 && x >= 4 && x <= 7) {
          addBlock(x, y, 15, 15, 12, 5,
          {
            { "color", { 1, 1, 1 } },
            { "type", "time-up" }
          });
        } else {
          addBlock(x, y, 15, 15, 12, 5,
          {
            { "color", { 0, 0, 1 } },
            { "type", "normal" }
          });
        }
      }
    }
  } else if (level == 7) {
    for (int y = 0; y < 5; ++y) {
      for (int x = 0; x < 12; ++x) {
        if (y == 0 || y == 4 || x == 0 || x == 11) {
          addBlock(x, y, 15, 15, 12, 5,
          {
            { "color", { 0, 0, 1 } },
            { "type", "normal" }
          });
        }
      }
    }
  } else if (level == 8) {
    for (int y = 0; y < 5; ++y) {
      for (int x = 0; x < 12; ++x) {
        if (y == 0 || y == 4 || x == 0 || x == 11) {
          addBlock(x, y, 15, 15, 12, 5,
          {
            { "color", { 0, 0, 1 } },
            { "type", "normal" }
          });
        } else if (y == 1 || y == 3 || x == 1 || x == 10) {
          addBlock(x, y, 15, 15, 12, 5,
          {
            { "color", { 1, 1, 0 } },
            { "type", "time-down" }
          });
        }
      }
    }
  } else if (level == 9) {
    for (int y = 0; y < 5; ++y) {
      for (int x = 0; x < 12; ++x) {
        if (y == 2 && x >= 4 && x <= 7) {
          addBlock(x, y, 15, 15, 12, 5,
          {
            { "color", { 0, 0, 1 } },
            { "type", "normal" }
          });
        } else {
          addBlock(x, y, 15, 15, 12, 5,
          {
            { "color", { 1, 1, 0 } },
            { "type", "time-down" }
          });
        }
      }
    }
  } else if (level == 10) {
    for (int y = 0; y < 5; ++y) {
      for (int x = 0; x < 12; ++x) {
        if (y % 2 == 0) {
          addBlock(x, y, 15, 15, 12, 5,
          {
            { "color", { 1, 1, 0 } },
            { "type", "time-down" }
          });
        } else {
          addBlock(x, y, 15, 15, 12, 5,
          {
            { "color", { 0, 0, 1 } },
            { "type", "normal" }
          });
        }
      }
    }
  }

  return false;
}

bool GameScene::draw(json& scene) {
  ///////////////////////////////////////////////////////////////////////
  // Scene
  {
    int edges = scene["Stage"]["edges"];
    float angle = scene["Stage"]["angle"];
    float radius = scene["Stage"]["radius"];

    glLineWidth(2);
    glColor3f(1, 1, 1);

    DrawCircle(angle, 0, 0, radius, edges);
  }
  ///////////////////////////////////////////////////////////////////////
  // Blocks
  {
    for (auto& [key, value] : scene["Blocks"].items()) {
      float xpos = value["xpos"];
      float ypos = value["ypos"];
      float width = value["width"];
      float height = value["height"];

      auto& color = value["color"];

      glColor3f(color[0], color[1], color[2]);
      DrawFilledRectangle(xpos, ypos, xpos + width, ypos + height);
    }
  }
  ///////////////////////////////////////////////////////////////////////
  // Ball
  {
    float xpos = scene["Ball"]["position"]["xpos"];
    float ypos = scene["Ball"]["position"]["ypos"];

    glColor3f(0, 1, 0);
    DrawFilledCircle(0, xpos, ypos, 10);
  }
  ///////////////////////////////////////////////////////////////////////

  return true;
}

bool GameScene::timer(json& scene) {
  if (isPause) return false;
  if (scene["Player"]["game_status"].get<GameStatus>() != PLAYING) return false;

  static chrono::time_point<chrono::steady_clock> checkTime;
  auto now = chrono::steady_clock::now();
  auto diff = chrono::duration_cast<chrono::microseconds>(now - checkTime);

  if (diff.count() >= 1000000) {
    int playerTime = scene["Player"]["time"];
    scene["Player"]["time"] = playerTime - 1;
    checkTime = now;

    if (playerTime == 6) {
      Audio::Play("countdown");
    }
  }

  return true;
}

bool GameScene::stageKeyboard(json& scene) {
  if (isPause) return false;

  float angle = scene["Stage"]["angle"];
  float speed = scene["Stage"]["speed"];

  if (WindowGetKey(GLFW_KEY_A)) {
    angle += speed;
  } else if (WindowGetKey(GLFW_KEY_D)) {
    angle -= speed;
  } else if (WindowGetKey(GLFW_KEY_Q)) {
    isPause = true;
    ADD_SYSTEM(2, pause);
  }
  
  scene["Stage"]["angle"] = angle;

  return true;
}

bool GameScene::ballMoving(json& scene) {
  if (isPause) return false;
  if (scene["Player"]["game_status"].get<GameStatus>() == GAME_OVER) return false;

  float xpos = scene["Ball"]["position"]["xpos"];
  float ypos = scene["Ball"]["position"]["ypos"];

  float xvel = scene["Ball"]["velocity"]["xpos"];
  float yvel = scene["Ball"]["velocity"]["ypos"];

  float speed = scene["Ball"]["speed"];

  glm::vec2 unit = UnitVector(xvel, yvel);
  xpos += unit.x * speed;
  ypos += unit.y * speed;

  scene["Ball"]["position"]["xpos"] = xpos;
  scene["Ball"]["position"]["ypos"] = ypos;

  return true;
}

bool GameScene::ballCollision(float x1, float y1, float x2, float y2) {
  if (isPause) return false;

  float xpos = scene["Ball"]["position"]["xpos"];
  float ypos = scene["Ball"]["position"]["ypos"];

  float xvel = scene["Ball"]["velocity"]["xpos"];
  float yvel = scene["Ball"]["velocity"]["ypos"];

  float xline = x2 - x1;
  float yline = y2 - y1;

  float ballRadius = 10;
  double distance = DistanceLineToPoint(xpos, ypos, x1, y1, x2, y2);
  glm::vec2 point = PointOnLine(xpos, ypos, x1, y1, x2, y2);
  double innerCheck = DistancePointToPoint(point.x, point.y, x1, y1) + DistancePointToPoint(point.x, point.y, x2, y2);
  double lineWidth = DistancePointToPoint(x1, y1, x2, y2);

  // glPointSize(10);
  // glColor3f(1, 0, 0);
  // glBegin(GL_POINTS);
  // glVertex2f(point.x, point.y);
  // glEnd();

  // glColor3f(1, 1, 1);
  // glRasterPos2i(point.x, point.y);
  // font->FaceSize(30);
  // font->Render(Format("%f <= %f", innerCheck, lineWidth).c_str());

  if (distance <= ballRadius && innerCheck <= lineWidth + 1) {
    // glLineWidth(10);
    // glColor3f(1, 0, 0);
    // glBegin(GL_LINES);
    // glVertex2f(x1, y1);
    // glVertex2f(x2, y2);
    // glEnd();

    glm::vec2 unit = UnitVector(xvel, yvel);
    float radiusDiff = ballRadius - distance + 5;
    xpos -= unit.x * radiusDiff;
    ypos -= unit.y * radiusDiff;

    scene["Ball"]["position"]["xpos"] = xpos;
    scene["Ball"]["position"]["ypos"] = ypos;
    
    double lineAngle = AngleVectorAndVector(xline, yline, 1, 0);
    double angle = AngleVectorAndVector(xline, yline, xvel, yvel);

    double nextAngle = angle + lineAngle;
    xvel = cos(nextAngle);
    yvel = sin(nextAngle);

    scene["Ball"]["velocity"]["xpos"] = xvel;
    scene["Ball"]["velocity"]["ypos"] = yvel;

    return true;
  }

  return false;
}

bool GameScene::stageCollsion(json& scene) {
  if (isPause) return false;

  int edges = scene["Stage"]["edges"];
  float angle = scene["Stage"]["angle"];
  float radius = scene["Stage"]["radius"];

  for (int edge = 0; edge <= edges; ++edge) {
    glm::vec2 p1 = GetPointCircle(edge, angle, 0, 0, radius, edges);
    glm::vec2 p2 = GetPointCircle(edge + 1, angle, 0, 0, radius, edges);

    if (ballCollision(p1.x, p1.y, p2.x, p2.y)) {
      return true;
    }
  }

  return true;
}

bool GameScene::blockCollision(json& scene) { 
  if (isPause) return false;

  for (auto& [key, value] : scene["Blocks"].items()) {
    float x = value["xpos"];
    float y = value["ypos"];
    float width = value["width"];
    float height = value["height"];

    bool result = false;
    result |= ballCollision(x, y + height, x + width, y + height);
    result |= ballCollision(x, y, x + width, y);
    result |= ballCollision(x, y, x, y + height);
    result |= ballCollision(x + width, y, x + width, y + height);

    if (result) {
      string type = value["type"];

      if (type == "normal") {
        Audio::Play("block-broken", 1);
        scene["Player"]["score"] = scene["Player"]["score"].get<int>() + 1;
        scene["Player"]["block_count"] = scene["Player"]["block_count"].get<int>() - 1;

      } else if (type == "time-up") {
        Audio::Play("item-time-up", 1);

        int time = scene["Player"]["time"];
        scene["Player"]["time"] = time + 10;

      } else if (type == "time-down") {
        Audio::Play("item-time-down", 1);

        int time = scene["Player"]["time"];
        scene["Player"]["time"] = time - 10;

      } else if (type == "item-add-blocks") {
        Audio::Play("item-add-blocks", 1);

        // ADD_SYSTEM(10, addBlocks);
      }

      scene["Blocks"].erase(key);

      return true;
    }
  }

  return true;
}

bool GameScene::screenCollision(json& scene) {
  if (isPause) return false;

  float xpos = scene["Ball"]["position"]["xpos"];
  float ypos = scene["Ball"]["position"]["ypos"];

  if ((xpos < screen.left) || (xpos > screen.right) || (ypos < screen.bottom) || (ypos > screen.top)) {
    scene["Ball"]["position"]["xpos"] = 0;
    scene["Ball"]["position"]["ypos"] = 0;

    Audio::Play("block-error", 1);
  }

  return true;
}

bool GameScene::ui(json& scene) {
  int level = scene["Player"]["level"];
  int time = scene["Player"]["time"];
  int blockCount = scene["Player"]["block_count"];

  font->FaceSize(50);
  TextBox textBox1(Format("LEVEL: %03d / TIMER: %05d / BLOCKS: %05d", level, time, blockCount), font);
  textBox1.Box = { screen.left + 10, screen.top - 10, screen.right - 10, 30 };
  textBox1.Draw();

  return true;
}

bool GameScene::gameOverCheck(json& scene) {
  if (isPause) return false;

  size_t blockCount = scene["Player"]["block_count"];
  int time = scene["Player"]["time"];

  if (blockCount <= 0) {
    scene["Player"]["game_status"] = GAME_CLEAR;

    Audio::Stop("countdown");
    Audio::Stop("game-background");
    Audio::Play("game-clear", 1);
    ADD_SYSTEM(3, stageAnimation);
    ADD_SYSTEM(3, drawGameClear);

    return false;

  } else if (time <= 0) {
    scene["Player"]["game_status"] = GAME_OVER;

    Audio::Stop("countdown");
    Audio::Play("game-over", 1);
    ADD_SYSTEM(3, drawGameOver);

    return false;
  }

  return true;
}

bool GameScene::gameStartCounter(json& scene) {
  if (isPause) return false;

  int level = scene["Player"]["level"];
  int count = scene["StartCounter"]["count"];
  int frame = scene["StartCounter"]["frame"];

  font->FaceSize(100);
  TextBox textBox1(Format("%03d", count), font);
  textBox1.Box = { -50, 10, 100, 20 };
  textBox1.Layout.SetAlignment(FTGL::ALIGN_CENTER);
  textBox1.Draw();

  font->FaceSize(48);
  TextBox textBox2(Format("Lv. %02d", level), font);
  textBox2.Box = { -50, 25, 100, 20 };
  textBox2.Layout.SetAlignment(FTGL::ALIGN_CENTER);
  textBox2.Draw();

  if (count <= 0) {
    addGameSystems();

    Audio::Play("game-background");
    return false;

  } else if (frame <= 0) {
    --count;
    frame = 100;

    if (count > 0) Audio::Play("game-count");

  } else {
    --frame;
  }

  scene["StartCounter"]["count"] = count;
  scene["StartCounter"]["frame"] = frame;

  return true;
}

bool GameScene::pause(json& scene) {
  if (!isPause) return false;

  pauseMenu.Draw();
  pauseMenu.KeyboardControlSystem();

  return true;
}

bool GameScene::stageAnimation(json& scene) {
  static float rects[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

  if (rects[3] >= 4.0f) {
    for (int i = 0; i < 4; ++i) rects[i] = 1.0f;
    return false;

  } else {
    float edges = scene["Stage"]["edges"];
    float angle = scene["Stage"]["angle"];
    float radius = scene["Stage"]["radius"];

    for (int i = 0; i < 4; ++i) {
      rects[i] += 0.001f * (i + 1);
      
      glColor3f(1, 1, 1);
      DrawCircle(angle, 0, 0, radius * rects[i], edges);
    }

    return true;
  }
}

bool GameScene::drawGameClear(json& scene) {
  if (isPause) return false;

  font->FaceSize(70);
  TextBox textbox1("GAME CLEAR", font);
  textbox1.Box = { -200, 20, 400, 20 };
  textbox1.Layout.SetAlignment(FTGL::ALIGN_CENTER);
  textbox1.Draw();

  font->FaceSize(50);
  TextBox textbox2("Press 'SPACE' key to next...", font);
  textbox2.Box = { -200, -10, 400, 20 };
  textbox2.FontColor = { 0, 1, 0 };
  textbox2.Layout.SetAlignment(FTGL::ALIGN_CENTER);
  textbox2.Draw();

  if (WindowGetKey(GLFW_KEY_SPACE, 150000)) {
    nextLevel();
  }

  return true;
}

bool GameScene::drawGameOver(json& scene) {
  if (isPause) return false;
  
  font->FaceSize(70);
  TextBox textbox1("GAME OVER", font);
  textbox1.Box = { -200, 20, 400, 20 };
  textbox1.Layout.SetAlignment(FTGL::ALIGN_CENTER);
  textbox1.Draw();

  font->FaceSize(50);
  TextBox textbox2("Press 'SPACE' key to retry...", font);
  textbox2.Box = { -200, -10, 400, 20 };
  textbox2.FontColor = { 1, 0, 0 };
  textbox2.Layout.SetAlignment(FTGL::ALIGN_CENTER);
  textbox2.Draw();

  if (WindowGetKey(GLFW_KEY_SPACE, 150000)) {
    retryLevel();
  }

  return true;
}