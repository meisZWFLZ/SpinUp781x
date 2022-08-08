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
  bool PTOPos;

  void movePTO();
public:
  AutonNode(Position *position, bool PTOPosition, ACTION action);
  void execute();
};