#include "robot.h"

enum Robot::PTO_STATE Robot::PTOState = {Robot::PTO_STATE::INTAKE};

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
const void Robot::Catapult::release() { Catapult1.spinFor(reverse, 5, deg); }
const void Robot::Catapult::retract() {
  Catapult1.spin(reverse, 10, pct);
  while (!CatapultLimitSwitch.pressing())
    wait(5, msec);
  Catapult1.stop();
}
const void Robot::Actions::shoot(const enum Robot::GOAL goal) {
  static bool readyToShoot = true;
  if (readyToShoot) {
    readyToShoot = false;
    thread([]() {
      Robot::Catapult::release();
      wait(300, msec);
      Robot::Catapult::retract();
      readyToShoot = true;
    });
  }
};
const void Robot::Actions::intake() {
  Robot::Actions::pto(Robot::PTO_STATE::INTAKE);
  PTOLeft.spin(fwd, 100, pct);
  PTORight.spin(fwd, 100, pct);
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