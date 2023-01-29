
#ifndef ODOM_TRACKING_H
#define ODOM_TRACKING_H

#include "odom/odomData.h"
#include <thread>

using namespace std;

class OdomTracking {
public:
  std::vector<double> encoderInches;

  OdomData data;
  Position deltaPos1 = {};

  void updateEncoderInches();
  void updateSnapshot(const Position robotPos); // update snapshot

  double findDeltaTheta();
  double findDeltaX(const double thetaDelta);
  double findDeltaY(const double thetaDelta);
  struct coordinate {
    float x;
    float y;

    coordinate(const double x, const double y) : x(x), y(y){};
  };
  coordinate findTravelCoord(const Position &deltaPos);
  Position findDeltaPosition();

  // axis:
  // true -> x
  // false -> y
  double findTravel(const bool axis, const Position &deltaPos);

  double findTravelX(const Position &deltaPos);
  double findTravelY(const Position &deltaPos);

  Position findRobotPosition();
  
public:
  constexpr static const double WAIT_TIME = 20;
  void reset(const Position pos);
  // static void
  // trackingLoop(OdomTracking *); // all operations used to find robot position

  OdomTracking(const Position startPos); // runs tracking loop
  Position getRobotPosition();
};

#endif