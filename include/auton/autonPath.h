#include "auton/autonNode.h"
class AutonPath {
private:
  AutonNode nodes[];

public:
  AutonPath(AutonNode nodes[]);
  void run();
};