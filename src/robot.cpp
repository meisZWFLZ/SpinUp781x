#include "robot.h"
#include "coordinate.h"

enum Robot::PTO_STATE Robot::PTOState = {Robot::PTO_STATE::INTAKE};
enum Robot::TEAM Robot::team = Robot::TEAM::RED;

inertial Robot::inertialSensor = Inertial10;
Robot::Encoders::EncOrMotor::EncOrMotor(motor *motorPtr)
    : motorPtr(motorPtr), encoderPtr(nullptr), motorPtr2(nullptr),
      type(Robot::Encoders::EncOrMotor::EncOrMotor::ENCODER_TYPE::MOTOR){};
Robot::Encoders::EncOrMotor::EncOrMotor(motor *motorPtr, motor *motorPtr2)
    : motorPtr(motorPtr), encoderPtr(nullptr), motorPtr2(motorPtr2),
      type(Robot::Encoders::EncOrMotor::EncOrMotor::ENCODER_TYPE::DUAL_MOTOR){};
Robot::Encoders::EncOrMotor::EncOrMotor(encoder *encoderPtr)
    : motorPtr(nullptr), encoderPtr(encoderPtr), motorPtr2(nullptr),
      type(Robot::Encoders::EncOrMotor::EncOrMotor::ENCODER_TYPE::ENCODER){};

const double Robot::Encoders::EncOrMotor::position() {
  switch (type) {
  case Robot::Encoders::EncOrMotor::EncOrMotor::ENCODER_TYPE::ENCODER:
    return encoderPtr->position(deg);
  case Robot::Encoders::EncOrMotor::EncOrMotor::ENCODER_TYPE::MOTOR:
    return motorPtr->position(deg);
  case Robot::Encoders::EncOrMotor::EncOrMotor::ENCODER_TYPE::DUAL_MOTOR:
    return (motorPtr->position(deg) + motorPtr2->position(deg)) / 2;
  }
  return 0;
};

std::vector<Robot::Encoders::EncOrMotor> Robot::Encoders::encoders = {
    {&VertEncoder}, // right
    {&VertEncoder}, // left
    {&HoriEncoder}  // back
};
// const double Robot::Dimensions::wheelRadius = 2.75;

constexpr double Ss = 2.375; // back
constexpr double Sl = 4.375; // right
constexpr double Sr = 4.375; // left

const std::vector<double> Robot::Encoders::distanceToTrackingCenter = {Sr, Sl,
                                                                       Ss};

// void Robot::print(){};

// 2.375 vert
// 4.375
const void Robot::Catapult::release() {
  Catapult1.spin(reverse, 100, pct);
  // Catapult1.spinFor(reverse, , deg);
  while (!CatapultLimitSwitch.pressing() && !Controller1.ButtonY.pressing()) {
    // printf("limit: %ld", CatapultLimitSwitch.pressing());
    wait(5, msec);
  }
  while (CatapultLimitSwitch.pressing() && !Controller1.ButtonY.pressing()) {
    // printf("limit: %ld", CatapultLimitSwitch.pressing());
    wait(5, msec);
  }
  Catapult1.stop();
}
const void Robot::Catapult::retract() {
  Catapult1.spin(reverse, 100, pct);
  while (!CatapultLimitSwitch.pressing() && !Controller1.ButtonY.pressing()) {
    // printf("limit: %ld", CatapultLimitSwitch.pressing());
    wait(5, msec);
  }
  Catapult1.stop();
}
// bool shooting = true;s
static bool shooting = false;
const void Robot::Actions::shoot(const enum Robot::GOAL goal) {
  if (!shooting && !Controller1.ButtonY.pressing()) {
    shooting = true;
    Robot::Actions::stopIntake();
    thread([]() {
      Robot::Catapult::release();
      Robot::Catapult::retract();
      shooting = false;
    });
  }
};
const void keepCataDown(){
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
};
const void Robot::Actions::intake() {
  if (CatapultLimitSwitch.pressing() && !shooting) {
    Robot::Actions::pto(Robot::PTO_STATE::INTAKE);
    PTOLeft.spin(fwd, 100, pct);
    PTORight.spin(fwd, 100, pct);
  }
};
const void Robot::Actions::outtake() {
  Robot::Actions::pto(Robot::PTO_STATE::INTAKE);
  PTOLeft.spin(reverse, 30, pct);
  PTORight.spin(reverse, 30, pct);
};
const void Robot::Actions::stopIntake() {
  Robot::Actions::pto(Robot::PTO_STATE::INTAKE);
  PTOLeft.stop();
  PTORight.stop();
};

