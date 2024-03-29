#include "robot.h"
#include "coordinate.h"
#include "stdio.h"
#include "vex.h"
#include "vex_rotation.h"
#include "vex_thread.h"
#include "vex_units.h"
#include <cmath>

// enum Robot::PTO_STATE Robot::PTOState = {Robot::PTO_STATE::INTAKE};
enum Robot::TEAM Robot::team = Robot::TEAM::RED;

motor_group *Robot::Drivetrain::Left = nullptr;
motor_group *Robot::Drivetrain::Right = nullptr;
inertial *Robot::inertialSensor = &Inertial10;

// Robot::Encoders::EncOrMotor::EncOrMotor(motor *motorPtr)
//     : motorPtr(motorPtr), encoderPtr(nullptr), motorPtr2(nullptr),
//       type(Robot::Encoders::EncOrMotor::EncOrMotor::ENCODER_TYPE::MOTOR){};
// Robot::Encoders::EncOrMotor::EncOrMotor(motor *motorPtr, motor *motorPtr2)
//     : motorPtr(motorPtr), encoderPtr(nullptr), motorPtr2(motorPtr2),
//       type(Robot::Encoders::EncOrMotor::EncOrMotor::ENCODER_TYPE::DUAL_MOTOR){};
// Robot::Encoders::EncOrMotor::EncOrMotor(encoder *encoderPtr)
//     : motorPtr(nullptr), encoderPtr(encoderPtr), motorPtr2(nullptr),
//       type(Robot::Encoders::EncOrMotor::EncOrMotor::ENCODER_TYPE::ENCODER){};

// const double Robot::Encoders::EncOrMotor::position() {
//   switch (type) {cat
//   case Robot::Encoders::EncOrMotor::EncOrMotor::ENCODER_TYPE::ENCODER:
//     return encoderPtr->position(deg);
//   case Robot::Encoders::EncOrMotor::EncOrMotor::ENCODER_TYPE::MOTOR:
//     return motorPtr->position(deg);
//   case Robot::Encoders::EncOrMotor::EncOrMotor::ENCODER_TYPE::DUAL_MOTOR:
//     return (motorPtr->position(deg) + motorPtr2->position(deg)) / 2;
//   }
//   return 0;
// };
Robot::Encoders::RotationSensor::RotationSensor(rotation *rotationPtr)
    : rotationPtr(rotationPtr){};
double Robot::Encoders::RotationSensor::position() {
  // printf("nail: %f\n", rotationPtr->position(deg));
  return rotationPtr->position(deg);
};

std::vector</* Robot::Encoders::RotationSensor */ rotation>
    Robot::Encoders::encoders = {LeftDriveR, RightDriveR, HoriR};
// {
// {&RightDriveR}, // right
// {&LeftDriveR},  // left
// {&HoriR}        // back
// };

// const double Robot::Dimensions::wheelRadius = 2.75;

constexpr double Ss = /* 8 */ 6.671062958; // back
constexpr double Sl = 0;                   // right
constexpr double Sr = 0;                   // left

const std::vector<double> Robot::Encoders::distanceToTrackingCenter = {Sl, Sr,
                                                                       Ss};

// void Robot::print(){};

// 2.375 vert
// 4.375
// const void Robot::DiscLock::unlock() { DiscLock1.set(true); }
// const void Robot::DiscLock::lock() { DiscLock1.set(false); }
// const void Robot::Catapult::AngleRelease() {
//   Catapult1.spin(reverse, 100, pct);
//   // Catapult1.spinFor(reverse, , deg);
//   while (!CatapultLimitSwitch.pressing() && !Controller1.ButtonY.pressing())
//   {
//     // printf("limit: %ld", CatapultLimitSwitch.pressing());
//     printf("release 1, limit:%ld\n", CatapultLimitSwitch.pressing());
//     wait(5, msec);
//   }
//   while (CatapultLimitSwitch.pressing() && !Controller1.ButtonY.pressing()) {
//     printf("release 2, limit:%ld\n", CatapultLimitSwitch.pressing());
//     // printf("limit: %ld", CatapultLimitSwitch.pressing());
//     // printf("release 2\n");
//     wait(5, msec);
//   }
//   Catapult1.stop();
//   wait(50, msec);
//   CataAngler.set(true);
//   wait(100, msec);
//   CataAngler.set(false);
// }
void Robot::PistonBoost::boost() { PistonBooster.set(true); }
void Robot::PistonBoost::unBoost() { PistonBooster.set(false); }

