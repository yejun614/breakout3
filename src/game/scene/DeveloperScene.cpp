#include "DeveloperScene.hpp"

#include <fstream>

#include <engine/image/image.hpp>
#include <engine/widget/TextBox.hpp>
#include <engine/widget/ScrollWidget.hpp>
#include <game/scene/TitleScene.hpp>
#include <window/glfw.hpp>

#include <OpenGL/gl.h>

DeveloperScene::DeveloperScene(string key) : Scene(key) {
  Image::Load("background", "res/img/contract-5338587_1280.jpg");

  ifstream fp("README_kr.md");
  if (!fp.is_open()) {
    Log("'DeveloperScene::DeveloperScene", "ERROR Faild open file");
  } else {
    string line;
    texts.push_back("------------ FILE START ------------");
    while (getline(fp, line)) {
      texts.push_back(line);
    }
    texts.push_back("------------  FILE  END  ------------");
    fp.close();
  }

  font = new FTGLPixmapFont("res/font/D2Coding.ttf");

  ADD_SYSTEM(5, draw);
  ADD_SYSTEM(3, keyboard);
}

bool DeveloperScene::draw(json& scene) {
  ScrollWidget scrollWidget;

  auto windowSize = WindowGetSize();
  float windowWidth = get<0>(windowSize) * 2.0f;
  float windowHeight = get<1>(windowSize) * 2.0f;

  scrollWidget.Box = {
    100,
    100,
    windowWidth - 200,
    windowHeight - 200
  };

  glClearColor(0.2f, 0.2f, 0.2f, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  scrollWidget.Draw([this, windowWidth, windowHeight](BoxCoord innerBox){
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    Image::Draw("background", 0.6f, 0.5f, 0.8f, 0.8f);

    string buf;
    for (int line = 0; line < texts.size(); ++line) {
      buf = texts[line];
      if (buf.size() <= xscroll) {
        buf = "";
      } else {
        buf = buf.substr(xscroll);
      }

      glColor3f(1, 1, 1);
      glRasterPos2f(-0.9f, 0.7f - (0.1f * line) + yscroll);
      font->FaceSize(50);
      font->Render(buf.c_str());
    }
  });

  return true;
}

bool DeveloperScene::keyboard(json& scene) {
  if (WindowGetKey(GLFW_KEY_A, 10000)) {
    xscroll -= 1.0f;
    if (xscroll < 0) xscroll = 0;

  } else if (WindowGetKey(GLFW_KEY_D, 10000)) {
    xscroll += 1.0f;

  } else if (WindowGetKey(GLFW_KEY_W, 10000)) {
    yscroll -= 0.1f;

  } else if (WindowGetKey(GLFW_KEY_S, 10000)) {
    yscroll += 0.1f;

  } else if (WindowGetKey(GLFW_KEY_SPACE, 1000000)) {
    Engine::Scenes["title"] = new TitleScene("title");
    Engine::SceneKey = "title";
  }

  return true;
}