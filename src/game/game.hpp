#ifndef GAME_HPP
#define GAME_HPP

#include <glm/vec2.hpp>
#include <engine/engine.hpp>

struct RotateComponent {
  float angle;
  glm::vec2 lastMousePos;
};

class Game : public Engine {
public:
  Game();
};

#endif // GAME_HPP