const void Robot::Actions::expand() { ExpansionPiston.set(true); };
const void Robot::Actions::pto(const Robot::PTO_STATE state) {
  if (Robot::PTOState == state)
    return;
  PTOLeft.stop();
  PTORight.stop();
  PTOPiston.set((bool)state);
  Robot::PTOState = state;
  if (state == Robot::PTO_STATE::INTAKE) {
    PTOLeft.setStopping(hold);
    PTORight.setStopping(hold);
  } else {
    PTOLeft.setStopping(coast);
    PTORight.setStopping(coast);
  }
};
const void Robot::Drivetrain::left(const float pct) {
  std::vector<motor> motors;
  if (Robot::PTOState == Robot::PTO_STATE::INTAKE)
    motors = {LeftDriveA, LeftDriveB}; // intake
  else
    motors = {LeftDriveA, LeftDriveB, PTOLeft}; // drivetrain
  for (auto i = motors.begin(); i != motors.end(); ++i)
    i->spin(fwd, pct, percent);
};
const void Robot::Drivetrain::right(const float pct) {
  std::vector<motor> motors;
  if (Robot::PTOState == Robot::PTO_STATE::INTAKE)
    motors = {RightDriveA, RightDriveB}; // intake
  else
    motors = {RightDriveA, RightDriveB, PTORight}; // drivetrain
  for (auto i = motors.begin(); i != motors.end(); ++i)
    i->spin(fwd, pct, percent);
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Vision Sensor Roller Start
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace roller {

enum class ROLLER : int { RED, BLUE, IN_BETWEEN } ROLLER;

static const bool operator==(const enum Robot::TEAM team1,
                             const enum ROLLER roller1) {
  return (int)team1 == (int)roller1;
};
static const bool operator==(const enum ROLLER roller1,
                             const enum Robot::TEAM team1) {
  return (int)team1 == (int)roller1;
};

struct RollerArea {
public:
  int red;
  int blue;
  Coordinate redC;
  Coordinate blueC;
  void update();
  RollerArea();
};

std::atomic<bool> spinningRoller = {false};
RollerArea rollArea = {};

RollerArea::RollerArea() : red(0), blue(0), redC(0, 0), blueC(0, 0) {}
void RollerArea::update() {
  VisionSensor.takeSnapshot(VisionSensor__RED_ROLLER);
  vision::object redObj = VisionSensor.largestObject;
  red = redObj.width * redObj.height;
  redC = {static_cast<double>(redObj.centerX),
          static_cast<double>(redObj.centerY)};
  VisionSensor.takeSnapshot(VisionSensor__BLUE_ROLLER);
  vision::object blueObj = VisionSensor.largestObject;
  blue = blueObj.width * blueObj.height;
  blueC = {static_cast<double>(blueObj.centerX),
           static_cast<double>(blueObj.centerY)};

  printf("updating: \n r(%f,%f) \nr.%d \n b(%f,%f) \nb.%d", redC.x, redC.y, red,
         blueC.x, blueC.y, blue);
};

enum ROLLER whatIsRoller() {
  rollArea.update();
  if (rollArea.red > 200 && rollArea.blue > 200 &&
      rollArea.redC.y < rollArea.blueC.y)
    return ROLLER::RED;
  if (rollArea.red > 200 && rollArea.blue > 200 &&
      rollArea.redC.y > rollArea.blueC.y)
    return ROLLER::BLUE;
  return ROLLER::IN_BETWEEN;
};

void visionAidedRoller() {
  if (whatIsRoller() == Robot::team)
    return;
  Robot::Actions::pto(Robot::PTO_STATE::INTAKE);
  spinningRoller = true;
  PTOLeft.spin(fwd, 25, pct);
  PTORight.spin(fwd, 25, pct);
  while (!(whatIsRoller() == Robot::team)) {
    if (Controller1.ButtonY.pressing())
      break;
    printf("%f", whatIsRoller());
    printf("vision roller\n");
    wait(10, msec);
  }
  wait(50, msec);
  // if (!(whatIsRoller() == Robot::team))
  //   visionAidedRoller();
  spinningRoller = false;
  PTOLeft.stop();
  PTORight.stop();
};

} // namespace roller
const void Robot::Actions::roller() { roller::visionAidedRoller(); };
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Vision Sensor Roller End
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~