#include "auton/action.h"
#include "conversions.h"
void auton::Roller::execute() const { Robot::Actions::roller(); };
void auton::Intake::execute() const { Robot::Actions::intake(); };
void auton::StopIntake::execute() const { Robot::Actions::stopIntake(); };
void auton::Expand::execute() const { Robot::Actions::expand(); };
void auton::Shoot::execute() const { Robot::Actions::shoot(goal); };
auton::Shoot::Shoot(const Robot::GOAL goal) : goal(goal){};
void auton::PTO::execute() const { Robot::Actions::pto(state); };
auton::PTO::PTO(const Robot::PTO_STATE goal) : state(state){};
constexpr float pi2 = 2 * M_PI;
inline constexpr float headingDifference(float a, float b) {
  const float diff = a - b;
  return (diff +
          pi2 * (diff > 0 ? (diff > M_PI ? -1 : 0) : diff < -M_PI ? 1 : 0));
}
void auton::GoTo::execute() const {
  static const float pidLimit = 50;
  while (abs(Position::distance({0,0}, Robot::getPosition())) > 0.1) {
    if (Controller1.ButtonA.pressing())
      break;

    const float distance = Position::distance({0,0}, Robot::getPosition());
    const Position vector = Position::difference({0,0,0}, Robot::getPosition());
    printf(")distance:%f\n", distance);
    const float target1 = atan2(vector.x, vector.y);
    const float target = target1 < 0 ? target1 + pi2 : target1;
    const float heading1 =
        Conversions::Degrees::toRadians(Inertial10.heading(degrees));
    const float headingDiff = headingDifference(target, heading1) /*  * 100 */;
    printf("{heading: %f\n+diff: %f\n@target: %f\n&input: %f\n", heading1,
           headingDiff /*  / 100 */, target, 1);
    const float stopAdjust =
        pow(cos(headingDiff), 5) * 40 * distance / (2 + abs(distance));
    const float turnAdjust = (20 * headingDiff / (0.5 + abs(heading1)));

    const float leftAdjustment = stopAdjust - turnAdjust;
    const float rightAdjustment = stopAdjust + turnAdjust;

    printf("(%f,%f,%f)", Robot::getPosition().x, Robot::getPosition().y,
           Robot::getPosition().heading);

        if (abs(leftAdjustment) < pidLimit)
            Robot::Drivetrain::left(leftAdjustment);
    else {
      printf("left at limit: %f\n", leftAdjustment);
      Robot::Drivetrain::left(leftAdjustment > 0 ? pidLimit : -pidLimit);
    }
    if (abs(rightAdjustment) < pidLimit)
      Robot::Drivetrain::right(rightAdjustment);
    else {
      printf("right at limit: %f\n", rightAdjustment);
      Robot::Drivetrain::right(rightAdjustment > 0 ? pidLimit : -pidLimit);
    };
    Controller1.Screen.setCursor(0, 0);

    static Position robotPos1 = Robot::getPosition();
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
  }
};
auton::GoTo::GoTo(const Position target) : target(target){};
void auton::Wait::execute() const { wait(time, msec); };
auton::Wait::Wait(const int time) : time(time){};
void auton::Path::execute() const {
  for (auto i = actions.begin(); i != actions.end(); i++)
    (*i)->execute();
};
auton::Path::Path(const std::vector<const Action *> actions)
    : actions(actions){};