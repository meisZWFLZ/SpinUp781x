#include "pid_controller.h"
#include "vex.h"

PIDController::PIDController(const float kP, const float kI, const float kD,
                             const int intervalInMsec)
    : kP(kP), kI(kI), kD(kD), interval(intervalInMsec), error(0), integral(0),
      lastError(0){};

void PIDController::pidSegment() {
  lastError = error;
  error = getError();
  integral += error;
  outputAdjustment((error * kP) +             // proportional
                   (integral * kI) +          // integral
                   ((error - lastError) * kD) // deriative
  );
};
void PIDController::pidLoop() {
  while (1) {
    pidSegment();
    wait(interval, msec);
  };
};

// callback
PIDController::Callback::Callback(
    const float kP, const float kI, const float kD, const int intervalInMsec,
    const std::function<const float(void)> getErrorCallback,
    const std::function<void(const float)> outputAdjustmentCallback)
    : PIDController(kP, kI, kD, intervalInMsec),
      getErrorCallback(getErrorCallback),
      outputAdjustmentCallback(outputAdjustmentCallback){};
const float PIDController::Callback::getError() const {
  return getErrorCallback();
};
void PIDController::Callback::outputAdjustment(const float adjustment) const {
  outputAdjustmentCallback(adjustment);
};

// // turn
// const float PIDController::Turn::getError() const {
//   const Position vector =
//       Position::difference(tracker->getRobotPosition(), GOAL_POS);
//   const float target1 = atan2(vector.x, vector.y);
//   // const float target = target1 < 0 ? target1 + M_PI : target1;
//   // const float heading1 =
//   //     Conversions::Degrees::toRadians(Inertial10.heading(deg));
//   // const float headingDiff = headingDifference(target, heading1) * 100;
//   // printf("{heading: %f\n+diff: %f\n@target: %f\n", heading1, headingDiff /
//   // 100,
//   //        target);
//   return headingDifference(
//              target1 < 0 ? target1 + M_PI : target1,
//              Conversions::Degrees::toRadians(Inertial10.heading(deg)) F) *
//          100;
// };
// void PIDController::Turn::outputAdjustment(const float adjustment) const {
//   // outputAdjustmentCallback(adjustment);
//   spinRightPID(-adjustment);
//   spinLeftPID(adjustment);
// };
