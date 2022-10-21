// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller
// PTOLeft              motor         2
// PTORight             motor         3
// PTOPiston            digital_out   H
// LeftDriveA           motor         4
// LeftDriveB           motor         5
// RightDriveA          motor         6
// RightDriveB          motor         11
// FlyWheel             motor_group   8, 9
// HoriEncoder          encoder       A, B
// VertEncoder          encoder       C, D
// Inertial10           inertial      1
// VisionSensor         vision        12
// ---- END VEXCODE CONFIGURED DEVICES ----
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller
// PTOLeft              motor         2
// PTORight             motor         3
// PTOPiston            digital_out   H
// LeftDriveA           motor         4
// LeftDriveB           motor         5
// RightDriveA          motor         6
// RightDriveB          motor         11
// FlyWheel             motor_group   8, 9
// HoriEncoder          encoder       A, B
// VertEncoder          encoder       C, D
// Inertial10           inertial      1
// VisionSensor         vision        12
// ---- END VEXCODE CONFIGURED DEVICES ----
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller
// PTOLeft              motor         2
// PTORight             motor         3
// PTOPiston            digital_out   H
// LeftDriveA           motor         4
// LeftDriveB           motor         5
// RightDriveA          motor         6
// RightDriveB          motor         11
// FlyWheel             motor_group   8, 9
// HoriEncoder          encoder       A, B
// VertEncoder          encoder       C, D
// Inertial10           inertial      1
// VisionSensor         vision        12
// ---- END VEXCODE CONFIGURED DEVICES ----
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller
// PTOLeft              motor         2
// PTORight             motor         3
// PTOPiston            digital_out   H
// LeftDriveA           motor         4
// LeftDriveB           motor         5
// RightDriveA          motor         6
// RightDriveB          motor         11
// FlyWheel             motor_group   8, 9
// HoriEncoder          encoder       A, B
// VertEncoder          encoder       C, D
// Inertial10           inertial      1
// VisionSensor         vision        12
// ---- END VEXCODE CONFIGURED DEVICES ----
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller
// PTOLeft              motor         2
// PTORight             motor         3
// PTOPiston            digital_out   H
// LeftDriveA           motor         4
// LeftDriveB           motor         5
// RightDriveA          motor         6
// RightDriveB          motor         11
// FlyWheel             motor_group   8, 9
// HoriEncoder          encoder       A, B
// VertEncoder          encoder       C, D
// Inertial10           inertial      1
// VisionSensor         vision        12
// ---- END VEXCODE CONFIGURED DEVICES ----
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller
// PTOLeft              motor         2
// PTORight             motor         3
// PTOPiston            digital_out   H
// LeftDriveA           motor         4
// LeftDriveB           motor         5
// RightDriveA          motor         6
// RightDriveB          motor         11
// FlyWheel             motor_group   8, 9
// HoriEncoder          encoder       A, B
// VertEncoder          encoder       C, D
// Inertial10           inertial      1
// VisionSensor         vision        12
// ---- END VEXCODE CONFIGURED DEVICES ----
#include "conversions.h"
#include "odom/tracking.h"
#include "pid_controller.h"
#include "position.h"
#include "vex.h"
#include <atomic>
#include <cmath>
#include <iostream>
#include <thread>
#include <vector>

// using namespace std::chrono_literals;

// #ifndef ODOMTRACKING_CPP
// #include "odom/tracking.cpp"
// #endif

/*
- normal drivetrain?
- tank drive control scheme
- automatic shooting :(
- distance in inches
*/

const Position GOAL_POS = {18, 18, 0};

// enum Button {
//   CANCEL_SHOOTING,
//   EXPANSION,
//   SAFTEY_SWITCH /*enables and disables saftey mechanisms*/,
//   SHOOT,
//   ROLLER,
//   INTAKE,
//   UNSTUCK_INTAKE,
//   PTO_SWITCH,
//   FLY_WHEEL_TOGGLE
// };

