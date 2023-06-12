#ifndef CHALLENGE_SCENE_HPP
#define CHALLENGE_SCENE_HPP

#include "GameScene.hpp"

class ChallengeScene : public GameScene {
public:
  ChallengeScene(string key);

private:
  bool reset(json& scene);
  bool ui(json& sceen);
  bool addMoreBlocks(json& scene);
  bool timeover(json& scene);
};

#endif // CHALLENGE_SCENE_HPP