#include "math.hpp"

double DistancePointToPoint(float x1, float y1, float x2, float y2) {
  return sqrt(powf(x1 - x2, 2) + powf(y1 - y2, 2));
}

double Magnitude(float x, float y) {
  return DistancePointToPoint(0, 0, x, y);
}

double DistanceLineToPoint(float x0, float y0, float x1, float y1, float x2, float y2) {
  double result = (x2 - x1) * (y1 - y0);
  result -= (x1 - x0) * (y2 - y1);
  result *= (result < 0) ? (-1) : (1);  // ABS
  result /= sqrt(powf(x2 - x1, 2) + powf(y2 - y1, 2));

  return result;
}

glm::vec2 UnitVector(float x, float y) {
  double mag = Magnitude(x, y);
  x /= mag;
  y /= mag;

  return glm::vec2(x, y);
}

double AngleVectorAndVector(float x1, float y1, float x2, float y2) {
  double theta = (x1 * x2) + (y1 * y2);
  theta /= sqrt(POW(x1) + POW(y1)) * sqrt(POW(x2) + POW(y2));
  theta = acos(theta);

  if (ZDirVectorAndVector(x1, y1, x2, y2) > 0) {
    theta *= -1;
  }

  return theta;
}

double ZDirVectorAndVector(float x1, float y1, float x2, float y2) {
  return (x1 * y2) - (y1 * x2);
}

glm::vec2 PointOnLine(float x0, float y0, float x1, float y1, float x2, float y2) {
  float a = (y2 - y1) / (x2 - x1);
  float b = -1;
  float c = -a * x1 + y1;

  float x = b * (b * x0 - a * y0) - (a * c);
  x /= POW(a) + POW(b);

  float y = a * (-b * x0 + a * y0) - (b * c);
  y /= POW(a) + POW(b);

  return glm::vec2(x, y);
}