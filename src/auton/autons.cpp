#include "auton/autons.h"
#include "./auton/path.h"
#include "auton/elements.h"
#include "auton/playback.h"
#include "conversions.h"
#include "coordinate.h"
#include "robot.h"
#include "stdio.h"
#include "vex_units.h"
#include <cmath>

namespace auton {

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//           Utility
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static constexpr float pi2 = 2 * M_PI;
constexpr float headingDifference(float a, float b) {
  const float diff = a - b;
  return (diff + pi2 * (diff > 0       ? (diff > M_PI ? -1 : 0)
                        : diff < -M_PI ? 1
                                       : 0));
}

// ============
//   TURNING

// turn clockwise
void turning(int target) {
  Robot::Drivetrain::right(.30);
  Robot::Drivetrain::left(-.30);
  while (std::abs(target - Inertial10.heading(degrees)) > 5) {
    wait(20, msec);
  }
  Robot::Drivetrain::right(0);
  Robot::Drivetrain::left(0);
}
// turn in the fastest direction
void turning2(int target) {
  if (headingDifference(Inertial10.heading(degrees), target + 90) >
      headingDifference(Inertial10.heading(degrees), target - 90)) {
    Robot::Drivetrain::right(.30);
    Robot::Drivetrain::left(-.30);
  } else {
    Robot::Drivetrain::right(-.30);
    Robot::Drivetrain::left(.30);
  }
  while (std::abs(target - Inertial10.heading(degrees)) > 5) {
    wait(20, msec);
  }
  Robot::Drivetrain::right(0);
  Robot::Drivetrain::left(0);
}

// ============
//   DRIVING

inline const void driveStraight(float pct) {
  Robot::Drivetrain::right(pct);
  Robot::Drivetrain::left(pct);
}
// drive for some time, dont stop
inline const void driveFor(float pct, int timeInMs) {
  driveStraight(pct);
  wait(timeInMs, msec);
  // driveStraight(0);
};
// drive for some time, then stop
inline const void driveForStop(float pct, int timeInMs) {
  driveStraight(pct);
  wait(timeInMs, msec);
  driveStraight(0);
};

constexpr float conversionCoefficent =
    (Robot::Dimensions::driveGearRatio * 360) /
    (2 * M_PI * Robot::Dimensions::driveWheelRadius);
// find how many degrees wheels must be spun to go this distance
constexpr float inchesToDeg(float inches) {
  return inches * conversionCoefficent;
};
// drive some amount of inches
void driveDistance(float pct, float inches) {
  driveStraight(pct);
  auto startPos = Robot::getPosition();
  // const float goalDeg = inchesToDeg(inches);
  // LeftDriveR.setPosition(0, degrees);
  // RightDriveR.setPosition(0, degrees);
  while (/* (LeftDriveR.position(degrees) + RightDriveR.position(degrees)) / 2 <
         goalDeg */
         Position::distance(startPos, Robot::getPosition()) < inches)
    wait(40, msec);
  driveStraight(0);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//          Left Side
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// roller side
void leftAutonRoller() {
  // into roller
  // driveForStop(0.3, 400);
  // driveFor(-0.15, 500);
  // Robot::Actions::roller();
  // driveStraight(0);

  // Robot::Actions::goTo({84, -96, 0}, 1);
  printf("start\n");
  Robot::Drivetrain::left(0.5);
  Robot::Drivetrain::right(-0.5);
}
void leftAuton3In() {
  leftAutonRoller();
  driveFor(0.2, 200);

  if (!Inertial10.installed())
    return;

  // grab first disc
  // turning(165);
  turning(335);
  Robot::Actions::intake();
  driveForStop(0.2, 500);
  wait(500, msec);
  Robot::Actions::stopIntake();
  wait(50, msec);
  Robot::Actions::intake();
  wait(500, msec);
  driveForStop(-0.3, 400);
}
void leftAuton2Roller() {
  Robot::setPosition({0, 0, Conversions::Degrees::toRadians(90)});

  driveFor(0.4, 200);
  Robot::Actions::roller();
  wait(200, msec);
  driveFor(0.0, 0);
  driveForStop(-0.4, 400);
  turning2(237);
  Robot::Actions::pistonShoot(Robot::GOAL::MY_TEAM);
  wait(1000, msec);
  turning2(297);
  Robot::Actions::intake();
  driveForStop(0.6, 700);
  Robot::Actions::intake();
  wait(1500, msec);
  Robot::Actions::stopIntake();
  turning2(208);
  Robot::Actions::pistonShoot(Robot::GOAL::MY_TEAM);
}
void leftAuton3Low() {
  leftAuton3In();
  // aim towards low goal
  turning(115);
  // Robot::Actions::stopIntake();
  driveFor(-0.3, 500);
  driveForStop(-0.3, 1000);

  // shoot into low goal
  Robot::Actions::shoot(Robot::GOAL::MY_TEAM);
  wait(500, msec);
}
void leftAuton3Low3In() {
  leftAuton3Low();

  // grab 3 stack
  turning(220);
  driveFor(0.6, 750);
  driveStraight(0);
  wait(500, msec);
  Robot::Actions::intake();
  driveFor(0.3, 1500);
  driveStraight(0);
  wait(2500, msec);
  Robot::Actions::stopIntake();
}

void leftAuton6() {
  leftAuton3Low3In();
  // shoot into low goal
  turning(130);
  Robot::Actions::shoot(Robot::GOAL::MY_TEAM);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//          Right Side
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void driveForwardUntilStopped(float speed, float allowedDistance,
                              float maxTime = 350) {
  driveStraight(speed);

  Coordinate lastPos = Robot::getPosition();
  auto startTime = vex::timer::system();
  wait(500, msec);
  while (Coordinate::distance(lastPos, Robot::getPosition()) >
             allowedDistance &&
         vex::timer::system() - startTime < maxTime) {
    lastPos = Robot::getPosition();
    wait(100, msec);
  }

  driveStraight(0);
};

void turnTowardsRedGoal(const float allowedDifference = 0.05,
                        const float addToTarget = 0) {
  Robot::Actions::stopIntake();

  const NormalPosition vector =
      NormalPosition::difference(elements::GOAL::RED, Robot::getPosition());
  const float target1 = atan2(vector.x, vector.y);
  const float target = target1 < 0 ? target1 + pi2 : target1;

  printf("target: %f\n", target);
  printf("target1: %f\n", target1);
  printf("heading: %f\n", Robot::getPosition().heading);

  // turning2(20);
  Robot::Actions::turnTo(
      target + addToTarget /* Conversions::Degrees::toRadians(13) */,
      allowedDifference);

  Robot::Drivetrain::stop();

  wait(250, msec);
}

/**
 * !! intake continues to spins after function !!
 * */
void right3Discs() {
  Robot::setPosition({84, 18 - (16.25 / 2), 0});

  Robot::Actions::intake();
  driveDistance(.3, 20);

  printf("heading: %f\n", Robot::getPosition().heading);
  // driveFor(-0.3, 500);
  // driveFor(0.3, 500);

  // Robot::Actions::turnTo(Conversions::Degrees::toRadians(55));
  // driveDistance(.4, 2.5);
  turnTowardsRedGoal(Conversions::Degrees::toRadians(1 /*deg*/));
  // shoot
  driveFor(-0.5, 550);
  driveStraight(0.6);
  wait(200, msec);
  // Robot::Actions::pistonShoot(Robot::GOAL::RED);
  wait(100, msec);
  Robot::Drivetrain::stop();
}
// non-roller side
void rightAutonRoller() {
  right3Discs();
  thread intakeThread([] {
    auto startTime = vex::timer::system();
    while (vex::timer::system() - startTime < 2000 && Intake.voltage() < 1) {
      Robot::Actions::intake();
      wait(10, msec);
    }
  });

  wait(500, msec);
  // Robot::Actions::turnTo(Conversions::Degrees::toRadians(270 /*deg*/));
  // driveDistance(0.5, 1);

  printf("heading: %f\n",
         Conversions::Radians::toDegrees(Robot::getPosition().heading));

  // Robot::Actions::turnTo(Conversions::Degrees::toRadians(0));

  wait(800, msec);

  // // 2nd volley
  // // 1st disc

  // Robot::Actions::intake();
  // driveDistance(0.3, 8.4);
  // // wait(400, msec);
  // // Robot::Drivetrain::setStopping(hold);

  // // Robot::Drivetrain::stop();
  // // Robot::Drivetrain::setStopping(coast);

  // Robot::Drivetrain::left(-0.5);
  // Robot::Drivetrain::right(-0.5);
  // wait(400, msec);
  // driveStraight(0);
  // // Robot::Drivetrain::stop();
  // // wait(100, msec);

  // // next 2 discs
  // Robot::Actions::goTo({72, 48}, 0.75);
  // wait(400, msec);
  // Robot::Actions::goTo({60, 60}, 0.75);
  // wait(400, msec);

  // return;

  // roller

  // go near to roller
  Robot::Actions::goTo({111, 12}, 1.2);
  Robot::Actions::stopIntake();

  // face into roller
  Robot::Actions::turnTo(Conversions::Degrees::toRadians(180 /*deg*/));
  // drive into roller
  driveForwardUntilStopped(0.3, 0.2, 750);

  // apply pressure
  Robot::Drivetrain::left(0.1);

  intakeThread.interrupt();

  Robot::Actions::roller();
  Robot::Drivetrain::left(0);

  // static Player *player = new Player("b.txt");
  // player->start();
  // static constexpr float distance1 = 11.24 / 3;           // 17.27 inches
  // static constexpr float distance2 = 9.5 / (2.75 * M_PI); // 6 inches

  // LeftDriveA.setPosition(0, turns);
  // Robot::Drivetrain::left(0.2);
  // Robot::Drivetrain::right(0.2);
  // while (LeftDriveA.position(turns) < distance1)
  //   wait(40, msec);
  // // printf("why neil be1\n");
  // turning2(280);
  // // printf("why neil be2\n");
  // LeftDriveA.setPosition(0, turns);
  // Robot::Drivetrain::left(-0.2);
  // Robot::Drivetrain::right(-0.2);
  // while (LeftDriveA.position(turns) < distance2)
  //   wait(40, msec);
  //   wait(500, msec);
  // // printf("why neil be3\n");
  // driveStraight(-0.15);
  // Robot::Actions::roller();
  // driveStraight(0);
}

void right8Discs() {
  right3Discs(); // intake continues to run after ward
  wait(700, msec);
  // face towards next discs
  Robot::Actions::turnTo(Conversions::Degrees::toRadians(360 - 45 /*deg*/),
                         Conversions::Degrees::toRadians(30 /*deg*/));
  // intake 2 discs
  Robot::Actions::goTo({48, 72}, 2);
  wait(300, msec);

  // aim 2nd volley
  turnTowardsRedGoal(Conversions::Degrees::toRadians(1 /*deg*/));
  Robot::Drivetrain::stop();
  wait(300, msec);

  // shoot 2nd volley of 2
  Robot::Actions::pistonShoot(Robot::GOAL::RED);
  wait(500, msec);

  // face 3 stack
  Robot::Actions::turnTo(Conversions::Degrees::toRadians(360 - 45 /*deg*/),
                         Conversions::Degrees::toRadians(30 /*deg*/));
  // wait until cata is down to intake
  thread intakeThread([] {
    auto startTime = vex::timer::system();
    while (vex::timer::system() - startTime < 2000 && Intake.voltage() < 1) {
      Robot::Actions::intake();
      wait(10, msec);
    }
  });
  // intake 3 stack
  Robot::Actions::goTo({32, 88}, 2);

  // aim 3rd volley
  turnTowardsRedGoal(Conversions::Degrees::toRadians(1 /*deg*/));
  Robot::Drivetrain::stop();
  wait(300, msec);

  // shoot 3rd volley of 3 discs
  driveFor(-0.5, 500);
  driveStraight(0.6);
  Robot::Actions::stopIntake();
  wait(200, msec);
  Robot::Actions::pistonShoot(Robot::GOAL::RED);
  wait(100, msec);
  Robot::Drivetrain::stop();
}
void rightAutonDiscs() {
  rightAutonRoller();
  wait(500, msec);
  //  Robot::Drivetrain::left(-0.3);
  // Robot::Drivetrain::right(-0.3);
  // wait(500,msec);
  // Robot::Drivetrain::left(0);
  // Robot::Drivetrain::right(0);
  driveForStop(-0.3, 500);
  turning2(200);
  Robot::Actions::intake();
  // Robot::Drivetrain::left(0.6);
  // Robot::Drivetrain::right(0.6);
  // wait(1000, msec);
  // Robot::Drivetrain::left(0);
  // Robot::Drivetrain::right(0);
  driveForStop(0.6, 1000);
  wait(1000, msec);
  Robot::Actions::stopIntake();
  turning2(340);
  Robot::Actions::shoot(Robot::GOAL::MY_TEAM);
  // printf("why neil be4\n");
}

void awp() {
  Robot::setPosition({12, 110, 270});
  Inertial10.setHeading(270, deg);

  // driveForwardUntilStopped(0.3, 0.2);
  driveForStop(0.25, 300);
  Robot::Drivetrain::right(0.1);

  Robot::Actions::roller();
  Robot::Drivetrain::stop();

  // reset for straight intake
  printf("drive\n");
  driveForStop(-0.3, 400);
  printf("drive stop\n");
  // Robot::Actions::goTo({12, 108}, 1);
  Robot::Actions::turnTo(Conversions::Degrees::toRadians(135 /*deg*/),
                         Conversions::Degrees::toRadians(30 /*deg*/));

  // intake 3 stack
  Robot::Actions::intake();
  Robot::Actions::goTo({52, 68}, 1.75, 0.5, 35);
  // driveDistance(0.4, 4.24264068712 * 12);
  Robot::Drivetrain::stop();

  // return;

  // shoot
  turnTowardsRedGoal(Conversions::Degrees::toRadians(1 /*deg*/),
                     Conversions::Degrees::toRadians(2.5 /*deg*/));
  driveForStop(-0.5, 400);
  Robot::Actions::stopIntake();
  driveStraight(0.6);
  wait(200, msec);
  Robot::Actions::pistonShoot(Robot::GOAL::RED);
  wait(150, msec);
  Robot::Drivetrain::stop();
  wait(1000, msec);

  // let cata retract, then start intake
  thread([] {
    auto startTime = vex::timer::system();
    while (vex::timer::system() - startTime < 2000 && Intake.voltage() < 1) {
      Robot::Actions::intake();
      wait(10, msec);
    }
  });

  // go to right roller
  Robot::Actions::goTo({115, 12}, 1.5, 1, 50);
  Robot::Actions::stopIntake();

  // face roller
  Robot::Actions::turnTo(Conversions::Degrees::toRadians(180 /*deg*/),
                         Conversions::Degrees::toRadians(5 /*deg*/));
  // go into roller
  driveForwardUntilStopped(0.5, 0.2);

  Robot::Drivetrain::stop();
  // apply pressure
  Robot::Drivetrain::left(0.1);

  Robot::Actions::roller();
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//           Skills
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void skills2() {
  Robot::setPosition({12, 110, 270});
  Inertial10.setHeading(270, deg);

  // driveForwardUntilStopped(0.3, 0.2);
  driveForStop(0.25, 300);
  Robot::Drivetrain::right(0.1);

  Robot::Actions::roller();
  Robot::Drivetrain::stop();

  wait(1000, msec);

  driveForStop(-0.3, 300);
  wait(1000, msec);

  Robot::Actions::intake();
  Robot::Actions::goTo({30, 120}, 1.5);
  wait(1000, msec);

  Robot::Actions::turnTo(0, Conversions::Degrees::toRadians(5 /*deg*/));
  Robot::Drivetrain::stop();
  wait(1000, msec);
  driveForwardUntilStopped(0.3, 0.2);

  Robot::Drivetrain::right(0.1);

  Robot::Actions::roller();
  Robot::Drivetrain::stop();

  wait(1000, msec);

  driveForStop(-0.3, 300);
  wait(1000, msec);

  // Robot::Actions::intake();
  Robot::Actions::turnTo(Conversions::Degrees::toRadians(45 /*deg*/),
                         Conversions::Degrees::toRadians(15 /*deg*/));
  wait(1000, msec);

  Robot::Actions::goTo({24, 120}, 2);
  wait(1000, msec);

  Robot::Actions::turnTo(Conversions::Degrees::toRadians(360 - 45 /*deg*/),
                         Conversions::Degrees::toRadians(2 /*deg*/));
  Robot::Drivetrain::stop();
  Robot::Actions::stopIntake();
  wait(1000, msec);

  Robot::Actions::expand();
}
void skillsAuton() {
  // static constexpr int pidLimit = 50;
  // static constexpr float target = Conversions::Degrees::toRadians(225);

  awp();

  // back out
  driveForStop(-0.3, 400);
  const NormalPosition curRoboPos = Robot::getPosition();
  Robot::setPosition(
      {curRoboPos.x, curRoboPos.y,
       Conversions::Degrees::toRadians(/*deg*/ Inertial10.heading(deg))});
  // go 3rd near roller
  Robot::Actions::goTo({132, 36}, 1.5);
  // face 3rd roller
  Robot::Actions::turnTo(Conversions::Degrees::toRadians(90 /*deg*/),
                         Conversions::Degrees::toRadians(5 /*deg*/));
  Robot::Drivetrain::stop();
  wait(1000, msec);

  // drive into 3rd roller
  driveForwardUntilStopped(0.3, 0.2);
  // apply pressure to 3rd roller
  Robot::Drivetrain::right(0.1);
  Robot::Actions::roller();

  // back out of 3rd roller
  driveForStop(-0.3, 400);

  // // // go near red goal
  // // Robot::Actions::goTo({132, 84}, 1.5);
  // // Robot::Drivetrain::stop();
  // // wait(300, msec);

  // // // aim 2nd volley
  // // turnTowardsRedGoal();
  // // Robot::Drivetrain::stop();
  // // wait(500, msec);
  // // Robot::Actions::shoot(Robot::GOAL::RED);
  // // wait(700, msec);

  // // // face 1st disc
  // // Robot::Actions::turnTo(Conversions::Degrees::toRadians(270 /*deg*/),
  // //                        Conversions::Degrees::toRadians(30 /*deg*/));
  // // // intake first disc
  // // thread intakeThread([] {
  // //   auto startTime = vex::timer::system();
  // //   while (vex::timer::system() - startTime < 2000 && Intake.voltage() <
  // 1) {
  // //     Robot::Actions::intake();
  // //     wait(10, msec);
  // //   }
  // // });
  // // Robot::Actions::goTo({96, 96}, 1.5);

  // // // face next 2 discs
  // // Robot::Actions::turnTo(Conversions::Degrees::toRadians(360 - 45
  // /*deg*/),
  // //                        Conversions::Degrees::toRadians(15 /*deg*/));
  // // Robot::Actions::goTo({48, 120}, 1);

  // // go to 4th roller
  // Robot::Actions::goTo({36, 132}, 1);
  // // face 4th  roller
  // Robot::Actions::turnTo(Conversions::Degrees::toRadians(0 /*deg*/),
  //                        Conversions::Degrees::toRadians(5 /*deg*/));
  // driveForwardUntilStopped(0.3, 0.2);
  // Robot::Actions::roller();

  // prep for expand
  Robot::Actions::goTo({120, 24}, 1.5);

  // aim expansion
  Robot::Actions::turnTo(Conversions::Degrees::toRadians(90 + 45 /*deg*/),
                         Conversions::Degrees::toRadians(5 /*deg*/));
  Robot::Drivetrain::stop();
  wait(500, msec);

  // Robot::Actions::expand();
  printf("expand");

  // leftAutonRoller();
  // driveForStop(0.3, 700);
  // turning2(225 - 180 - 10);
  // driveForStop(-0.3, 500);

  // float robotHeading;
  // float headingDiff = headingDifference(target, Inertial10.heading());

  // while (std::abs(headingDiff) > 0.1) {
  //   // if (Controller1.ButtonA.pressing())
  //   //   break;

  //   headingDiff = headingDifference(target, Inertial10.heading()) /*  * 100
  //   */;

  //   // printf("{heading: %f\n+diff: %f\n@target: %f\n", heading1,
  //   //        headingDiff /*  / 100 */, target);
  //   // pow(cos(headingDiff), 5) * 40 * distance / (2 + abs(distance));
  //   const float turnAdjust = (20 * headingDiff / (0.5 +
  //   std::abs(headingDiff)));

  //   const float leftAdjustment = +turnAdjust;
  //   const float rightAdjustment = -turnAdjust;

  //   // printf("(%f,%f,%f)", robotPos.x, robotPos.y, robotPos.heading);

  //   if (std::abs(leftAdjustment) < pidLimit) {
  //     Robot::Drivetrain::left(leftAdjustment);
  //     Robot::Drivetrain::right(rightAdjustment);
  //   } else {
  //     printf("both at limit: %f\n", leftAdjustment);
  //     Robot::Drivetrain::left(leftAdjustment > 0 ? pidLimit : -pidLimit);
  //     Robot::Drivetrain::right(rightAdjustment > 0 ? pidLimit : -pidLimit);
  //   }

  //   // Controller1.Screen.setCursor(0, 0);
  //   // Controller1.Screen.clearLine();
  //   // Controller1.Screen.print("(");
  //   // Controller1.Screen.print(robotPos.x);
  //   // Controller1.Screen.print(",");
  //   // Controller1.Screen.print(robotPos.y);
  //   // Controller1.Screen.print(",");
  //   // Controller1.Screen.print(robotPos.heading);
  //   // Controller1.Screen.print(")");
  //   // Controller1.Screen.newLine();

  //   wait(20, msec);

  // robotPos = Robot::getPosition();
  // distance = Position::distance(pos, robotPos);
  // }
  // Robot::Drivetrain::left(0);
  // Robot::Drivetrain::right(0);

  // Robot::Actions::expand();
} // namespace auton

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//     Array Initialization
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Auton::Auton(const char *name) : name(name){};

CallbackAuton::CallbackAuton(const char *name, void (*callback)(void))
    : Auton(name), callback(callback) {}

void CallbackAuton::execute() const { callback(); }

void doNothing() { Brain.Screen.print("i should be doing nothing"); }
const std::vector<const Auton *> autons = {
    new CallbackAuton("Right Roller", rightAutonRoller),
    new CallbackAuton("AWP", awp),
    // new auton::Path("AutoGUI/brandonerd.vauto", "right roll gui"),
    new CallbackAuton("Skills2", skills2),
    new CallbackAuton("neilton (nerd)", leftAuton2Roller),
    new CallbackAuton("YO MOTHER", right8Discs),
    // new CallbackAuton("Left Roller", leftAutonRoller),
    // new CallbackAuton("Left R 3Low", leftAuton3Low),
    // new CallbackAuton("Left R 3Low 3In", leftAuton3Low3In),
    // new CallbackAuton("Left R 6Low", leftAuton6),
    // new CallbackAuton("Right R 3Low", rightAutonDiscs),
    new CallbackAuton("Do Nothing", doNothing),
    new CallbackAuton("Skills", skillsAuton),
};
}; // namespace auton