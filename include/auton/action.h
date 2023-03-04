#include "../position.h"
#include "../robot.h"
#include <vector>

namespace auton {
class Action {
public:
  virtual void execute() const = 0;
};
class Roller : public Action {
public:
  void execute() const override;
  // static const Roller ROLLER;
};
class Intake : public Action {
public:
  void execute() const override;
};
class StopIntake : public Action {
public:
  void execute() const override;
};
class Expand : public Action {
public:
  void execute() const override;
};
class Shoot : public Action {
private:
  const Robot::GOAL goal;

public:
  Shoot(const Robot::GOAL goal);
  void execute() const override;
};
// class PTO : public Action {
// private:
//   const Robot::PTO_STATE state;

// public:
//   PTO(const Robot::PTO_STATE state);
//   void execute() const override;
// };
class GoTo : public Action {
private:
  const Position pos;

public:
  GoTo(const Position target);
  void execute() const override;
};
class Wait : public Action {
private:
  const int time;

public:
  Wait(const int time);
  void execute() const override;
};
class Path : public Action {
private:
  const std::vector<const Action *> actions;

public:
  Path(const std::vector<const Action *> actions);
  void execute() const override;
};
class IntakeDisc : public Path {
  Wait waitAction;
  IntakeDisc(int time);
};
} // namespace auton