#include "../position.h"
#include <string>
#include <vector>

using NormalPosition = Position;
namespace auton {
struct Position : public NormalPosition {
public:
  float marginOfError;
  Position(double x, double y, double heading, float marginOfError);
};
enum class ACTION : int {
  SHOOT,
  PISTON_SHOOT,
  INTAKE,
  ROLLER,
  EXPAND,
};

struct Node {
public:
  Position pos;
  std::vector<ACTION> actions;
  Node(Position pos, std::vector<ACTION> actions);
  static std::vector<Node> fromJSON(const std::string path);
  static std::string toString(Node *node);
};
} // namespace auton