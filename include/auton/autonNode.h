#include "auton/position.h"

enum ACTION {
  SHOOT_RED,  // for skills
  SHOOT_BLUE, // for skills
  SHOOT,      // for auton (shoot into goal based on team)

  INTAKE,  // intake 1 disc
  INTAKE3, // intake 3 stack
  ROLLER,

  MOVE, // null
};

class AutonNode {
private:
  ACTION act;
  Position *pos;

public:
  AutonNode(Position *position, ACTION action);
  void execute();
};