const void Robot::Catapult::release(bool boost) {
  Intake.spin(reverse, 12, volt);
  // Robot::DiscLock::unlock();
  // Catapult1.spinFor(reverse, , deg);
  const auto startTime = vex::timer::system();
  while (!CatapultLimitSwitch.pressing() && !Controller1.ButtonY.pressing() &&
         boost && (vex::timer::system() - startTime) < 200) {
    // printf("limit: %ld", CatapultLimitSwitch.pressing());
    // printf("release 1, limit:%ld\n", CatapultLimitSwitch.pressing());
    wait(5, msec);
  }
  Brain.Screen.setCursor(2, 0);
  Brain.Screen.print("boost cata");
  if (boost)
    Robot::PistonBoost::boost();
  while (!CatapultLimitSwitch.pressing() && !Controller1.ButtonY.pressing()) {
    // printf("limit: %ld", CatapultLimitSwitch.pressing());
    // printf("release 1, limit:%ld\n", CatapultLimitSwitch.pressing());
    wait(5, msec);
  }
  while (CatapultLimitSwitch.pressing() && !Controller1.ButtonY.pressing()) {
    // printf("release 2, limit:%ld\n", CatapultLimitSwitch.pressing());
    // printf("limit: %ld", CatapultLimitSwitch.pressing());
    // printf("release 2\n");
    wait(5, msec);
  }
  Intake.stop();
}
const void Robot::Catapult::retract() {
  // Robot::DiscLock::unlock();
  Intake.spin(reverse, 12, volt);
  // Catapult1.spin(reverse, 100, pct);

  // const auto startTime = vex::timer::system();
  // // wait(150, msec);
  // while (!CatapultLimitSwitch.pressing() && !Controller1.ButtonY.pressing()
  // &&
  //        (vex::timer::system() - startTime) < 50) {
  //   // printf("retract, limit:%ld\n", CatapultLimitSwitch.pressing());
  //   // printf("limit: %ld", CatapultLimitSwitch.pressing());
  //   // printf("retract\n", CatapultLimitSwitch.pressing());
  //   wait(5, msec);
  // }
  Robot::PistonBoost::unBoost();
  Brain.Screen.print("unboost cata");
  while (!CatapultLimitSwitch.pressing() && !Controller1.ButtonY.pressing()) {
    wait(5, msec);
  }
  Intake.stop();
  // Robot::DiscLock::lock();
  Controller1.rumble(".");
}
// bool shooting = true;
static bool shooting = false;
const void Robot::Actions::shoot(const enum Robot::GOAL goal) {
  if (!shooting && !Controller1.ButtonY.pressing()) {
    shooting = true;
    Robot::Actions::stopIntake();
    thread([]() {
      Robot::Catapult::release();
      wait(200, msec);
      Robot::Catapult::retract();
      shooting = false;
    });
    // Controller1.rumble("-");
  }
};

const void Robot::Actions::pistonShoot(const enum Robot::GOAL goal) {
  if (!shooting && !Controller1.ButtonY.pressing()) {
    Robot::Actions::stopIntake();
    shooting = true;
    thread([]() {
      // Robot::Catapult::retract();
      // wait(200, msec);
      // Intake.spin(fwd, -12, volt);
      // wait(50, msec);
      // Intake.stop();
      // Robot::Catapult::retract();
      // Robot::PistonBoost::boost();
      printf("^^release\n");
      Robot::Catapult::release(true);
      wait(100, msec);
      Robot::PistonBoost::unBoost();
      wait(100, msec);
      // wait(1000, msec);
      printf("^^retract\n");
      Robot::Catapult::retract();
      shooting = false;
      printf("^^done\n");
    });
  }
};
// const void keepCataDown(){
// static bool running = false;
// if (!running)
//   thread([]() {
//     running = true;
//     while (!Controller1.ButtonLeft.pressing()) {
//       if ()
//       while(!shooting && !CatapultLimitSwitch.pressing()) {
//         Catapult1.spin(reverse, 100, pct);
//       }

