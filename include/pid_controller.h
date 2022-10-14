#include <functional>

class PIDController {
public:
  const float kP;
  const float kI;
  const float kD;
  const int interval;

  float error;
  float integral;
  float lastError;
  float adjustment;

  virtual const float getError() const = 0;
  virtual void outputAdjustment(const float adjustment) const = 0;

  void pidLoop();
  void pidSegment();

  PIDController(const float kP, const float kI, const float kD,
                const int intervalInMsec);

  class Callback;
  // class Turn;
};

class PIDController::Callback : public PIDController {
public:
  const std::function<const float(void)> getErrorCallback;
  const std::function<void(const float)> outputAdjustmentCallback;

  const float getError() const override;
  void outputAdjustment(const float adjustment) const override;

  Callback(const float kP, const float kI, const float kD,
           const int intervalInMsec,
           const std::function<const float(void)> getErrorCallback,
           const std::function<void(const float)> outputAdjustmentCallback);
};

// class PIDController::Turn : public PIDController {
// public:
//   const float heading;

//   const float getError() const override;
//   void outputAdjustment(const float adjustment) const override;

//   Turn(const float kP, const float kI, const float kD, const int intervalInMsec,
//        const float heading);
//   Turn(const int intervalInMsec, const float heading);
//   Turn(const float heading);
// };