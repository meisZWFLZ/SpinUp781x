#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#include <cmath>

namespace Conversions {
namespace Degrees {
double toRadians(const double Degrees);
}
namespace Radians {
double toDegrees(const double Radians);
}
namespace EncoderRadians {
double toInches(const double encoderRadians);
}
namespace EncoderInches {
double toRadians(const double encoderInches);
}
} // namespace Conversions
#endif