//     }
//     Robot::Catapult::retract();
//   });
// };
void Robot::Actions::intake() {
  printf("***shooting:%d\n", shooting);
  if (/* getFailSafe() ||  */ !shooting)
    Intake.spin(fwd, 12, volt);
};
const void Robot::Actions::outtake() { Intake.spin(reverse, 12, volt); };
void Robot::Actions::stopIntake() {
  if (!shooting) {
    // while ((int(std::abs(Intake.position(deg))) % 150) > 0.5) {
    //   wait(10, msec);
    // }
    Intake.stop();
  }
};

const void Robot::Actions::expand() {
  ExpansionPiston.set(true);
  wait(500, msec);
  ExpansionPiston.set(false);

  printf("expand!!\n");
};
// const void Robot::Actions::pto(const Robot::PTO_STATE state) {
//   if (Robot::PTOState == state)
//     return;
//   PTOLeft.stop();
//   PTORight.stop();
//   PTOPiston.set((bool)state);
//   Robot::PTOState = state;
//   if (state == Robot::PTO_STATE::INTAKE) {
//     PTOLeft.setStopping(hold);
//     PTORight.setStopping(hold);
//   } else {
//     PTOLeft.setStopping(coast);
//     PTORight.setStopping(coast);
//   }
// };
const void Robot::Drivetrain::left(const float pct) {
  // printf("%f\n", pct);
  Robot::Drivetrain::Left->spin(fwd, pct * 12, volt);
};
const void Robot::Drivetrain::right(const float pct) {
  // printf("%f\n", pct);
  Robot::Drivetrain::Right->spin(fwd, pct * 12, volt);
  // std::vector<motor> motors;
  // if (Robot::PTOState == Robot::PTO_STATE::INTAKE)
  //   motors = {RightDriveA, RightDriveB}; // intake
  // else
  //   motors = {RightDriveA, RightDriveB, PTORight}; // drivetrain
  // for (auto i = motors.begin(); i != motors.end(); ++i)
  //   i->spin(fwd, pct, percent);
};

