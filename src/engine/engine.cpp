#include "engine.hpp"

map<string, Scene*> Engine::Scenes;
string Engine::SceneKey;

set<int> Engine::ContinuousKeyBlock;

Engine::Engine() {
  Log("Engine", "Start Game Engine (v0.1 beta)");

  Scenes["default"] = new Scene("default");
  SceneKey = "default";
}

Engine::~Engine() {
  Log("Engine", "Exit Game Engine");
}

void Engine::InitWindow() {
  WindowInit(1080, 720, "Breakout Game");
}

void Engine::MainLoop() {
  WindowMainLoop([this]() {display();});
}

void Engine::display() {
  static string prevSceneKey = "no scene";

  if (SceneKey != prevSceneKey) {
    Log(
      "Engine::display", 
      Format(
          "Change scene id (%s -> %s)",
          prevSceneKey.c_str(),
          SceneKey.c_str()
        )
    );
    prevSceneKey = SceneKey;
  }

  Scenes[SceneKey]->RunSystems();
}