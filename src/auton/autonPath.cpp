#include "auton/autonPath.h"

AutonPath::AutonPath(std::vector<AutonNode> autonNodes) { nodes = autonNodes; };

void AutonPath::run() {
  for (int i = 0; i < nodes.size(); i++) {
    nodes[i].execute();
  };
};