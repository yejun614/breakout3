#ifndef SCENE_HPP
#define SCENE_HPP

#include <map>
#include <queue>
#include <vector>
using namespace std;

#include <nlohmann/json.hpp>
using namespace nlohmann;

#include <engine/core/logger.hpp>
#include <engine/core/system.hpp>

#define ADD_SYSTEM(P, F) (systems.push(System((P), [this](json& scene){ return this->F(scene); })))
#define ADD_LAMBDA_SYSTEM(P, F) (systems.push(System((P), [this](json& scene)F)))

struct ScreenProperties {
  float left;
  float right;
  float bottom;
  float top;
  float zNear;
  float zFar;
};

class Scene {
public:
  Scene(string key);
  void RunSystems();
  string SceneKey;

protected:
  json scene;
  priority_queue<System> systems;
  int screenScale;
  ScreenProperties screen;

  bool reshape(json& scene);
  bool fps(json& scene);
};

#endif // SCENE_HPP