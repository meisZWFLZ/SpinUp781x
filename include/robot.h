#ifndef ROBOT_H
#define ROBOT_H
#include "position.h"
#include "vex.h"
#include "vex_motor.h"
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
    static constexpr float driveWheelRadius = 3.25 / 2;
    static constexpr double driveGearRatio = /* 1; */
        2.0F / 3.0F; // 2 turns of wheel to 3 turns of rotation sensor
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

    static std::vector</* Robot::Encoders::RotationSensor */ rotation> encoders;
    static const std::vector<double> distanceToTrackingCenter;
  };
  static inertial *inertialSensor;
  class Drivetrain {
  public:
    static motor_group *Left;
    static motor_group *Right;
    static const void left(const float pct);
    static const void right(const float pct);
    static void setStopping(vex::brakeType mode);
    static void stop();
  };
  enum class GOAL : int { MY_TEAM, RED, BLUE };
  enum class TEAM : bool { RED, BLUE } TEAM;
  static enum TEAM team;
  class Catapult {
  public:
    static const void retract();
    static const void release(bool boost = false);
    // static const void AngleRelease();
  };
  class PistonBoost {
  public:
    static void boost();
    static void unBoost();
  };

  // class DiscLock {
  // public:
  //   static const void lock();
  //   static const void unlock();
  // };

  // enum class PTO_STATE : bool { DRIVE, INTAKE };
  // static enum PTO_STATE PTOState;
  enum class CATAPULT_STATE : bool { UP, DOWN };
  class Actions {
  public:
    // static const void keepCataDown();
    static const void shoot(const Robot::GOAL);
    static const void pistonShoot(const Robot::GOAL);
    static void intake();
    static const void outtake();
    static void stopIntake();
    static void roller();
    static const void expand();
    /** @param marginOfError in inches */
    static const void goTo(const Position pos, const float marginOfError,
                           const float maxsped = 0.5);
    /** @param marginOfError in inches */
    static const void goTo(const Coordinate coord, const float marginOfError,
                           const float maxsped = 0.5, const float someVal = 30);
    /** @param marginOfError in radians */
    static const void turnTo(const float heading,
                             const float marginOfError = 0.1);
  };
  class InputListeners {
  public:
    static void leftDrive(int);
    static void rightDrive(int);
    static void shoot();
    static void roller();
    static void intake();
    static void outtake();
    static void expand();
  };
  static const Position getPosition();
  static void setPosition(const Position);
  static const CATAPULT_STATE catapultState();
};

#endif