// controller::button getControllerButton(Button button) {
//   controller::button ControllerButtons[] = {
//       Controller1.ButtonA,  Controller1.ButtonB,    Controller1.ButtonX,
//       Controller1.ButtonY,  Controller1.ButtonDown, Controller1.ButtonR1,
//       Controller1.ButtonR2, Controller1.ButtonLeft, Controller1.ButtonUp};
//   return ControllerButtons[button];
// }

enum Axis { LEFT_DRIVE, RIGHT_DRIVE };

controller::axis getControllerAxis(Axis axis) {
  controller::axis ControllerAxises[] = {Controller1.Axis3, Controller1.Axis2};
  return ControllerAxises[axis];
}

// motor_group PTOGroup = motor_group(PTOLeft, PTORight);

// // const std::chrono::duration<long long, milli> LENGTH_OF_GAME = 12000ms;
// const int LENGTH_OF_GAME = 20000;
// // const std::chrono::duration<long long, milli> LENGTH_OF_ENGAME = 10000ms;
// const int LENGTH_OF_ENGAME = 10000;

// // const std::chrono::steady_clock::time_point TIME_OF_ENGAME =
// // chrono::steady_clock::now() + LENGTH_OF_GAME - LENGTH_OF_ENGAME;
// const int TIME_OF_ENGAME =
//     /* chrono::steady_clock::now() + */ LENGTH_OF_GAME - LENGTH_OF_ENGAME;

// // const std::chrono::duration<long long, milli> DISC_LOAD_DELAY = 500ms;
// // //should be time piston takes to complete full cycle
// const int DISC_LOAD_DELAY =
//     200; // should be time piston takes to complete full cycle

// static const int DISC_LOADER_DISTANCE = 720;

// static bool shooting = false; // disables drivetrain whilst aiming/shooting
// static int shots = 0; // tap shoot button to add more discs after original
// press static bool cancelShooting = false;

// static atomic<bool> shootingYawed;   // wait until yawed to shoot
// static atomic<bool> shootingPitched; // wait until pitched to shoot

// static bool PTOSwitching = false; // Disable PTO motors and drivetrain

static bool flyWheelSpin = false;

// static bool expanded = false;
// static bool saftey = false; // true -> disables saftey mechanisms

static OdomTracking *tracker;

// bool buttonPressed(Button button) {
//   return getControllerButton(button).pressing();
// }

int axisPosition(Axis axis) { return getControllerAxis(axis).position(); }

// void PTOIntake() {
//   if (PTOState)
//     return;
//   Brain.Screen.print("intake");
//   PTOPiston.set(false);
//   // freeze drivetrain and pto motors
//   PTOState = true;
//   PTOSwitching = false;
// }

// void PTODrivetrain() {
//   if (!PTOState)
//     return;
//   Brain.Screen.print("drive");
//   PTOPiston.set(true);
//   // freeze drivetrain and pto motors
//   PTOState = false;
//   PTOSwitching = false;
// }

// void endgameExpansion() {
//   // expansion stuff
// }

// void loadDisc() {
//   PTOGroup.setVelocity(100, pct);
//   PTOGroup.spinFor(reverse, DISC_LOADER_DISTANCE, degrees);
// }

// Position getPosition() { return {}; } // gets robot position

// void turn(double newHeading) {
//   Brain.Screen.setCursor(2, 1);
//   Brain.Screen.clearLine();
//   Brain.Screen.print(newHeading);
//   double hDiff = getPosition().heading - newHeading;
//   spinLeftDrive(hDiff ? fwd : reverse, 10, pct);
//   spinRightDrive(hDiff ? reverse : fwd, 10, pct);
//   while (abs(getPosition().heading - newHeading) > 0.1)
//     wait(10, msec);
// };

// void aimTurret(int distance /*inches*/) { wait(0, msec); }

// void expansionButtonSubscriber() {
//   // prevents expansion untill endgame
//   if (saftey || 432423 > TIME_OF_ENGAME)
//     endgameExpansion();
// }

// void cancelShootingButtonSubscriber() { cancelShooting = true; }

