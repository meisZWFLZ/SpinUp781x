#ifndef POSITION_H
#define POSITION_H
#include "coordinate.h"

class Position : public Coordinate {
public:
  double heading;

  Position(const double x, const double y, const double heading);
  Position(const Coordinate coord);
  Position();

  static Position difference(const Position &pos1 /* Minuend  */,
                             const Position &pos2 /* Subtrahend */);
  static Position sum(const Position &pos1, const Position &pos2);
  void goTo(); // move robot to position
};
#endif