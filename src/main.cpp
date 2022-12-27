#include "auton/action.h"
#include "auton/elements.h"
#include "auton/selector.h"
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
// #endifOdomTracking(

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
//   for (int i = 0; i < 3 && !cancelShooting && i < shots; i++) {l
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

// enum class TEAM : bool { RED, BLUE } TEAM;
// enum TEAM team;
enum class ROLLER : int { RED, BLUE, IN_BETWEEN } ROLLER;

static const bool operator==(const enum Robot::TEAM team1,
                             const enum ROLLER roller1) {
  return (int)team1 == (int)roller1;
};
static const bool operator==(const enum ROLLER roller1,
                             const enum Robot::TEAM team1) {
  return (int)team1 == (int)roller1;
};

// void flyWheelButtonSubscriber() {
//   flyWheelSpin = !flyWheelSpin;
//   if (flyWheelSpin)
//     FlyWheel.spin(fwd, 10, volt);
//   else
//     FlyWheel.stop();
// }

// void spinLeftDrive(directionType dir, double velocity, percentUnits units) {
//   std::vector<motor> motors;
//   if (PTOState == PTO::INTAKE)
//     motors = {LeftDriveA, LeftDriveB}; // intake
//   else
//     motors = {LeftDriveA, LeftDriveB, PTOLeft}; // drivetrain
//   for (auto i = motors.begin(); i != motors.end(); ++i)
//     i->spin(dir, velocity, units);
// };

// void spinRightDrive(directionType dir, double velocity, percentUnits units) {
//   std::vector<motor> motors;
//   if (PTOState == PTO::INTAKE)
//     motors = {RightDriveA, RightDriveB}; // intake
//   else
//     motors = {RightDriveA, RightDriveB, PTORight}; // drivetrain
//   for (auto i = motors.begin(); i != motors.end(); ++i)
//     i->spin(dir, velocity, units);
// }

void leftDriveSubscriber() {
  // spinLeftDrive(fwd, pow((float)axisPosition(LEFT_DRIVE) / 100, 5) * 100,
  // pct);
  Robot::Drivetrain::left(pow((float)axisPosition(LEFT_DRIVE) / 100, 5));
}

void rightDriveSubscriber() {
  // spinRightDrive(fwd, pow((float)axisPosition(RIGHT_DRIVE) / 100, 5) * 100,
  //                pct);
  Robot::Drivetrain::right(pow((float)axisPosition(RIGHT_DRIVE) / 100, 5));
}

// struct RollerArea {
// public:
//   int red;
//   int blue;
//   void update();
//   RollerArea();
// };

// atomic<bool> spinningRoller = {false};
// RollerArea rollArea = {};

// RollerArea::RollerArea() : red(0), blue(0) {}
// void RollerArea::update() {
//   // VisionSensor.takeSnapshot(VisionSensor__RED_ROLLER);
//   // vision::object redObj = VisionSensor.largestObject;
//   // red = redObj.width * redObj.height;
//   // VisionSensor.takeSnapshot(VisionSensor__BLUE_ROLLER);
//   // vision::object blueObj = VisionSensor.largestObject;
//   // blue = blueObj.width * blueObj.height;
//   // printf("updating\n");
// };

// enum ROLLER whatIsRoller() {
//   rollArea.update();
//   if (rollArea.red > 1000 && rollArea.red > rollArea.blue * 5)
//     return ROLLER::RED;
//   if (rollArea.blue > 1000 && rollArea.blue > rollArea.red * 5)
//     return ROLLER::BLUE;
//   return ROLLER::IN_BETWEEN;
// };

// void visionAidedRoller() {
//   if (whatIsRoller() == Robot::team)
//     return;
//   // Robot::Actions::pto(Robot::PTO_STATE::INTAKE);
//   spinningRoller = true;
//   Intake.spin(fwd, 25, pct);
//   while (!(whatIsRoller() == Robot::team)) {
//     printf("vision roller\n");
//     wait(20, msec);
//   }
//   spinningRoller = false;
//   Intake.stop();
// };

// bool readyToShoot = true;

void shootListener() { Robot::Actions::shoot((Robot::GOAL)(int)Robot::team); }

// struct NEW_PRESS {
//   static atomic<bool> R1;
//   static atomic<bool> L1;
//   static atomic<bool> R2;
//   static atomic<bool> L2;
// };
// atomic<bool> NEW_PRESS::R1 = {false};
// atomic<bool> NEW_PRESS::L1 = {false};
// atomic<bool> NEW_PRESS::R2 = {false};
// atomic<bool> NEW_PRESS::L2 = {false};

