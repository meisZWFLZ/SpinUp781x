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
  class Dimensions {
  public:
    static constexpr float driveWheelRadius = 1.625;
    static constexpr double driveGearRatio = 3 / 5;
    static constexpr float encoderWheelRadius = 1.375;
  };
  class Encoders {
  public:
    class EncOrMotor {
    public:
      enum class ENCODER_TYPE { ENCODER, MOTOR, DUAL_MOTOR };
      const ENCODER_TYPE type; // true -> enc, false -> motor
      motor *motorPtr;
      motor *motorPtr2;
      encoder *encoderPtr;
      
      EncOrMotor(motor *motorPtr);
      EncOrMotor(motor *motorPtr, motor *motorPtr2);
      EncOrMotor(encoder *encoderPtr);

      const double position();
    };
    enum ENCODER { LEFT, RIGHT, BACK };

    static std::vector<EncOrMotor> encoders;
    static const std::vector<double> distanceToTrackingCenter;
  };
  static inertial inertialSensor;
  class Drivetrain {};
};

#endif