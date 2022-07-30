

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\neilj                                            */
/*    Created:      Sat Jul 23 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller
// DiscLoader           motor         1
// PTOLeft              motor         2
// PTORight             motor         3
// PTOPiston            digital_out   A
// LeftDriveA           motor         4
// LeftDriveB           motor         5
// RightDriveA          motor         6
// RightDriveB          motor         7
// FlyWheel             motor         8
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

#include <atomic>
#include <cmath>
#include <iostream>
#include <thread>

using namespace vex;

using namespace std;
// using namespace std::chrono_literals;

/*
NOlong ES:
- normal drivetrain?
- tank drive control scheme
- automatic shooting :(
- distance in inches
*/

class Position {
public:
  double x; // inches
  double y; // inches
  double heading;

  Position(double x, double y, double heading)
      : x(x), y(y), heading(fmod(heading, 360)){};
  Position() : x(0), y(0), heading(0){};

  static double distance(const Position &pos1, const Position &pos2) {
    return sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2));
  }
};

enum Button {
  CANCEL_SHOOTING,
  EXPANSION,
  SAFTEY_SWITCH /*enables and disables saftey mechanisms*/,
  SHOOT,
  ROLLER,
  INTAKE,
  UNSTUCK_INTAKE,
  PTO_SWITCH,
  FLY_WHEEL_TOGGLE
};

controller::button getControllerButton(Button button) {
  controller::button ControllerButtons[] = {
      Controller1.ButtonA,  Controller1.ButtonB,    Controller1.ButtonX,
      Controller1.ButtonY,  Controller1.ButtonDown, Controller1.ButtonR1,
      Controller1.ButtonR2, Controller1.ButtonLeft, Controller1.ButtonUp};
  return ControllerButtons[button];
}

enum Axis { LEFT_DRIVE, RIGHT_DRIVE };

controller::axis getControllerAxis(Axis axis) {
  controller::axis ControllerAxises[] = {Controller1.Axis3, Controller1.Axis2};
  return ControllerAxises[axis];
}

static motor_group PTOGroup;

// const std::chrono::duration<long long, milli> LENGTH_OF_GAME = 12000ms;
const int LENGTH_OF_GAME = 20000;
// const std::chrono::duration<long long, milli> LENGTH_OF_ENGAME = 10000ms;
const int LENGTH_OF_ENGAME = 10000;

// const std::chrono::steady_clock::time_point TIME_OF_ENGAME =
// chrono::steady_clock::now() + LENGTH_OF_GAME - LENGTH_OF_ENGAME;
const int TIME_OF_ENGAME =
    /* chrono::steady_clock::now() + */ LENGTH_OF_GAME - LENGTH_OF_ENGAME;

// const std::chrono::duration<long long, milli> DISC_LOAD_DELAY = 500ms;
// //should be time piston takes to complete full cycle
const int DISC_LOAD_DELAY =
    500; // should be time piston takes to complete full cycle

const Position GOAL_POS = {0, 0, 0};

static bool shooting = false; // disables drivetrain whilst aiming/shooting
static int shots = 0; // tap shoot button to add more discs after original press
static bool cancelShooting = false;

static atomic<bool> shootingYawed;   // wait until yawed to shoot
static atomic<bool> shootingPitched; // wait until pitched to shoot

static bool PTOState = false;     // false = drivetrain, true = intake
static bool PTOSwitching = false; // Disable PTO motors and drivetrain

static bool flyWheelSpin = true;

static bool expanded = false;
static bool saftey = false; // true -> disables saftey mechanisms

bool buttonPressed(Button button) {
  return getControllerButton(button).pressing();
}

int axisPosition(Axis axis) { return getControllerAxis(axis).position(); }

motor_group getLeftDrive() {
  return PTOState ? motor_group(LeftDriveA, LeftDriveB)
                  : motor_group(LeftDriveA, LeftDriveB, PTOLeft);
}

motor_group getRightDrive() {
  return PTOState ? motor_group(RightDriveA, RightDriveB)
                  : motor_group(RightDriveA, RightDriveB, PTORight);
}

void PTOIntake() {
  if (PTOState)
    return;
  Brain.Screen.print("intake");
  PTOPiston.set(false);
  // freeze drivetrain and pto motors
  PTOState = true;
  PTOSwitching = false;
}

void PTODrivetrain() {
  if (!PTOState)
    return;
  Brain.Screen.print("drive");
  PTOPiston.set(true);
  // freeze drivetrain and pto motors
  PTOState = false;
  PTOSwitching = false;
}

void endgameExpansion() {
  // expansion stuff
}

void loadDisc() { DiscLoader.spinFor(fwd, 90, degrees); }

Position getPosition() { return {}; } // gets robot position

void turn(double newHeading) { wait(600, msec); }

void aimTurret(int distance /*inches*/) { wait(0, msec); }