// void safteySwitchSubscriber() { saftey = !saftey; }
// void shoot() {
//   // aiming
//   Position pos = getPosition();
//   if (!cancelShooting) {
//     double xDiff = GOAL_POS.x - pos.x;
//     double yDiff = GOAL_POS.y - pos.y;
//     turn(atan(yDiff / xDiff) + (xDiff > 0 ? 180 : !(yDiff > 0) ? 360 : 0));
//   }
//   if (!cancelShooting)
//     aimTurret(Position::distance(pos, GOAL_POS));

//   // shooting
//   for (int i = 0; i < 3 && !cancelShooting && i < shots; i++) {
//     loadDisc();

//     // wait for delay to repeat
//     this_thread::sleep_for(DISC_LOAD_DELAY);
//   }
//   shots = 0;
//   cancelShooting = false;
//   shooting = false;
// }

// void shootButtonSubscriber() {
//   Brain.Screen.print("pressed");
//   shots = shots + 1;
//   if (shooting)
//     return;
//   shooting = true;

//   thread Thread;
//   (thread(shoot));
// }

// void PTOSwitchSubscriber() {
//   PTOSwitching = true;
//   if (PTOState)
//     PTODrivetrain();
//   else
//     PTOIntake();
// }

// void intakeButtonSubscriber() {
//   PTOIntake();
//   PTOGroup.spin(fwd, 100, pct);
// }

// void intakeReleasedSubscriber() { PTOGroup.stop(); }

// void unstuckIntakeButtonSubscriber() {
//   PTOIntake();
//   PTOGroup.spin(reverse, 100, pct);
// }

// void unstuckIntakeReleasedSubscriber() { PTOGroup.stop(); }

// void subscribeButtonListener(Button button, void (*callback)()) {
//   getControllerButton(button).pressed(callback);
// }

// void subscribeReleasedListener(Button button, void (*callback)()) {
//   getControllerButton(button).released(callback);
// }

// void subscribeAxisListener(Axis axis, void (*callback)()) {
//   getControllerAxis(axis).changed(callback);
// }
enum PTO { DRIVE, INTAKE };

static atomic<bool> PTOState = {INTAKE}; // false = drivetrain, true = intake

enum class TEAM { RED, BLUE } TEAM;
enum class ROLLER { RED, BLUE, IN_BETWEEN } ROLLER;

void flyWheelButtonSubscriber() {
  flyWheelSpin = !flyWheelSpin;
  if (flyWheelSpin)
    FlyWheel.spin(fwd, 10, volt);
  else
    FlyWheel.stop();
}

void spinLeftDrive(directionType dir, double velocity, percentUnits units) {
  std::vector<motor> motors;
  if (PTOState == PTO::INTAKE)
    motors = {LeftDriveA, LeftDriveB}; // intake
  else
    motors = {LeftDriveA, LeftDriveB, PTOLeft}; // drivetrain
  for (auto i = motors.begin(); i != motors.end(); ++i)
    i->spin(dir, velocity, units);
};

void spinRightDrive(directionType dir, double velocity, percentUnits units) {
  std::vector<motor> motors;
  if (PTOState == PTO::INTAKE)
    motors = {RightDriveA, RightDriveB}; // intake
  else
    motors = {RightDriveA, RightDriveB, PTORight}; // drivetrain
  for (auto i = motors.begin(); i != motors.end(); ++i)
    i->spin(dir, velocity, units);
}

void leftDriveSubscriber() {
  spinLeftDrive(fwd, pow((float)axisPosition(LEFT_DRIVE) / 100, 5) * 100, pct);
}

void rightDriveSubscriber() {
  spinRightDrive(fwd, pow((float)axisPosition(RIGHT_DRIVE) / 100, 5) * 100,
                 pct);
}

void PTOToggle() {
  PTOState = !PTOState;
  PTOPiston.set(PTOState);
  printf("pto switch to %d\n", PTOState.load());
  // TODO: change pto stop setting
}
void PTOSetTo(bool setTo) {
  if (PTOState != setTo)
    PTOToggle();
};

struct RollerArea {
public:
  int red;
  int blue;
  void update();
  RollerArea();
};

atomic<bool> spinningRoller = {false};
RollerArea rollArea = {};

