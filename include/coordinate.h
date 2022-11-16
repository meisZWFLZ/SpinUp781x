#ifndef COORDINATE_H
#define COORDINATE_H

struct Coordinate {
  float x;
  float y;

  static const double distance(const Coordinate &pos1, const Coordinate &pos2);
  static const double distance(const Coordinate &pos1);

  static Coordinate difference(const Coordinate &pos1 /* Minuend  */,
                               const Coordinate &pos2 /* Subtrahend */);

  static Coordinate sum(const Coordinate &pos1, const Coordinate &pos2);

  Coordinate(const double x, const double y) : x(x), y(y){};
};

#endif