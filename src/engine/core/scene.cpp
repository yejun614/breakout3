#include "scene.hpp"

#include <glm/vec2.hpp>
#include <window/glfw.hpp>
#include <engine/widget/FpsWidget.hpp>

Scene::Scene(string key) : SceneKey(key) {
  screenScale = 500;

  ADD_SYSTEM(100, reshape);
  ADD_SYSTEM(1, fps);
}

void Scene::RunSystems() {
  queue<System> buffers;

  while (!systems.empty()) {
    System item = systems.top();
    systems.pop();

    if (item.Func(scene)) {
      buffers.push(item);
    }
  }

  while (!buffers.empty()) {
    System item = buffers.front();
    buffers.pop();
    systems.push(item);
  }
}

bool Scene::reshape(json& scene) {
  const auto windowSize = WindowGetSize();
  double width = get<0>(windowSize);
  double height = get<1>(windowSize);

  glDisable(GL_TEXTURE_2D);
  
  glViewport(0, 0, width * 2, height * 2);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  screen.zNear = 1;
  screen.zFar = 0;

  if (width < height) {
    double heightRate = screenScale * (height / width);

    screen.left = -screenScale / 2.0f;
    screen.right = screenScale / 2.0f;
    screen.top = heightRate / 2.0f;
    screen.bottom = -heightRate / 2.0f;
  } else {
    double widthRate = screenScale * (width / height);

    screen.bottom = -screenScale / 2.0f;
    screen.top = screenScale / 2.0f;
    screen.left = -widthRate / 2.0f;
    screen.right = widthRate / 2.0f;
  }

  glOrtho(
    screen.left,
    screen.right,
    screen.bottom,
    screen.top,
    screen.zNear,
    screen.zFar
  );

  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  return true;
}

bool Scene::fps(json& scene) {
  FpsWidgetSystem(screen.left + 10, screen.bottom + 10);

  return true;
}