RollerArea::RollerArea() : red(0), blue(0) {}
void RollerArea::update() {
  VisionSensor.takeSnapshot(VisionSensor__RED_ROLLER);
  vision::object redObj = VisionSensor.largestObject;
  red = redObj.width * redObj.height;
  VisionSensor.takeSnapshot(VisionSensor__BLUE_ROLLER);
  vision::object blueObj = VisionSensor.largestObject;
  blue = blueObj.width * blueObj.height;
};

enum ROLLER whatIsRoller() {
  rollArea.update();
  if (rollArea.red > 1000 && rollArea.red > rollArea.blue * 5)
    return ROLLER::RED;
  if (rollArea.blue > 1000 && rollArea.blue > rollArea.red * 5)
    return ROLLER::BLUE;
  return ROLLER::IN_BETWEEN;
};

void visionAidedRoller() {
  if (whatIsRoller() == ROLLER::BLUE)
    return;
  PTOSetTo(PTO::INTAKE);
  spinningRoller = true;
  PTOLeft.spin(fwd, 25, pct);
  PTORight.spin(fwd, 25, pct);
  while (whatIsRoller() != ROLLER::BLUE) {

    printf("vision roller\n");
    wait(20, msec);
  }
  spinningRoller = false;
  PTOLeft.stop();
  PTORight.stop();
};
struct NEW_PRESS {
  static atomic<bool> R1;
  static atomic<bool> L1;
  static atomic<bool> R2;
  static atomic<bool> L2;
};
atomic<bool> NEW_PRESS::R1 = {false};
atomic<bool> NEW_PRESS::L1 = {false};
atomic<bool> NEW_PRESS::R2 = {false};
atomic<bool> NEW_PRESS::L2 = {false};

atomic<bool> PTOWasSwitched = {false};

void shiftKeyStuff() {
  while (1) {
    if (NEW_PRESS::R1)
      if (NEW_PRESS::L1) {
        // pto switch
        PTOToggle();
        PTOWasSwitched = true;
      } else {
        PTOSetTo(PTO::INTAKE);
        PTOLeft.spin(fwd, 100, pct);
        PTORight.spin(fwd, 100, pct);
        printf("intake in\n");
      }
    else if (NEW_PRESS::L1)
      printf("shoot\n");
    // if (!spinningRoller)
    //   rollArea.update();
    // printf("red: %d, blue: %d\n", rollArea.red, rollArea.blue);
    // if ((rollArea.red + rollArea.blue) > 30000)
    //   visionAidedRoller();
    wait(100, msec);
  }
}

void controllerDisplay(OdomTracking);

void motorSetup() {
  FlyWheel.setStopping(coast);

  LeftDriveA.setStopping(coast);
  LeftDriveB.setStopping(coast);
  RightDriveA.setStopping(coast);
  RightDriveB.setStopping(coast);

  PTOPiston.set(true);
}

// void tracking() {
//   OdomTracking tracker1 = {{0, 0, 0}};
//   tracker = &tracker1;
// }

const float pidLimit = 30; // pct

float leftAdjustment = 0;
float rightAdjustment = 0;

void spinLeftPID(const float velocityPercent) {
  leftAdjustment += velocityPercent;
};

void spinRightPID(const float velocityPercent) {
  rightAdjustment += velocityPercent;
};

void spinBothPID(const float velocityPercent) {
  spinRightPID(velocityPercent);
  spinLeftPID(velocityPercent);
  // if (abs(velocityPercent) < pidLimit) {
  //   spinRightDrive(fwd, velocityPercent, pct);
  //   spinLeftDrive(fwd, velocityPercent, pct);
  // } else {
  //   printf("both at limit: %f\n", velocityPercent);
  //   spinRightDrive(fwd, velocityPercent > 0 ? pidLimit : -pidLimit, pct);
  //   spinLeftDrive(fwd, velocityPercent > 0 ? pidLimit : -pidLimit, pct);
  // }
};

