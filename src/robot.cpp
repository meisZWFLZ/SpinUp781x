#include "robot.h"

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

const std::vector<double> Robot::Encoders::distanceToTrackingCenter = {
    Sr, Sl, Ss};

// void Robot::print(){};

// 2.375 vert
// 4.375