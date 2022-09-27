#include "conversions.h"
#include "robot.h"

#include <cmath>
#define _USE_MATH_DEFINES
// using namespace Conversions;

double Conversions::EncoderRadians::toInches(const double encoderRadians) {
  return encoderRadians * Robot::Dimensions::wheelRadius;
};

double Conversions::EncoderInches::toRadians(const double encoderInches) {
  return encoderInches / Robot::Dimensions::wheelRadius;
};

double Conversions::Degrees::toRadians(const double Degrees) {
  return (Degrees * M_PI) / 180;
};

double Conversions::Radians::toDegrees(const double Radians) {
  return (Radians / M_PI) * 180;
};