// atomic<bool> PTOWasSwitched = {false};

// void shiftKeyStuff() {
//   while (1) {
//     if (NEW_PRESS::R1)
//       if (NEW_PRESS::L1) {
//         // pto switch
//         // Robot::Actions::pto((Robot::PTO_STATE)(!((bool)Robot::PTOState)));
//         // PTOWasSwitched = true;
//       } else {
//         Robot::Actions::intake();
//         printf("intake in\n");
//       }
//     else if (NEW_PRESS::L1) {
//       Robot::Actions::shoot(Robot::GOAL::MY_TEAM);
//       // printf("shoot\n");
//       // shootListener();
//     }

//     // if (!spinningRoller)
//     //   rollArea.update();
//     // printf("red: %d, blue: %d\n", rollArea.red, rollArea.blue);
//     // if ((rollArea.red + rollArea.blue) > 30000)
//     //   visionAidedRoller();
//     wait(100, msec);
//   }
// }

void controllerDisplay(/* OdomTracking */);

void motorSetup() {
  // FlyWheel.setStopping(coast);
  ExpansionPiston.set(false);

  Catapult1.setStopping(coast);
  Catapult1.setVelocity(100, pct);

  Robot::Drivetrain::Left = new motor_group(LeftDriveA, LeftDriveB, LeftDriveC);
  Robot::Drivetrain::Right =
      new motor_group(RightDriveA, RightDriveB, RightDriveC);
  Robot::Drivetrain::Left->setStopping(coast);
  Robot::Drivetrain::Right->setStopping(coast);
  RollerSensor.setLightPower(100, pct);

  // PTOPiston.set(false);
}

// void tracking() {
//   OdomTracking tracker1 = {{0, 0, 0}};
//   tracker = &tracker1;
// }

// const float pidLimit = 50; // pct

// float leftAdjustment = 0;
// float rightAdjustment = 0;

// void spinLeftPID(const float velocityPercent) {
//   leftAdjustment += velocityPercent;
// };

// void spinRightPID(const float velocityPercent) {
//   rightAdjustment += velocityPercent;
// };

// void spinBothPID(const float velocityPercent) {
//   spinRightPID(velocityPercent);
//   spinLeftPID(velocityPercent);
//   // if (abs(velocityPercent) < pidLimit) {
//   //   spinRightDrive(fwd, velocityPercent, pct);
//   //   spinLeftDrive(fwd, velocityPercent, pct);
//   // } else {
//   //   printf("both at limit: %f\n", velocityPercent);
//   //   spinRightDrive(fwd, velocityPercent > 0 ? pidLimit : -pidLimit, pct);
//   //   spinLeftDrive(fwd, velocityPercent > 0 ? pidLimit : -pidLimit, pct);
//   // }
// };

// void spinPID() {
//   if (abs(leftAdjustment) < pidLimit)
//     spinLeftDrive(fwd, leftAdjustment, pct);
//   else {
//     printf("left at limit: %f\n", leftAdjustment);
//     spinLeftDrive(fwd, leftAdjustment > 0 ? pidLimit : -pidLimit, pct);
//   }
//   if (abs(rightAdjustment) < pidLimit)
//     spinRightDrive(fwd, rightAdjustment, pct);
//   else {
//     printf("right at limit: %f\n", rightAdjustment);
//     spinRightDrive(fwd, rightAdjustment > 0 ? pidLimit : -pidLimit, pct);
//   }
//   leftAdjustment = 0;
//   rightAdjustment = 0;
// };

// float getLeftPID() {
//   return (LeftDriveA.position(degrees) + LeftDriveB.position(degrees)) / 2;
// };

// float getRightPID() {
//   return (RightDriveA.position(degrees) + RightDriveB.position(degrees)) / 2;
// };

// float getBothPID() { return (getRightPID() + getLeftPID()) / 2; };

// void pid() {
//   spinLeftPID(10);
//   const float kP = 0.5;
//   float error = 0;
//   float proportional = 0;
//   while (1) {
//     error = getLeftPID() - getRightPID();
//     proportional = error * kP;
//     spinRightPID(10 + proportional);
//     printf("error: %f, pro: %f\n", error, proportional);
//     wait(20, msec);
//   }
// }