void expansionButtonSubscriber() {
  // prevents expansion untill endgame
  if (saftey || 432423 > TIME_OF_ENGAME)
    endgameExpansion();
}

void cancelShootingButtonSubscriber() { cancelShooting = true; }

void safteySwitchSubscriber() { saftey = !saftey; }
void shoot() {
  // aiming
  Position pos = getPosition();
  if (!cancelShooting)
    turn(0);
  if (!cancelShooting)
    aimTurret(Position::distance(pos, GOAL_POS));

  // shooting
  for (int i = 0; i < 3 && !cancelShooting && i < shots; i++) {
    loadDisc();

    // wait for delay to repeat
    this_thread::sleep_for(DISC_LOAD_DELAY);
  }
  shots = 0;
  cancelShooting = false;
  shooting = false;
}

void shootButtonSubscriber() {
  Brain.Screen.print("pressed");
  shots = shots + 1;
  if (shooting)
    return;
  shooting = true;

  thread Thread;
  (thread(shoot));
}

void PTOSwitchSubscriber() {
  PTOSwitching = true;
  if (PTOState)
    PTODrivetrain();
  else
    PTOIntake();
}

void intakeButtonSubscriber() {
  PTOIntake();
  PTOGroup.spin(fwd, 100, pct);
}

void intakeReleasedSubscriber() { PTOGroup.stop(); }

void unstuckIntakeButtonSubscriber() {
  PTOIntake();
  PTOGroup.spin(reverse, 25, pct);
}

void flyWheelButtonSubscriber() {
  flyWheelSpin = !flyWheelSpin;
  if(flyWheelSpin) FlyWheel.spin(fwd);
  else FlyWheel.stop();
}

// void leftDriveSubscriber() {
//   getLeftDrive().spin(fwd, pow((float)axisPosition(LEFT_DRIVE) / 100, 3) * 100,
//                       pct);
// }

//old
void leftDriveSubscriber() {
  getLeftDrive().spin(fwd, ((axisPosition(LEFT_DRIVE) / 100) ^ 3) * 100, pct);
}

void rightDriveSubscriber() {
  getRightDrive().spin(
      fwd, pow((float)axisPosition(RIGHT_DRIVE) / 100, 3) * 100, pct);
}

void unstuckIntakeReleasedSubscriber() { PTOGroup.stop(); }

void subscribeButtonListener(Button button, void (*callback)()) {
  getControllerButton(button).pressed(callback);
}

void subscribeReleasedListener(Button button, void (*callback)()) {
  getControllerButton(button).released(callback);
}

void subscribeAxisListener(Axis axis, void (*callback)()) {
  getControllerAxis(axis).changed(callback);
}

void controllerDisplay();

void motorSetup() {
  PTOGroup = motor_group(PTOLeft, PTORight);

  FlyWheel.setBrake(coast);
  FlyWheel.setVelocity(100, pct);
  FlyWheel.spin(fwd);

  PTOPiston.set(false);
}

int main() {
  vexcodeInit();

  motorSetup();

  subscribeButtonListener(Button::SHOOT, &shootButtonSubscriber);
  subscribeButtonListener(Button::SAFTEY_SWITCH, &safteySwitchSubscriber);
  subscribeButtonListener(Button::CANCEL_SHOOTING,
                          &cancelShootingButtonSubscriber);
  subscribeButtonListener(Button::EXPANSION, &expansionButtonSubscriber);
  subscribeButtonListener(Button::INTAKE, &intakeButtonSubscriber);
  subscribeButtonListener(Button::UNSTUCK_INTAKE,
                          &unstuckIntakeButtonSubscriber);
  subscribeButtonListener(Button::PTO_SWITCH, &PTOSwitchSubscriber);
  subscribeButtonListener(Button::FLY_WHEEL_TOGGLE, &flyWheelButtonSubscriber);

  subscribeReleasedListener(Button::UNSTUCK_INTAKE,
                            &unstuckIntakeReleasedSubscriber);
  subscribeReleasedListener(Button::INTAKE, &intakeReleasedSubscriber);

  subscribeAxisListener(Axis::LEFT_DRIVE, &leftDriveSubscriber);
  subscribeAxisListener(Axis::RIGHT_DRIVE, &rightDriveSubscriber);

  controllerDisplay();
}

void controllerDisplay() {
  {
    while (1) {
      // reset
      Controller1.Screen.setCursor(0, 0);

      // shots
      Controller1.Screen.clearLine();
      Controller1.Screen.print("shots: ");
      Controller1.Screen.print(shots);
      Controller1.Screen.newLine();

      // PTO State
      Controller1.Screen.clearLine();
      Controller1.Screen.print(PTOState ? "intake" : "drive");
      Controller1.Screen.newLine();

      // Deez Nuts
      Controller1.Screen.clearLine();
      Controller1.Screen.print("deez nuts");
      Controller1.Screen.newLine();

      wait(50, msec);
    }
  }
}