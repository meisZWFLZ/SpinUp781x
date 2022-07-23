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
// discLoader           motor         1
// PTOGroup             motor_group   2, 3
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

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
  double x; // inches
  double y; // inches
  double heading;

public:
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
  UNSTUCK_INTAKE
};

controller::button getControllerButton(Button button) {
  controller::button ControllerButtons[] = {
      Controller1.ButtonA, Controller1.ButtonB,    Controller1.ButtonX,
      Controller1.ButtonY, Controller1.ButtonDown, Controller1.ButtonR1,
      Controller1.ButtonR2};
  return ControllerButtons[button];
}

// const std::chrono::duration<long long, milli> LENGTH_OF_GAME = 12000ms;
const int LENGTH_OF_GAME = 20000;
// const std::chrono::duration<long long, milli> LENGTH_OF_ENGAME = 10000ms;
const int LENGTH_OF_ENGAME = 10000;
discLoader.setStopping(brake); 
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

static bool PTOState = false; // false = drivetrain, true = intake

static bool expanded = false;
static bool saftey = false; // true -> disables saftey mechanisms

bool buttonPressed(Button button) {
  getControllerButton(button).pressing();
  return false;
}

void PTOIntake() {
  // freeze drivetrain and pto motors
  PTOState = true;
}

void endgameExpansion() {
  // expansion stuff
}

void loadDisc() { discLoader.spinFor(fwd, 90, degrees); }

Position getPosition() { return {}; }

void turn(double newHeading) { wait(150, msec); }

void aimTurret(int distance /*inches*/) { wait(150, msec); }

void expansionButtonSubscriber() {
  // prevents expansion untill endgame
  if (saftey || 432423 > TIME_OF_ENGAME)
    endgameExpansion();
}

void cancelShootingButtonSubscriber() { cancelShooting = true; }

void safteySwitchSubscriber() { saftey ^= saftey; }
void shoot() {
  // aiming
  Position pos = getPosition();
  if (!cancelShooting)
    turn(0 /*too lazy*/);
  if (!cancelShooting)
    aimTurret(Position::distance(pos, GOAL_POS));

  // shooting
  for (shots = (shots < 3 ? shots : 3); shots != 0 && !cancelShooting;
       shots--) {
    loadDisc();

    // wait for delay to repeat
    this_thread::sleep_for(DISC_LOAD_DELAY);
  }
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

void intakeButtonSubscriber() {
  PTOIntake();
  PTOGroup.spin(fwd, 100, pct);
}
void intakeReleasedSubscriber() { PTOGroup.stop(); }

void subscribeButtonListener(Button button, void (*callback)()) {
  getControllerButton(button).pressed(callback);
}
void subscribeReleasedListener(Button button, void (*callback)()) {
  getControllerButton(button).released(callback);
}

int main() {
  vexcodeInit();

  subscribeButtonListener(Button::SHOOT, &shootButtonSubscriber);
  subscribeButtonListener(Button::SAFTEY_SWITCH, &safteySwitchSubscriber);
  subscribeButtonListener(Button::CANCEL_SHOOTING,
                          &cancelShootingButtonSubscriber);
  subscribeButtonListener(Button::EXPANSION, &expansionButtonSubscriber);
  subscribeButtonListener(Button::INTAKE, &intakeButtonSubscriber);
  subscribeReleasedListener(Button::INTAKE, &intakeReleasedSubscriber);
}