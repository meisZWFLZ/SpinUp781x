#ifndef ROBOT_H
#define ROBOT_H
#include "position.h"
#include "vex.h"
#include <atomic>
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
  class Drivetrain {
  public:
    static const void left(const float pct);
    static const void right(const float pct);
  };
  enum class GOAL : int { MY_TEAM, RED, BLUE };
  enum class TEAM : bool { RED, BLUE } TEAM;
  class Catapult {
  public:
    static const void retract();
    static const void release();
  };
  enum class PTO_STATE : bool { DRIVE, INTAKE };
  static enum PTO_STATE PTOState;
  class Actions {
  public:
    static const void shoot(const Robot::GOAL);
    static const void intake();
    static const void outtake();
    static const void stopIntake();
    static const void roller(); // to do
    static const void expand(); // to do
    static const void pto(const Robot::PTO_STATE state);
    static const void goTo(const Position pos); // to do
  };
  static const Position getPosition();
};
#include "odom/tracking.h"

#endif