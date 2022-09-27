#ifndef POSITION_H
#define POSITION_H

class Position {
public:
  double x; // inches
  double y; // inches
  double heading;

  Position(const double x, const double y, const double heading);
  Position();

  static double distance(const Position &pos1, const Position &pos2);

  static Position difference(const Position &pos1 /* Minuend  */,
                             const Position &pos2 /* Subtrahend */);
  static Position sum(const Position &pos1, const Position &pos2);
  void goTo(); // move robot to position
};
#endif