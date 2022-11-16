#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#include "robot.h"
#include <cmath>

namespace Conversions {
namespace Degrees {
inline constexpr const double toRadians(const double Degrees) {
  return (Degrees * M_PI) / 180;
};
} // namespace Degrees
namespace Radians {
inline constexpr const double toDegrees(const double Radians) {
  return (Radians / M_PI) * 180;
};
} // namespace Radians
namespace EncoderRadians {
inline constexpr const double toInches(const double encoderRadians) {
  return encoderRadians * Robot::Dimensions::encoderWheelRadius;
};
} // namespace EncoderRadians
namespace EncoderInches {
inline constexpr const double toRadians(const double encoderInches) {
  return encoderInches / (Robot::Dimensions::encoderWheelRadius);
};
} // namespace EncoderInches
// namespace MotorRadians {
// inline constexpr const double toInches(const double motorRadians) {
//   return motorRadians * Robot::Dimensions::driveWheelRadius *
//          Robot::Dimensions::driveGearRatio;
// };
// } // namespace MotorRadians
// namespace MotorInches {
// inline constexpr const double toRadians(const double motorInches) {
//   return motorInches / (Robot::Dimensions::driveWheelRadius *
//                         Robot::Dimensions::driveGearRatio);
// };
// } // namespace MotorInches
} // namespace Conversions
#endif