#ifndef GLFW_HPP
#define GLFW_HPP

#include <cstdio>
#include <cstdlib>
#include <functional>
#include <tuple>
#include <set>
using namespace std;

#include <GLFW/glfw3.h>

void WindowInit(int width, int height, const char* title);

void WindowMainLoop(std::function<void()> display);

bool WindowGetKey(int key, int continueDelay = 1);

tuple<int, int> WindowGetMousePos();

bool WindowGetMouseButton(int button);

tuple<int, int> WindowGetSize();

void WindowSetSize(int width, int height);

#endif // GLFW_HPP