void Robot::Drivetrain::stop() {
  Robot::Drivetrain::Left->stop();
  Robot::Drivetrain::Right->stop();
}
void Robot::Drivetrain::setStopping(brakeType mode) {
  Robot::Drivetrain::Left->setStopping(mode);
  Robot::Drivetrain::Right->setStopping(mode);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Vision Sensor Roller Start
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace roller {

enum class ROLLER : int { RED, BLUE, IN_BETWEEN } ROLLER;

static const inline bool operator==(const enum Robot::TEAM team1,
                                    const enum ROLLER roller1) {
  return (int)team1 == (int)roller1;
};
static const inline bool operator==(const enum ROLLER roller1,
                                    const enum Robot::TEAM team1) {
  return (int)team1 == (int)roller1;
};

// struct RollerArea {
// public:
//   int red;
//   int blue;
//   Coordinate redC;
//   Coordinate blueC;
//   void update();
//   RollerArea();
// };

std::atomic<bool> spinningRoller = {false};
// RollerArea rollArea = {};

// RollerArea::RollerArea() : red(0), blue(0), redC(0, 0), blueC(0, 0) {}
// void RollerArea::update() {
//   // VisionSensor.takeSnapshot(VisionSensor__RED_ROLLER);
//   // vision::object redObj = VisionSensor.largestObject;
//   // red = redObj.width * redObj.height;
//   // redC = {static_cast<double>(redObj.centerX),
//   //         static_cast<double>(redObj.centerY)};
//   // VisionSensor.takeSnapshot(VisionSensor__BLUE_ROLLER);
//   // vision::object blueObj = VisionSensor.largestObject;
//   // blue = blueObj.width * blueObj.height;
//   // blueC = {static_cast<double>(blueObj.centerX),
//   //          static_cast<double>(blueObj.centerY)};

//   // printf("updating: \n r(%f,%f) \nr.%d \n b(%f,%f) \nb.%d", redC.x,
//   redC.y, red,
//   //        blueC.x, blueC.y, blue);
// };

enum ROLLER whatIsRoller() {
  // rollArea.update();
  // if (rollArea.red > 200 && rollArea.blue > 200 &&
  //     rollArea.redC.y < rollArea.blueC.y)
  //   return ROLLER::RED;
  // if (rollArea.red > 200 && rollArea.blue > 200 &&
  //     rollArea.redC.y > rollArea.blueC.y)
  //   return ROLLER::BLUE;
  // return ROLLER::IN_BETWEEN;
  auto hue1 = RollerSensor.hue();
  // printf("hue: %f,\nroller: %s\n", RollerSensor.hue(),
  //        hue1 > 325 || hue1 < 10
  //            ? "blue"
  //            : (hue1 > 220 && hue1 < 275 ? "red" : "in_between"));
  return hue1 > 325 || hue1 < 20
             ? ROLLER::BLUE
             : (hue1 > 220 && hue1 < 275 ? ROLLER::RED : ROLLER::IN_BETWEEN);
};
bool amSeeRoller() {
  // // rollArea.update();
  // // if (rollArea.red > 200 && rollArea.blue > 200 &&
  // //     rollArea.redC.y < rollArea.blueC.y)
  // //   return ROLLER::RED;
  // // if (rollArea.red > 200 && rollArea.blue > 200 &&
  // //     rollArea.redC.y > rollArea.blueC.y)
  // //   return ROLLER::BLUE;
  // // return ROLLER::IN_BETWEEN;
  // auto hue1 = RollerSensor.hue();
  // // printf("hue: %f,\nroller: %s\n", RollerSensor.hue(),
  // //        hue1 > 325 || hue1 < 10
  // //            ? "blue"
  // //            : (hue1 > 220 && hue1 < 275 ? "red" : "in_between"));
  // return hue1 > 325 || hue1 < 20
  //            ? ROLLER::BLUE
  //            : (hue1 > 220 && hue1 < 275 ? ROLLER::RED : ROLLER::IN_BETWEEN);
  // return ;
  // printf("rgb: %lu", RollerSensor.color().rgb());
  return RollerSensor
      .isNearObject() /* && whatIsRoller() == ROLLER::IN_BETWEEN */;
};

void visionAidedRoller() {
  // printf("rgb: %lu\n", RollerSensor.color().rgb());
  // return Robot::Actions::stopIntake();
  if (shooting)
    return;

  // Robot::Actions::pto(Robot::PTO_STATE::INTAKE);
  spinningRoller = true;
  if (RollerSensor.installed() && amSeeRoller()) {
    const auto startTime = vex::timer::system();
    // Intake.spin(fwd, 25, pct);
    Intake.spin(fwd, 100, pct);
    // if (whatIsRoller() == Robot::team)
    //   // wait(250, msec);
    //   wait(100, msec);
    while ((whatIsRoller() == Robot::team) && !Controller1.ButtonY.pressing() &&
           (vex::timer::system() - startTime < 2000))
      wait(10, msec);
    while (!(whatIsRoller() == Robot::team) &&
           !Controller1.ButtonY.pressing() &&
           (vex::timer::system() - startTime < 2000))
      wait(10, msec);

    // wait(300, msec);
    // wait(75, msec);
    // RollerSensor.setLightPower(0, pct);
  } else {
    Intake.setVelocity(100, pct);
    Intake.spinFor(fwd, 0.625, turns);
  }
  Intake.stop();
  spinningRoller = false;
};

} // namespace roller
void Robot::Actions::roller() { roller::visionAidedRoller(); };
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Vision Sensor Roller End
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//      Input Listeners Start
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void Robot::InputListeners::leftDrive(int axis) {
  Robot::Drivetrain::left(getDriveCoefficient() * pow((float)axis / 100, 5));
};
void Robot::InputListeners::rightDrive(int axis) {
  Robot::Drivetrain::right(getDriveCoefficient() * pow((float)axis / 100, 5));
};
void Robot::InputListeners::shoot() {
  Robot::Actions::shoot((Robot::GOAL)(int)Robot::team);
};
void Robot::InputListeners::roller() { Robot::Actions::roller(); };

void Robot::InputListeners::intake() {
  if (Intake.voltage(volt) > 0.1)
    Robot::Actions::stopIntake();
  else
    Robot::Actions::intake();
};
void Robot::InputListeners::outtake() {
  if (Intake.voltage(volt) < -0.1)
    Robot::Actions::stopIntake();
  else
    Robot::Actions::outtake();
};
void Robot::InputListeners::expand() {
  // if (!getFailSafe())

  // ensures expansion will fire even if chain breaks / intake motors cannot spin / limit switch cannot be pressed
  thread([] {
    wait(300, msec);
    auto startTime = vex::timer::system();
    while (shooting && startTime < 500)
      wait(15, msec);
    Robot::Actions::expand();
  });
  Robot::Catapult::release();

  // Robot::Actions::expand();
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Input Listeners End
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