static constexpr float pi2 = 2 * M_PI;
constexpr float headingDifference(float a, float b) {
  const float diff = a - b;
  return (diff +
          pi2 * (diff > 0 ? (diff > M_PI ? -1 : 0) : diff < -M_PI ? 1 : 0));
}
// PIDController::Callback *stopPID;
// PIDController::Callback *turnPID;
// float target = 0;

// class AutonSelection {
// public:
//   static std::vector<std::function<void(void)>> autonArr;
//   static int autonPos;
//   static void print() {
//     Brain.Screen.clearScreen();
//     Brain.Screen.setFont(monoXXL);
//     Brain.Screen.setFillColor(transparent);
//     Brain.Screen.setCursor(1, 1);
//     Brain.Screen.print(autonPos);
//     // Brain.Screen.newLine();
//     // Brain.Screen.setFont(monoXL);
//     // switch (autonPos) {
//     //   case 0:
//     // }
//     Brain.Screen.setFillColor(Robot::TEAM::RED == Robot::team ? red : blue);
//     Brain.Screen.drawRectangle(240, 0, 480, 240);
//   }
//   static void listener() {
//     if (Brain.Screen.xPosition() < 240) {
//       // auton
//       autonPos = (autonPos + 1) % autonArr.size();
//     } else {
//       // team
//       Robot::team = Robot::TEAM::RED == Robot::team ? Robot::TEAM::BLUE
//                                                     : Robot::TEAM::RED;
//     }
//     print();
//   }
//   static const void
//   start(const std::vector<std::function<void(void)>> autonArr1) {
//     // autonArr1[0]();
//     autonArr = autonArr1;
//     Brain.Screen.setFont(monoXXL);
//     Brain.Screen.pressed(AutonSelection::listener);
//     print();
//   };
//   static void run() {
//     Brain.Screen.setFont(monoL);
//     Brain.Screen.setFillColor(transparent);
//     autonArr[/* autonPos */ 0]();
//   };
// };
// std::vector<std::function<void(void)>> AutonSelection::autonArr = {};
// int AutonSelection::autonPos = 0;

// void autonSelection(const std::vector<std::function<void(void)>> autonArr1) {
//   static int autoPos = 0;
//   static const std::vector<std::function<void(void)>> autonArr = autonArr1;
// }
// competition Competition;

// void autonomous() {
//   Brain.Screen.print("neil bad!");

