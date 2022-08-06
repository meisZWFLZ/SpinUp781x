#include <cmath>

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

  void goTo(){
      // move robot to position
  };
};

namespace Actions {
namespace {
void shootRed(){};
void shootBlue(){};
void shoot(){};
void intake(){};
void intake3(){};
void roller(){};
void move(){};
void (*actionArr[])() = {shootRed, shootBlue, shoot, intake,
                         intake3,  roller,    move};
} // namespace
enum ACTION {
  SHOOT_RED,  // for skills
  SHOOT_BLUE, // for skills
  SHOOT,      // for auton (shoot into goal based on team)

  INTAKE,  // intake 1 disc
  INTAKE3, // intake 3 stack
  ROLLER,

  MOVE, // null
};
void performAction(ACTION act) { actionArr[act](); }
} // namespace Actions

class AutonNode {
  Actions::ACTION act;
  Position *pos;

  AutonNode(Position *position, Actions::ACTION action)
      : pos(position), act(action){};

  void execute() {
    pos->goTo();
    Actions::performAction(act);
  }
};

autonSkills(){

};