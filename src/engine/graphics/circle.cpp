#include "circle.hpp"

glm::vec2 GetPointCircle(float index, float angle, float x, float y, float r, float edges) {
  float step = (2 * M_PI) / edges;
  float i = angle + (step * index);

  return glm::vec2(
    x + cosf(i) * r,
    y + sinf(i) * r
  );
}

void DrawCircle(float angle, float x, float y, float r, float edges) {
  float step = (2 * M_PI) / edges;

  glBegin(GL_LINE_LOOP);

  for (float i = angle; i < angle + (2 * M_PI); i += step) {
    glVertex2f(
      x + cosf(i) * r,
      y + sinf(i) * r
    );
  }

  glEnd();
}

void DrawFilledCircle(float angle, float x, float y, float r, float edges) {
  for (float radius = r; radius >= 0; radius -= 0.1) {
    DrawCircle(angle, x, y, radius, edges);
  }
}