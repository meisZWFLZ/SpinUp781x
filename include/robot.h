#ifndef ROBOT_H
#define ROBOT_H
#include "vex.h"
#include <vector>

// namespace Robot {
//   extern void print();
//   namespace Encoders {
//     enum ENCODER { LEFT, RIGHT, BACK };

//     extern std::vector<encoder> encoders;
//     extern const std::vector<double> distanceToTrackingCenter;
//   };
//   extern inertial inertialSensor;
//   namespace Dimensions {
//     extern const double wheelRadius;
//   };
// };

class Robot {
public:
  static void print();
  class Encoders {
  public:
    enum ENCODER { LEFT, /* RIGHT, */ BACK };

    static std::vector<encoder> encoders;
    static const std::vector<double> distanceToTrackingCenter;
  };
  static inertial inertialSensor;
  class Dimensions {
  public:
    static const double wheelRadius;
  };
};

#endif