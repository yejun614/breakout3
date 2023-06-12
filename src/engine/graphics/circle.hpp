#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include <OpenGL/gl.h>
#include <cmath>
#include <glm/vec2.hpp>

glm::vec2 GetPointCircle(float index, float angle, float x, float y, float r, float edges = 36);

void DrawCircle(float angle, float x, float y, float r, float edges = 36);

void DrawFilledCircle(float angle, float x, float y, float r, float edges = 36);

#endif // CIRCLE_HPP