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
// VisionSense          vision        12
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
// VisionSense          vision        12
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
// VisionSense          vision        12
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
// Vision12             vision        12
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
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "odom/tracking.h"
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

// const Position GOAL_POS = {0, 0, 0};

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
static atomic<bool> PTOState = {true}; // false = drivetrain, true = intake

enum PTO { DRIVE, INTAKE };
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
  spinLeftDrive(fwd, pow((float)axisPosition(LEFT_DRIVE) / 100, 3) * 100, pct);
}

void rightDriveSubscriber() {
  spinRightDrive(fwd, pow((float)axisPosition(RIGHT_DRIVE) / 100, 3) * 100,
                 pct);
}

void PTOToggle() {
  PTOState = !PTOState;
  PTOPiston.set(PTOState);
  printf("pto switch to %d\n", PTOState.load());
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
    if (!spinningRoller)
      rollArea.update();
    printf("red: %d, blue: %d\n", rollArea.red, rollArea.blue);
    if ((rollArea.red + rollArea.blue) > 30000)
      visionAidedRoller();
    wait(200, msec);
  }
}

void controllerDisplay(OdomTracking);

void motorSetup() {
  // FlyWheel.setBrake(coast);

  PTOPiston.set(true);
}

void tracking() {
  OdomTracking tracker1 = {{0, 0, 0}};
  tracker = &tracker1;
}

int main() {
  vexcodeInit();
  // R1 -> intake in
  // R2 -> intake out
  // L1 -> shoot
  // R1 L1 -> pto switch
  // R2 L2 -> flywheel toggle
  // L2 -> roller

  // shoot pressed
  Controller1.ButtonL1.pressed([]() {
    NEW_PRESS::L1 = true;
    wait(200, msec);
    NEW_PRESS::L1 = false;
  });
  // intake pressed
  Controller1.ButtonR1.pressed([]() {
    NEW_PRESS::R1 = true;
    wait(200, msec);
    NEW_PRESS::R1 = false;
  });
  // intake released
  Controller1.ButtonR1.released([]() {
    if (!PTOWasSwitched) {
      PTOLeft.stop();
      PTORight.stop();
      printf("intake stopped\n");
    } else
      PTOWasSwitched = false;
  });
  // outtake pressed
  Controller1.ButtonR2.pressed([]() {
    // NEW_PRESS::R2 = true;
    // wait(200, msec);
    // NEW_PRESS::R2 = false;
    PTOSetTo(PTO::INTAKE);
    PTOLeft.spin(reverse, 100, pct);
    PTORight.spin(reverse, 100, pct);
    printf("intake in\n");
  });
  // outtake released
  Controller1.ButtonR2.released([]() {
    PTOLeft.stop();
    PTORight.stop();
    printf("intake stopped\n");
  });

  Controller1.ButtonL2
      .pressed(&visionAidedRoller /* []() {
NEW_PRESS::L2 = true;
wait(200, msec);
NEW_PRESS::L2 = false;
} */);

  // flywheel
  Controller1.ButtonUp.pressed(&flyWheelButtonSubscriber);
  // temp testing flywheel
  Brain.Screen.pressed(&flyWheelButtonSubscriber);

  Controller1.Axis2.changed(&rightDriveSubscriber);
  Controller1.Axis3.changed(&leftDriveSubscriber);

  // start shiftKeyNonsense
  thread Thread;
  (thread(shiftKeyStuff));

  motorSetup();

  // subscribeButtonListener(Button::SHOOT, &shootButtonSubscriber);
  // subscribeButtonListener(Button::SAFTEY_SWITCH, &safteySwitchSubscriber);
  // subscribeButtonListener(Button::CANCEL_SHOOTING,
  //                         &cancelShootingButtonSubscriber);
  // subscribeButtonListener(Button::EXPANSION, &expansionButtonSubscriber);
  // subscribeButtonListener(Button::INTAKE, &intakeButtonSubscriber);
  // subscribeButtonListener(Button::UNSTUCK_INTAKE,
  //                         &unstuckIntakeButtonSubscriber);
  // subscribeButtonListener(Button::PTO_SWITCH, &PTOSwitchSubscriber);
  // subscribeButtonListener(Button::FLY_WHEEL_TOGGLE,
  // &flyWheelButtonSubscriber);

  // subscribeReleasedListener(Button::UNSTUCK_INTAKE,
  //                           &unstuckIntakeReleasedSubscriber);
  // subscribeReleasedListener(Button::INTAKE, &intakeReleasedSubscriber);

  // subscribeAxisListener(Axis::LEFT_DRIVE, &leftDriveSubscriber);
  // subscribeAxisListener(Axis::RIGHT_DRIVE, &rightDriveSubscriber);

  // start tracking
  // thread Thread;
  // (thread(tracking));

  controllerDisplay({{}});
}

void controllerDisplay(OdomTracking tracker) {
  {
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

      // tracking
      robotPos1 = tracker.getRobotPosition();
      Brain.Screen.setCursor(1, 1);
      Brain.Screen.clearLine();
      Brain.Screen.print("(");
      Brain.Screen.print(robotPos1.x);
      Brain.Screen.print(",");
      Brain.Screen.print(robotPos1.y);
      Brain.Screen.print(",");
      Brain.Screen.print(robotPos1.heading);
      Brain.Screen.print(")");
      Brain.Screen.newLine();

      Controller1.Screen.clearLine();
      Controller1.Screen.print("(");
      Controller1.Screen.print(robotPos1.x);
      Controller1.Screen.print(",");
      Controller1.Screen.print(robotPos1.y);
      Controller1.Screen.print(",");
      Controller1.Screen.print(robotPos1.heading);
      Controller1.Screen.print(")");
      Controller1.Screen.newLine();

      wait(50, msec);
    }
  }
}