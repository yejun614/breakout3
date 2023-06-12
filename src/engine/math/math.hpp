#ifndef MATH_HPP
#define MATH_HPP

#include <cmath>
#include <glm/vec2.hpp>

#define POW(x) ((x)*(x))
#define MIN(x, y) ((x) < (y) ? (x) : (y));

double DistancePointToPoint(float x1, float y1, float x2, float y2);

double Magnitude(float x, float y);

double DistanceLineToPoint(float x0, float y0, float x1, float y1, float x2, float y2);

glm::vec2 UnitVector(float x, float y);

double AngleVectorAndVector(float x1, float y1, float x2, float y2);

double ZDirVectorAndVector(float x1, float y1, float x2, float y2);

glm::vec2 PointOnLine(float x0, float y0, float x1, float y1, float x2, float y2);

#endif // MATH_HPP