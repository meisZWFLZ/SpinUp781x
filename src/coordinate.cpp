#include "coordinate.h"
#include <cmath>

const double Coordinate::distance(const Coordinate &pos1, const Coordinate &pos2) {
  return sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2));
}
const double Coordinate::distance(const Coordinate &pos1) {
  return sqrt((pos1.x * pos1.x) + (pos1.y * pos1.y));
}

Coordinate Coordinate::difference(const Coordinate &pos1, const Coordinate &pos2) {
  return {pos1.x - pos2.x, pos1.y - pos2.y};
};
Coordinate Coordinate::sum(const Coordinate &pos1, const Coordinate &pos2) {
  return {pos1.x + pos2.x, pos1.y + pos2.y};
}