//   // Robot::Drivetrain::left(10);
//   // Robot::Drivetrain::right(10);
//   // wait(250, msec);
//   // AutonSelection::run(); 
//   // // Robot::Actions::outtake();
//   // // Robot::Drivetrain::left(-10);
//   // // Robot::Drivetrain::right(-10);
//   // wait(250, msec);
//   // // Robot::Actions::stopIntake();
//   // Robot::Drivetrain::left(0);
//   // Robot::Drivetrain::right(0);
//   // Robot::Drivetrain::left(30);
//   // Robot::Drivetrain::right(30);
//   // wait(750, msec);
//   // // AutonSelection::run();
//   // Robot::Actions::outtake();
//   // Robot::Drivetrain::left(-15);
//   // Robot::Drivetrain::right(-15);
//   // wait(750, msec);
//   // Robot::Actions::stopIntake();
//   // Robot::Drivetrain::left(0);
//   // Robot::Drivetrain::right(0);
// };
void expansionCheck() {
  if (Controller1.ButtonA.pressing() && Controller1.ButtonB.pressing() &&
      Controller1.ButtonX.pressing() && Controller1.ButtonY.pressing())
    // Robot::Actions::expand();
    Robot::Actions::expand();
};
void driverControl() {
  thread([] { controllerDisplay(/* {{}} */); });
  // drive code
  Controller1.Axis2.changed(&rightDriveSubscriber);
  Controller1.Axis3.changed(&leftDriveSubscriber);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //    START CATA TEST
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~

  Controller1.ButtonUp.pressed([]() {
    Catapult1.spin(fwd, 100, pct);
    while (Controller1.ButtonUp.pressing()) {
      wait(10, msec);
    }
    Catapult1.stop();
  });
  Controller1.ButtonDown.pressed([]() {
    Catapult1.spin(reverse, 100, pct);
    while (Controller1.ButtonDown.pressing()) {
      wait(10, msec);
    }
    Catapult1.stop();
  });

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //    END CATA TEST
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~

  // R1 -> intake in (toggle)
  // R2 -> intake out
  // L1 -> shoot
  // R2 L2 -> flywheel toggle
  // L2 -> roller
  // a b -> expansion

  // printf("neil");

  // shoot pressed
  Controller1.ButtonL1.pressed(shootListener);
  // intake pressed
  Controller1.ButtonR1.pressed([]() {
    // static bool intaking = false;
    // intaking = !intaking;
    // printf("int: 5")
    // printf("vel: %f\n", Intake.voltage(volt));
    if (Intake.voltage(volt) > 0.1)
      Robot::Actions::stopIntake();
    else
      Robot::Actions::intake();
  });
  // outtake pressed
  Controller1.ButtonR2.pressed([]() {
    // printf("int: %i\n", Intake.direction());
    // static bool outtaking = false;
    // outtaking = !outtaking;
    // printf("vel: %f\n", Intake.voltage(volt));
    if (Intake.voltage(volt) < -0.1)
      Robot::Actions::stopIntake();
    else
      Robot::Actions::outtake();
  });

  Controller1.ButtonL2.pressed([] { Robot::Actions::roller(); });

  // flywheel
  // Controller1.ButtonUp.pressed(&flyWheelButtonSubscriber);
  // temp testing flywheel
  // Brain.Screen.pressed(&flyWheelButtonSubscriber);

  // expansion
  Controller1.ButtonA.pressed(expansionCheck);
  Controller1.ButtonB.pressed(expansionCheck);
  Controller1.ButtonX.pressed(expansionCheck);
  Controller1.ButtonY.pressed(expansionCheck);

  // Controller1.ButtonX.pressed([])

  // start shiftKeyNonsense
  // thread Thread;
  // (thread(shiftKeyStuff));

  // Controller1.ButtonA.pressed([]() {
  //   while (abs(tracker1->getRobotPosition().heading - M_PI) > 0.01)
  //     printf("%f\n", tracker1->getRobotPosition().heading);
  // });

  // ANYTHING AFTER THIS FUNCTION WILL NOT BE EXECUTED
  // controllerDisplay({{}});
}
// void turning(int target) {
//   Robot::Drivetrain::right(.30);
//   Robot::Drivetrain::left(-.30);
//   while (std::abs(target - Inertial10.heading(degrees)) > 5) {
//     wait(20, msec);
//   }
//   Robot::Drivetrain::right(0);
//   Robot::Drivetrain::left(0);
// }
// void turning2(int target) {
//   if (headingDifference(Inertial10.heading(degrees), target + 90) >
//       headingDifference(Inertial10.heading(degrees), target - 90)) {
//     Robot::Drivetrain::right(.30);
//     Robot::Drivetrain::left(-.30);
//   } else {
//     Robot::Drivetrain::right(-.30);
//     Robot::Drivetrain::left(.30);
//   }
//   while (std::abs(target - Inertial10.heading(degrees)) > 5) {
//     wait(20, msec);
//   }
//   Robot::Drivetrain::right(0);
//   Robot::Drivetrain::left(0);
// }

// inline const void driveStraight(float pct) {
//   Robot::Drivetrain::right(pct);
//   Robot::Drivetrain::left(pct);
// }
// inline const void driveFor(float pct, int timeInMs) {
//   driveStraight(pct);
//   wait(timeInMs, msec);
//   // driveStraight(0);
// };
// inline const void driveForStop(float pct, int timeInMs) {
//   driveStraight(pct);
//   wait(timeInMs, msec);
//   driveStraight(0);
// };

// // roller side
// void leftAutonRoller() {
//   // into roller
//   driveForStop(0.3, 400);

//   Robot::Actions::roller();
// }
// void leftAuton3() {
//   leftAutonRoller();

//   driveFor(-0.3, 300);

//   // grab first disc
//   turning(165);
//   Robot::Actions::intake();
//   driveForStop(0.3, 400);
//   wait(1000, msec);

//   // aim towards low goal
//   turning(115);
//   driveFor(-0.3, 500);
//   Robot::Actions::stopIntake();
//   driveForStop(-0.3, 1000);

//   // shoot into low goal
//   Robot::Actions::shoot(Robot::GOAL::MY_TEAM);
//   wait(500, msec);
// }
// void leftAuton3Intake() {
//   leftAuton3();

//   // grab 3 stack
//   turning(220);
//   driveFor(0.6, 750);
//   driveStraight(0);
//   wait(500, msec);
//   Robot::Actions::intake();
//   driveFor(0.3, 1500);
//   driveStraight(0);
//   wait(2500, msec);
//   Robot::Actions::stopIntake();
// }

