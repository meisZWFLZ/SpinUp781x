#include "auton/action.h"
#include "conversions.h"
#include "v5_color.h"
#include <cmath>

void auton::Roller::execute() const { Robot::Actions::roller(); };
void auton::Intake::execute() const { Robot::Actions::intake(); };
void auton::StopIntake::execute() const { Robot::Actions::stopIntake(); };
void auton::Expand::execute() const { Robot::Actions::expand(); };
void auton::Shoot::execute() const { Robot::Actions::shoot(goal); };
auton::Shoot::Shoot(const Robot::GOAL goal) : goal(goal){};
// void auton::PTO::execute() const { Robot::Actions::pto(state); };
// auton::PTO::PTO(const Robot::PTO_STATE state) : state(state){};
constexpr float pi2 = 2 * M_PI;
inline constexpr float headingDifference(float a, float b) {
  const float diff = a - b;
  return (diff + pi2 * (diff > 0       ? (diff > M_PI ? -1 : 0)
                        : diff < -M_PI ? 1
                                       : 0));
}

const void Robot::Actions::goTo(const Position pos) {
  static const float pidLimit = .5;
  Position robotPos = Robot::getPosition();
  float distance = Position::distance(pos, Robot::getPosition());
  float headingDiff = 0;
  float lastDistance = Position::distance(pos, Robot::getPosition());
  while (distance > 1) {
    // if (Controller1.ButtonA.pressing())
    //   break;

    const Position vector = Position::difference(pos, robotPos);
    printf(")distance:%f\n", distance);
    const float target1 = atan2(vector.x, vector.y);
    const float target = target1 < 0 ? target1 + pi2 : target1;
    const float heading1 = robotPos.heading;
    headingDiff = headingDifference(target, heading1) /*  * 100 */;

    printf("{heading: %f\n+diff: %f\n@target: %f\n", heading1,
           headingDiff /*  / 100 */, target);
    const float stopAdjust = /* 0; */
        pow(cos(headingDiff), 21) * 30 * distance / (2 + std::abs(distance));
    const float turnAdjust = (25 * headingDiff / (0.3 + std::abs(headingDiff)));

    const float leftAdjustment = (stopAdjust + turnAdjust) / 100;
    const float rightAdjustment = (stopAdjust - turnAdjust) / 100;
    printf("(%f,%f,%f)\n", robotPos.x, robotPos.y, robotPos.heading);
    printf("<left%f\n>right%f\n", leftAdjustment, rightAdjustment);

    // if (std::abs(leftAdjustment) < pidLimit)
    //   Robot::Drivetrain::left(leftAdjustment);
    // else {,
    //   printf("left at limit: %f\n", leftAdjustment);
    //   Robot::Drivetrain::left(leftAdjustment > 0 ? pidLimit : -pidLimit);
    // }
    Robot::Drivetrain::left(
        std::max(std::min(leftAdjustment, pidLimit), -pidLimit));
    // if (std::abs(rightAdjustment) < pidLimit)
    //   Robot::Drivetrain::right(rightAdjustment);
    // else {
    //   printf("right at limit: %f\n", rightAdjustment);
    //   Robot::Drivetrain::right(rightAdjustment > 0 ? pidLimit : -pidLimit);
    // };
    Robot::Drivetrain::right(
        std::max(std::min(rightAdjustment, pidLimit), -pidLimit));

    Controller1.Screen.setCursor(0, 0);
    Controller1.Screen.clearLine();
    Controller1.Screen.print("(");
    Controller1.Screen.print(robotPos.x);
    Controller1.Screen.print(",");
    Controller1.Screen.print(robotPos.y);
    Controller1.Screen.print(",");
    Controller1.Screen.print(robotPos.heading);
    Controller1.Screen.print(")");
    Controller1.Screen.newLine();

    lastDistance = distance;
    printf("derivative:%f\n", lastDistance - distance);
    wait(50, msec);

    robotPos = Robot::getPosition();
    distance = Position::distance(pos, robotPos);
  }
  // return;
  // headingDiff = headingDifference(pos.heading, robotPos.heading);
  // while (std::abs(headingDiff) > 0.1) {
  //   // if (Controller1.ButtonA.pressing())
  //   //   break;

  //   const float target = pos.heading;
  //   const float heading1 = robotPos.heading;
  //   headingDiff = headingDifference(target, heading1) /*  * 100 */;

  //   printf("{heading: %f\n+diff: %f\n@target: %f\n", heading1,
  //          headingDiff /*  / 100 */, target);
  //   // pow(cos(headingDiff), 5) * 40 * distance / (2 + abs(distance));
  //   const float turnAdjust = (20 * headingDiff / (0.5 +
  //   std::abs(headingDiff)));

  //   const float leftAdjustment = +turnAdjust / 100;
  //   // const float rightAdjustment = -turnAdjust / 100;

  //   const float limitedAdjustment =
  //       std::max(std::min(leftAdjustment, pidLimit), -pidLimit);
  //   Robot::Drivetrain::left(limitedAdjustment);
  //   Robot::Drivetrain::right(-limitedAdjustment);

  //   printf("(%f,%f,%f)", robotPos.x, robotPos.y, robotPos.heading);

  //   // if (std::abs(leftAdjustment) < pidLimit) {
  //   //   Robot::Drivetrain::left(leftAdjustment);
  //   //   Robot::Drivetrain::right(rightAdjustment);
  //   // } else {
  //   //   printf("both at limit: %f\n", leftAdjustment);
  //   //   Robot::Drivetrain::left(leftAdjustment > 0 ? pidLimit : -pidLimit);
  //   //   Robot::Drivetrain::right(rightAdjustment > 0 ? pidLimit :
  //   -pidLimit);
  //   // }

  //   Controller1.Screen.setCursor(0, 0);
  //   Controller1.Screen.clearLine();
  //   Controller1.Screen.print("(");
  //   Controller1.Screen.print(robotPos.x);
  //   Controller1.Screen.print(",");
  //   Controller1.Screen.print(robotPos.y);
  //   Controller1.Screen.print(",");
  //   Controller1.Screen.print(robotPos.heading);
  //   Controller1.Screen.print(")");
  //   Controller1.Screen.newLine();

  //   wait(20, msec);

  //   robotPos = Robot::getPosition();
  //   distance = Position::distance(pos, robotPos);
  // }
  Robot::Drivetrain::left(0);
  Robot::Drivetrain::right(0);
}
void auton::GoTo::execute() const { Robot::Actions::goTo(pos); };
auton::GoTo::GoTo(const Position target) : pos(target){};
void auton::Wait::execute() const { wait(time, msec); };
auton::Wait::Wait(const int time) : time(time){};
void auton::Path::execute() const {
  for (auto i = actions.begin(); i != actions.end(); i++)
    (*i)->execute();
};
auton::Path::Path(const std::vector<const Action *> actions)
    : actions(actions){};