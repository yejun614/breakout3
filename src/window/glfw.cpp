#include "glfw.hpp"

#include <engine/engine.hpp>

GLFWwindow* window;

void WindowInit(int width, int height, const char* title) {
  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    exit(EXIT_FAILURE);
  }

  window = glfwCreateWindow(width, height, title, NULL, NULL);
  if (!window) {
    fprintf(stderr, "Failed to open GLFW window\n");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
}

void WindowMainLoop(std::function<void()> display) {
  while (!glfwWindowShouldClose(window)) {
    display();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  
  glfwTerminate();
}

bool WindowGetKey(int key, int continueDelay) {
  static int prevKey = GLFW_KEY_UNKNOWN;
  static chrono::time_point<chrono::steady_clock> checkTime;
  auto now = chrono::steady_clock::now();
  auto diff = chrono::duration_cast<chrono::microseconds>(now - checkTime);

  if (diff.count() < continueDelay && prevKey == key) {
    return false;

  } else if (glfwGetKey(window, key) == GLFW_PRESS) {
    prevKey = key;
    checkTime = now;

    return true;

  } else {
    return false;
  }
}

tuple<int, int> WindowGetMousePos() {
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);

  return { xpos, ypos };
}

bool WindowGetMouseButton(int button) {
  return glfwGetMouseButton(window, button) == GLFW_PRESS;
}

tuple<int, int> WindowGetSize() {
  int width, height;
  glfwGetWindowSize(window, &width, &height);

  return { width, height };
}

void WindowSetSize(int width, int height) {
  glfwSetWindowSize(window, width, height);
}
