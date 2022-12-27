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
    static constexpr float driveWheelRadius = 2.75 / 2;
    static constexpr double driveGearRatio =
        3 / 4; // 3 turns of wheel to 4 turns of rotation sensor
    static constexpr float encoderWheelRadius = 2.75 / 2;
  };
  class Encoders {
  public:
    // class EncOrMotor {
    // public:
    //   enum class ENCODER_TYPE { ENCODER, MOTOR, DUAL_MOTOR, ROTATION_SENSOR
    //   }; const ENCODER_TYPE type; // true -> enc, false -> motor motor
    //   *motorPtr; motor *motorPtr2; encoder *encoderPtr; rotation
    //   *rotationPtr;

    //   EncOrMotor(motor *motorPtr);
    //   EncOrMotor(motor *motorPtr, motor *motorPtr2);
    //   EncOrMotor(encoder *encoderPtr);
    //   EncOrMotor()

    //       const double position();
    // };
    class RotationSensor {
    public:
      rotation *rotationPtr;
      RotationSensor(rotation *rotationPtr);
      double position();
    };
    enum ENCODER { LEFT, RIGHT, BACK };

    static std::vector<Robot::Encoders::RotationSensor> encoders;
    static const std::vector<double> distanceToTrackingCenter;
  };
  static inertial inertialSensor;
  class Drivetrain {
  public:
    static motor_group *Left;
    static motor_group *Right;
    static const void left(const float pct);
    static const void right(const float pct);
  };
  enum class GOAL : int { MY_TEAM, RED, BLUE };
  enum class TEAM : bool { RED, BLUE } TEAM;
  static enum TEAM team;
  class Catapult {
  public:
    static const void retract();
    static const void release();
  };
  // enum class PTO_STATE : bool { DRIVE, INTAKE };
  // static enum PTO_STATE PTOState;
  enum class CATAPULT_STATE : bool { UP, DOWN };
  class Actions {
  public:
    // static const void keepCataDown();
    static const void shoot(const Robot::GOAL);
    static const void intake();
    static const void outtake();
    static const void stopIntake();
    static const void roller(); // to do
    static const void expand(); // to do
    // static const void pto(const Robot::PTO_STATE state);
    static const void goTo(const Position pos); // to do
  };
  static const Position getPosition();
  static const CATAPULT_STATE catapultState();
};

#endif