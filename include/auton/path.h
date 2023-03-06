#include "./autons.h"
#include "./node.h"
#include "vex_task.h"
#include <functional>
#include <utility>
#include <vector>

namespace auton {
class Path : public Auton {
public:
  const std::vector<Node> nodes;

  Path(const std::vector<Node> nodes, const std::string name);
  Path(const std::string path, const std::string name);
  Path(const std::string path);

  void execute() const;

protected:
  static void executeNode(const Node);
};
} // namespace auton