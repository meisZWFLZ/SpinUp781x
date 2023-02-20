#include "auton/autons.h"
#include "auton/playback.h"
#include "conversions.h"
#include "robot.h"
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
  const float goalDeg = inchesToDeg(inches);
  LeftDriveR.setPosition(0, degrees);
  RightDriveR.setPosition(0, degrees);
  while ((LeftDriveR.position(degrees) + RightDriveR.position(degrees) / 2) <
         goalDeg)
    wait(40, msec);
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

  Robot::Actions::goTo({84, -96, 0});

  // Robot::Drivetrain::left(0.2);
  // Robot::Drivetrain::right(-0.2);
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
  leftAuton3In();
  turning2(215);
  driveForStop(-0.5, 3000);
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
void leftAuton3Disc() {
  leftAuton3Disc();

  // grab 3 stack
  driveForStop(0.3, 200);
  Robot::Actions::roller();
  wait(200, msec);
  driveForStop(-0.3, 500);
  turning2(0.3, 350);
  driveForStop(0.5, 200);
  Robot::Actions::intake();
 wait(200, msec);
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

// non-roller side
void rightAutonRoller() {
  static Player *player = new Player("b.txt");
  player->start();
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

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//           Skills
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void skillsAuton() {
  // static constexpr int pidLimit = 50;
  // static constexpr float target = Conversions::Degrees::toRadians(225);

  leftAutonRoller();
  driveForStop(0.3, 700);
  turning2(225 - 180 - 10);
  driveForStop(-0.3, 500);
  Robot::Actions::expand();

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

void doNothing() { Brain.Screen.print("i should be doing nothing"); }
const std::vector<std::pair<const char *, const std::function<void(void)>>>
    autons = {{"Left Roller", leftAutonRoller},
              {"Left R 3Low", leftAuton3Low},
              {"Left 2R 3In", leftAuton2Roller},
              {"Left R 3Low 3In", leftAuton3Low3In},
              {"Left R 6Low", leftAuton6},
              {"Right Roller", rightAutonRoller},
              {"Right R 3Low", rightAutonDiscs},
              {"Do Nothing", doNothing},
              {"Skills", skillsAuton}};
}; // namespace auton