#include "position.h"
#include "robot.h"
#include <vector>

namespace auton {
class Action {
public:
  virtual void execute() const = 0;
};

class Roller : public Action {
public:
  void execute() const override;
  static const Roller ROLLER = {};
};

class Intake : public Action {
public:
  void execute() const override;
  static const Intake INTAKE = {};
};

class StopIntake : public Action {
public:
  void execute() const override;
  static const StopIntake STOP_INTAKE = {};
};

class Expand : public Action {
public:
  void execute() const override;
  static const Expand EXPAND = {};
};

class Shoot : public Action {
private:
  const Robot::GOAL goal;
public:
  Shoot(const Robot::GOAL goal);
  void execute() const override;
  static const Shoot RED = {Robot::GOAL::RED};
  static const Shoot BLUE = {Robot::GOAL::BLUE};
  static const Shoot MY_TEAM = {Robot::GOAL::MY_TEAM};
};

class PTO : public Action {
private:
  const Robot::PTO_STATE state;
public:
  PTO(const Robot::PTO_STATE state);
  void execute() const override;
  static const PTO INTAKE = {Robot::PTO_STATE::INTAKE};
  static const PTO DRIVE = {Robot::PTO_STATE::DRIVE};
};

class GoTo : public Action {
private:
  const Position target;
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
  static constexpr int discIntakeWait = 10;
  static const WAIT DISC_INTAKE_WAIT = {10};
  const Wait waitAction;
public:
  IntakeDisc(int time);
  IntakeDisc();
  static const IntakeDisc INTAKE_DISC = {};
};
} // namespace auton