#include "auton/path.h"
#include "./robot.h"
#include "auton/autons.h"
#include <functional>

using namespace auton;

Path::Path(const std::vector<Node> nodes, const std::string name)
    : Auton(name.c_str()), nodes(nodes){};
Path::Path(const std::string path, const std::string name)
    : Auton(name.c_str()), nodes(Node::fromJSON(path)){};
;
Path::Path(const std::string path)
    : Auton(path.c_str()), nodes(Node::fromJSON(path)){};

void executeActions(const std::vector<ACTION> actions) {
  bool intake = false;
  for (ACTION act : actions)
    switch (act) {
    case ACTION::EXPAND:
      Robot::InputListeners::expand();
      break;
    case ACTION::INTAKE:
      intake = true;
      break;
    case ACTION::ROLLER:
      Robot::Drivetrain::left(0.2);
      Robot::Drivetrain::right(0.2);
      Robot::Actions::roller();
      Robot::Drivetrain::left(0);
      Robot::Drivetrain::right(0);
      break;
    case ACTION::SHOOT:
      Robot::Actions::shoot(Robot::GOAL::MY_TEAM);
      wait(2000, msec);
      break;
    case ACTION::PISTON_SHOOT:
      Robot::Actions::pistonShoot(Robot::GOAL::MY_TEAM);
      wait(2000, msec);
      break;
    }
  if (intake)
    Robot::Actions::intake();
  else
    Robot::Actions::stopIntake();
}
void Path::execute() const {
  auto i = nodes.begin();
  Robot::setPosition(i->pos);
  executeActions(i->actions);
  for (++i; i != nodes.end(); ++i) {
    Path::executeNode(*i);
    // printf(
    //     "\x1b[32m\x1b[0m~~~~~~~~\nat target!!\n~~~~~~~~\x1b[37m\x1b[0m\n");
  }
}

void Path::executeNode(Node node) {
  Robot::Actions::goTo(node.pos, node.pos.marginOfError);
  executeActions(node.actions);
}