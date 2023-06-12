#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <set>
#include <map>
#include <queue>
#include <string>
using namespace std;

#include <OpenGL/gl.h>

#include <engine/core/logger.hpp>
#include <engine/core/scene.hpp>
#include <engine/core/system.hpp>
#include <utilities/format.cpp>
#include <window/glfw.hpp>

class Engine {
public:
  static set<int> ContinuousKeyBlock;
  static map<string, Scene*> Scenes;
  static string SceneKey;

  Engine();
  ~Engine();
  void InitWindow();
  void MainLoop();

protected:
  void display();
};

#endif // ENGINE_HPP