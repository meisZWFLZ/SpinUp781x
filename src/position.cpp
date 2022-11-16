#include "position.h"
#include <cmath>

Position::Position(const double x, const double y, const double heading)
    : Coordinate(x, y), heading(fmod(heading, 360)){};
Position::Position() : Coordinate(0, 0), heading(0){};
Position::Position(const Coordinate coord) : Coordinate(coord), heading(0){};

Position Position::difference(const Position &pos1, const Position &pos2) {
  return {pos1.x - pos2.x, pos1.y - pos2.y, pos1.heading - pos2.heading};
};
Position Position::sum(const Position &pos1, const Position &pos2) {
  return {pos1.x + pos2.x, pos1.y + pos2.y, pos1.heading + pos2.heading};
}
void Position::goTo(){
    // move robot to position
};