// void leftAuton6() {
//   leftAuton3Intake();
//   // shoot into low goal
//   turning(130);
//   Robot::Actions::shoot(Robot::GOAL::MY_TEAM);
// }

// constexpr float conversionCoefficent =
//     (Robot::Dimensions::driveGearRatio * 360) /
//     (2 * M_PI * Robot::Dimensions::driveWheelRadius);
// constexpr float inchesToDeg(float inches) {
//   return inches * conversionCoefficent;
// };

// void driveDistance(float pct, float inches) {
//   driveStraight(pct);
//   const float goalDeg = inchesToDeg(inches);
//   LeftDriveR.setPosition(0, degrees);
//   RightDriveR.setPosition(0, degrees);
//   while ((LeftDriveR.position(degrees) + RightDriveR.position(degrees) / 2) <
//          goalDeg)
//     wait(40, msec);
// }

// // non-roller side
// void rightAutonRoller() {
//   static constexpr float distance1 = 11.24 / 3;           // 17.27 inches
//   static constexpr float distance2 = 9.5 / (2.75 * M_PI); // 6 inches

//   LeftDriveA.setPosition(0, turns);
//   Robot::Drivetrain::left(0.2);
//   Robot::Drivetrain::right(0.2);
//   while (LeftDriveA.position(turns) < distance1)
//     wait(40, msec);
//   // printf("why neil be1\n");
//   turning2(80);
//   // printf("why neil be2\n");
//   LeftDriveA.setPosition(0, turns);
//   Robot::Drivetrain::left(0.2);
//   Robot::Drivetrain::right(0.2);
//   while (LeftDriveA.position(turns) < distance2)
//     wait(40, msec);
//   // printf("why neil be3\n");
//   driveStraight(0);
//   Robot::Actions::roller();
// }
// void rightAutonDiscs() {
//   rightAutonRoller();
//   wait(500, msec);
//   //  Robot::Drivetrain::left(-0.3);
//   // Robot::Drivetrain::right(-0.3);
//   // wait(500,msec);
//   // Robot::Drivetrain::left(0);
//   // Robot::Drivetrain::right(0);
//   driveForStop(-0.3, 500);
//   turning2(200);
//   Robot::Actions::intake();
//   // Robot::Drivetrain::left(0.6);
//   // Robot::Drivetrain::right(0.6);
//   // wait(1000, msec);
//   // Robot::Drivetrain::left(0);
//   // Robot::Drivetrain::right(0);
//   driveForStop(0.6, 1000);
//   wait(1000, msec);
//   Robot::Actions::stopIntake();
//   turning2(340);
//   Robot::Actions::shoot(Robot::GOAL::MY_TEAM);
//   // printf("why neil be4\n");
// }
// void skillsAuton() {
//   leftAuton6();

//   Robot::Actions::expand();
// }