void spinPID() {
  if (abs(leftAdjustment) < pidLimit)
    spinLeftDrive(fwd, leftAdjustment, pct);
  else {
    printf("left at limit: %f\n", leftAdjustment);
    spinLeftDrive(fwd, leftAdjustment > 0 ? pidLimit : -pidLimit, pct);
  }
  if (abs(rightAdjustment) < pidLimit)
    spinRightDrive(fwd, rightAdjustment, pct);
  else {
    printf("right at limit: %f\n", rightAdjustment);
    spinRightDrive(fwd, rightAdjustment > 0 ? pidLimit : -pidLimit, pct);
  }
  leftAdjustment = 0;
  rightAdjustment = 0;
};

float getLeftPID() {
  return (LeftDriveA.position(degrees) + LeftDriveB.position(degrees)) / 2;
};

float getRightPID() {
  return (RightDriveA.position(degrees) + RightDriveB.position(degrees)) / 2;
};

float getBothPID() { return (getRightPID() + getLeftPID()) / 2; };

void pid() {
  spinLeftPID(10);
  const float kP = 0.5;
  float error = 0;
  float proportional = 0;
  while (1) {
    error = getLeftPID() - getRightPID();
    proportional = error * kP;
    spinRightPID(10 + proportional);
    printf("error: %f, pro: %f\n", error, proportional);
    wait(20, msec);
  }
}

