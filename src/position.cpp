#include "position.h"
#include <cmath>

Position::Position(double x, double y, double heading)
    : x(x), y(y), heading(fmod(heading, 360)){};
Position::Position() : x(0), y(0), heading(0){};

double Position::distance(const Position &pos1, const Position &pos2) {
  return sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2));
}

Position Position::difference(const Position &pos1, const Position &pos2) {
  return {pos1.x - pos2.x, pos1.y - pos2.y, pos1.heading - pos2.heading};
};
Position Position::sum(const Position &pos1, const Position &pos2) {
  return {pos1.x + pos2.x, pos1.y + pos2.y, pos1.heading + pos2.heading};
}
void Position::goTo(){
    // move robot to position
};