void preAuton() {
  // using namespace auton;w
  // AutonSelection::start({leftAutonRoller, leftAuton3, leftAuton3Intake,
  //                        leftAuton6, rightAutonRoller, rightAutonDiscs,
  //                        doNothing, skillsAuton});
  // myton();
}
int main() {
  vexcodeInit();
  // return 1;
  motorSetup();

  Robot::team = Robot::TEAM::RED;

  Inertial10.calibrate(2);
  // wait until inertial finsished calibrating
  while (Inertial10.isCalibrating())
    wait(20, msec);

  OdomTracking tracker1 = {{}};
  tracker = &tracker1;

  // tracker->reset();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //    START COMPETITION
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~

  // Competition.autonomous(autonomous);
  Competition.drivercontrol(driverControl);
  preAuton();
  while (1) {
    wait(100, msec);
  }
  return 1;

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //    END COMPETITION
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~

  // drivetrain code
  // Controller1.Axis2.changed(&rightDriveSubscriber);
  // Controller1.Axis3.changed(&leftDriveSubscriber);

  // odometry

  // team

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //    START CATA TEST
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~

  // Catapult1.setVelocity(100, percent);

  Controller1.ButtonUp.pressed([]() {
    Catapult1.spin(fwd, 100, pct);
    while (Controller1.ButtonUp.pressing()) {
      wait(10, msec);
    }
    Catapult1.stop();
  });
  Controller1.ButtonDown.pressed([]() {
    Catapult1.spin(reverse, 100, pct);
    while (Controller1.ButtonDown.pressing()) {
      wait(10, msec);
    }
    Catapult1.stop();
  });

  // while(1) {
  //   // Brain.Screen.print(Catapult.pr
  //   printf("cata: %f\n", Catapult1.position(degrees));
  //   wait(20,msec);
  // }

  // return 1;

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //    END CATA TEST
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //    START AUTON TESTING
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~

  // auto x = []() {
  //   using namespace auton;
  //   Path({
  //            new GoTo(Robot::team == Robot::TEAM::BLUE
  //                         ? elements::ROLLER::BLUE_LEFT
  //                         : elements::ROLLER::RED_LEFT),
  //            //  new Roller(),
  //            new GoTo(Robot::team == Robot::TEAM::BLUE
  //                         ? elements::ROLLER::BLUE_RIGHT
  //                         : elements::ROLLER::RED_RIGHT),
  //            //  new Roller(),
  //            //  new GoTo(Robot::team == Robot::TEAM::BLUE
  //            //               ? elements::ROLLER::BLUE_LEFT
  //            //               : elements::ROLLER::RED_LEFT),
  //        })
  //       .execute();
  // };
  // tracker->reset(elements::ROLLER::LEFT);
  // Controller1.ButtonX.pressed([]() {
  //   []() {

  // });
  // (thread([]() {
  //   Position robotPos1;

  //   while (1) {
  //     Controller1.Screen.setCursor(0, 0);

  //     robotPos1 = tracker->getRobotPosition();
  //     Controller1.Screen.clearLine();
  //     Controller1.Screen.print("(");
  //     Controller1.Screen.print(robotPos1.x);
  //     Controller1.Screen.print(",");
  //     Controller1.Screen.print(robotPos1.y);
  //     Controller1.Screen.print(",");
  //     Controller1.Screen.print(robotPos1.heading);
  //     Controller1.Screen.print(")");
  //     Controller1.Screen.newLine();
  //     wait(20, msec);

  //     Brain.Screen.clearLine();
  //     Brain.Screen.print("(");
  //     Brain.Screen.print(robotPos1.x);
  //     Brain.Screen.print(",");
  //     Brain.Screen.print(robotPos1.y);
  //     Brain.Screen.print(",");
  //     Brain.Screen.print(robotPos1.heading);
  //     Brain.Screen.print(")");
  //     Brain.Screen.newLine();
  //     wait(20, msec);
  //   }
  // }));
  // Controller1.ButtonX.pressed([]() {
  //   auton::GoTo goTo = {{}};
  //   goTo.execute();
  // });

  // return 1;
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //    END AUTON TESTING
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~
}

void controllerDisplay(/* OdomTracking tracker1 */) {
  Brain.Screen.clearScreen();
  // tracker = &tracker1;
  // Catapult1.setPosition(0, deg);
  // Controller1.ButtonA.pressed([]() { Catapult1.setPosition(0, deg); });

  while (1) {
    // reset
    Controller1.Screen.setCursor(0, 0);

    // shots
    // Controller1.Screen.clearLine();
    // Controller1.Screen.print("shots: ");
    // Controller1.Screen.print(shots);
    // Controller1.Screen.newLine();
    // PTO State
    // Controller1.Screen.clearLine();
    // if (PTOState == PTO::INTAKE)
    //   Controller1.Screen.print("intake");
    // else
    //   Controller1.Screen.print("drive");
    // Controller1.Screen.newLine();

    // // Deez Nuts
    // Controller1.Screen.clearLine();
    // Controller1.Screen.print(/* "deez nuts" */ tracker.deltaPos1.x);
    // // Controller1.Screen.print(/* "deez nuts" */
    // tracker.data.curr.pos.x);

    // temp
    Controller1.Screen.clearLine();
    Controller1.Screen.print("drive: ");
    Controller1.Screen.print(
        (Robot::Drivetrain::Left->temperature(fahrenheit) +
         Robot::Drivetrain::Right->temperature(fahrenheit)) /
        2);
    Controller1.Screen.print(" F");
    Controller1.Screen.newLine();

    Controller1.Screen.clearLine();
    Controller1.Screen.print("cata: ");
    Controller1.Screen.print(Catapult1.temperature(fahrenheit));
    Controller1.Screen.print(" F");
    Controller1.Screen.newLine();

    // tracking
    Position robotPos1 = Robot::getPosition();
    // printf("neil!: %f\n", robotPos1.heading);
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

    // printf("cata: %f\n", Catapult1.position(degrees));

    wait(50, msec);
  }
}