static constexpr float pi2 = 2 * M_PI;
float headingDifference(float a, float b) {
  const float diff = a - b;
  return (diff +
          pi2 * (diff > 0 ? (diff > M_PI ? -1 : 0) : diff < -M_PI ? 1 : 0));
}
PIDController::Callback *stopPID;
PIDController::Callback *turnPID;
// float target = 0;
int main() {
  vexcodeInit();
  motorSetup();

  Inertial10.calibrate(2);
  // wait until inertial finsished calibrating
  while (Inertial10.isCalibrating())
    wait(20, msec);

  // drive code
  Controller1.Axis2.changed(&rightDriveSubscriber);
  Controller1.Axis3.changed(&leftDriveSubscriber);

  // odometry
  OdomTracking tracker1 = {{}};
  tracker = &tracker1;

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //    START PID TESTING
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~

  // pid();
  // drive straight
  // spinLeftPID(10);
  // PIDController::Callback(
  //     0.5, 0, 0, 20, []() -> float { return getLeftPID() - getRightPID();
  // },
  //     [](float adjustment) { spinRightPID(10 + adjustment); })
  //     .pidLoop();

  // stop at distance
  PIDController::Callback stop = PIDController::Callback(
      1, 0, 0, 20,
      []() -> float {
        const float distance = Position::distance(tracker->getRobotPosition());
        printf(")distance:%f\n", distance);
        return distance;
      },
      [](float adjustment1) {
        const float adjustment = 10 * adjustment1 / (0.5 + abs(adjustment1));
        printf("=stop adjustment: %f\n", adjustment);
        spinBothPID(adjustment);
      });
  // .pidLoop();
  // turn

  PIDController::Callback turn = PIDController::Callback(
      1, 0.000, 0, 20,
      []() -> float {
        // const Position vector =
        //     Position::difference({0, 0, 0}, tracker->getRobotPosition());
        // const float target1 = atan2(vector.x, vector.y);
        // const float target = target1 < 0 ? target1 + pi2 : target1;
        const float conIn =
            ((((float)Controller1.Axis1.position()) / 100) + 1) * M_PI;
        const Position vector = {cos(conIn), sin(conIn), 0};
        const float target1 = atan2(vector.y, vector.x);
        const float target = target1 < 0 ? target1 + pi2 : target1;

        const float heading1 =
            Conversions::Degrees::toRadians(Inertial10.heading(degrees));
        // const float heading = heading1 > M_PI ? heading1 - pi2 : heading1;
        const float headingDiff =
            headingDifference(target, heading1) /*  * 100 */;
        printf("{heading: %f\n+diff: %f\n@target: %f\n&input: %f\n", heading1,
               headingDiff /*  / 100 */, target, conIn);
        return headingDiff;
      },
      [](const float adjustment1) {
        const float adjustment = 10 * adjustment1 / (0.5 + abs(adjustment1));
        printf("=turn adjustment: %f\n", adjustment);
        spinLeftPID(adjustment);
        spinRightPID(-adjustment);
      });
  stopPID = &stop;
  turnPID = &turn;
  Controller1.ButtonX.pressed([]() {
    // const Position vector =
    //     Position::difference({0, 0, 0}, tracker->getRobotPosition());
    // const float target1 = atan2(vector.x, vector.y);
    // target = target1 < 0 ? target1 + pi2 : target1;
    while (abs(Position::distance(tracker->getRobotPosition())) > 0.1) {
      if (Controller1.ButtonA.pressing())
        break;
      stopPID->pidSegment();
      turnPID->pidSegment();

      spinPID();
      wait(20, msec);
    }
    RightDriveA.stop();
    RightDriveB.stop();
    LeftDriveA.stop();
    LeftDriveB.stop();
  });

  // position printing
  (thread([]() {
    Position robotPos1;

    while (1) {
      Controller1.Screen.setCursor(0, 0);

      robotPos1 = tracker->getRobotPosition();
      Controller1.Screen.clearLine();
      Controller1.Screen.print("(");
      Controller1.Screen.print(robotPos1.x);
      Controller1.Screen.print(",");
      Controller1.Screen.print(robotPos1.y);
      Controller1.Screen.print(",");
      Controller1.Screen.print(robotPos1.heading);
      Controller1.Screen.print(")");
      Controller1.Screen.newLine();
      wait(20, msec);

      Brain.Screen.clearLine();
      Brain.Screen.print("(");
      Brain.Screen.print(robotPos1.x);
      Brain.Screen.print(",");
      Brain.Screen.print(robotPos1.y);
      Brain.Screen.print(",");
      Brain.Screen.print(robotPos1.heading);
      Brain.Screen.print(")");
      Brain.Screen.newLine();
      wait(20, msec);
    }
  }));

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //    END PID TESTING
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //  START I/O INITIALIZATION
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~

  //   // R1 -> intake in
  //   // R2 -> intake out
  //   // L1 -> shoot
  //   // R1 L1 -> pto switch
  //   // R2 L2 -> flywheel toggle
  //   // L2 -> roller

  //   // shoot pressed
  //   Controller1.ButtonL1.pressed([]() {
  //     NEW_PRESS::L1 = true;
  //     wait(100, msec);
  //     NEW_PRESS::L1 = false;
  //   });
  //   // intake pressed
  //   Controller1.ButtonR1.pressed([]() {
  //     NEW_PRESS::R1 = true;
  //     wait(100, msec);
  //     NEW_PRESS::R1 = false;
  //   });
  //   // intake released
  //   Controller1.ButtonR1.released([]() {
  //     if (!PTOWasSwitched) {
  //       PTOLeft.stop();
  //       PTORight.stop();
  //       printf("intake stopped\n");
  //     } else
  //       PTOWasSwitched = false;
  //   });
  //   // outtake pressed
  //   Controller1.ButtonR2.pressed([]() {
  //     // NEW_PRESS::R2 = true;
  //     // wait(200, msec);
  //     // NEW_PRESS::R2 = false;
  //     PTOSetTo(PTO::INTAKE);
  //     PTOLeft.spin(reverse, 12, volt);
  //     PTORight.spin(reverse, 12, volt);
  //     printf("intake in\n");
  //   });
  //   // outtake released
  //   Controller1.ButtonR2.released([]() {
  //     PTOLeft.stop();
  //     PTORight.stop();
  //     printf("intake stopped\n");
  //   });

  //   Controller1.ButtonL2
  //       .pressed(&visionAidedRoller /* []() {
  // NEW_PRESS::L2 = true;
  // wait(200, msec);
  // NEW_PRESS::L2 = false;
  // } */);

  //   // flywheel
  //   Controller1.ButtonUp.pressed(&flyWheelButtonSubscriber);
  //   // temp testing flywheel
  //   Brain.Screen.pressed(&flyWheelButtonSubscriber);

  //   // start shiftKeyNonsense
  //   thread Thread;
  //   (thread(shiftKeyStuff));

  //   Controller1.ButtonA.pressed([]() {
  //     while (abs(tracker1->getRobotPosition().heading - M_PI) > 0.01)
  //       printf("%f\n", tracker1->getRobotPosition().heading);
  //   });

  // // ANYTHING AFTER THIS FUNCTION WILL NOT BE EXECUTED
  //   controllerDisplay({{}});

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //  END I/O INITIALIZATION
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //  START EARLY I/O INITIALIZATION
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~

  //   // subscribeButtonListener(Button::SHOOT, &shootButtonSubscriber);
  //   // subscribeButtonListener(Button::SAFTEY_SWITCH,
  //   //  &safteySwitchSubscriber);
  //   // subscribeButtonListener(Button::CANCEL_SHOOTING,
  //   //                         &cancelShootingButtonSubscriber);
  //   // subscribeButtonListener(Button::EXPANSION,
  //   //  &expansionButtonSubscriber);
  //   // subscribeButtonListener(Button::INTAKE, &intakeButtonSubscriber);
  //   // subscribeButtonListener(Button::UNSTUCK_INTAKE,
  //   //                         &unstuckIntakeButtonSubscriber);
  //   // subscribeButtonListener(Button::PTO_SWITCH, &PTOSwitchSubscriber);
  //   // subscribeButtonListener(Button::FLY_WHEEL_TOGGLE,
  //   // &flyWheelButtonSubscriber);

  //   // subscribeReleasedListener(Button::UNSTUCK_INTAKE,
  //   //                           &unstuckIntakeReleasedSubscriber);
  //   // subscribeReleasedListener(Button::INTAKE, &intakeReleasedSubscriber);

  //     // subscribeAxisListener(Axis::LEFT_DRIVE, &leftDriveSubscriber);
  //     // subscribeAxisListener(Axis::RIGHT_DRIVE, &rightDriveSubscriber);
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //  END EARLY I/O INITIALIZATION
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~
}

