cmake_minimum_required(VERSION 3.15)

project(breakout_game)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Freetype REQUIRED)
find_package(GLM REQUIRED)

include_directories(src)

add_subdirectory(lib/ftgl)
add_subdirectory(lib/json)

add_executable(game
  src/main.cpp
  src/engine/engine.cpp
  src/engine/audio/miniaudio.cpp
  src/engine/audio/audio.cpp
  src/engine/core/logger.cpp
  src/engine/core/scene.cpp
  src/engine/graphics/circle.cpp
  src/engine/graphics/rectangle.cpp
  src/engine/image/stb.cpp
  src/engine/image/image.cpp
  src/engine/math/math.cpp
  src/engine/widget/widget.cpp
  src/engine/widget/TextBox.cpp
  src/engine/widget/FpsWidget.cpp
  src/engine/widget/ProgressBarWidget.cpp
  src/engine/widget/VerticalMenuWidget.cpp
  src/engine/widget/ScrollWidget.cpp
  src/game/game.cpp
  src/game/scene/IntroScene.cpp
  src/game/scene/GameScene.cpp
  src/game/scene/TitleScene.cpp
  src/game/scene/DeveloperScene.cpp
  src/game/scene/LevelSelectScene.cpp
  src/game/scene/ChallengeScene.cpp
  src/utilities/format.cpp
  src/window/glfw.cpp
)

target_compile_features(game PRIVATE cxx_std_20)
target_compile_options(game PRIVATE -w -g)

target_include_directories(game PRIVATE "lib/miniaudio", "lib/stb")

target_link_libraries(
  game
  PRIVATE
    OpenGL::GL
    glfw
    Freetype::Freetype
    ftgl
    nlohmann_json::nlohmann_json
)
