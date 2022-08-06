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

AutonNode::AutonNode(Position *position, ACTION action)
    : pos(position), act(action){};

void AutonNode::execute() {
  pos->goTo();
  performAction(act);
}