void controllerDisplay(OdomTracking tracker1) {
  {
    tracker = &tracker1;
    Position robotPos1;
    while (1) {
      // reset
      Controller1.Screen.setCursor(0, 0);

      // shots
      // Controller1.Screen.clearLine();
      // Controller1.Screen.print("shots: ");
      // Controller1.Screen.print(shots);
      // Controller1.Screen.newLine();
      // PTO State
      Controller1.Screen.clearLine();
      if (PTOState == PTO::INTAKE)
        Controller1.Screen.print("intake");
      else
        Controller1.Screen.print("drive");
      Controller1.Screen.newLine();

      // // Deez Nuts
      // Controller1.Screen.clearLine();
      // Controller1.Screen.print(/* "deez nuts" */ tracker.deltaPos1.x);
      // // Controller1.Screen.print(/* "deez nuts" */ tracker.data.curr.pos.x);

      // temp
      Controller1.Screen.clearLine();
      Controller1.Screen.print("left: ");
      Controller1.Screen.print(
          (LeftDriveA.temperature(pct) + LeftDriveB.temperature(pct)) / 2);
      Controller1.Screen.newLine();

      Controller1.Screen.clearLine();
      Controller1.Screen.print("right: ");
      Controller1.Screen.print(
          (RightDriveA.temperature(pct) + RightDriveB.temperature(pct)) / 2);
      Controller1.Screen.newLine();

      // // tracking
      // robotPos1 = tracker.getRobotPosition();
      // Brain.Screen.setCursor(1, 1);
      // Brain.Screen.clearLine();
      // Brain.Screen.print("(");
      // Brain.Screen.print(robotPos1.x);
      // Brain.Screen.print(",");
      // Brain.Screen.print(robotPos1.y);
      // Brain.Screen.print(",");
      // Brain.Screen.print(robotPos1.heading);
      // Brain.Screen.print(")");
      // Brain.Screen.newLine();

      // Controller1.Screen.clearLine();
      // Controller1.Screen.print("(");
      // Controller1.Screen.print(robotPos1.x);
      // Controller1.Screen.print(",");
      // Controller1.Screen.print(robotPos1.y);
      // Controller1.Screen.print(",");
      // Controller1.Screen.print(robotPos1.heading);
      // Controller1.Screen.print(")");
      // Controller1.Screen.newLine();

      wait(50, msec);
    }
  }
}