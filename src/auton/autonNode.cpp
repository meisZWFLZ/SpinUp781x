#include "auton/autonNode.h"

namespace {
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

void performAction(ACTION act) { actionArr[act](); }
} // namespace

void AutonNode::movePTO() {
  
}

AutonNode::AutonNode(Position *position, bool PTOPosition, ACTION action)
    : pos(position), PTOPos(PTOPosition), act(action){};

void AutonNode::execute() {
  pos->goTo();
  performAction(act);
}