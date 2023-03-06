#ifndef AUTONS_H
#define AUTONS_H

#include <functional>
#include <utility>
#include <vector>

namespace auton {

class Auton {
protected:
  Auton(const char *);

public:
  const char *name;
  virtual void execute() const = 0;
};
class CallbackAuton : public Auton {
protected:
  void (*callback)(void);

public:
  CallbackAuton(const char *, void (*)(void));
  void execute() const override;
};

extern const std::vector<const Auton*> autons;
} // namespace auton
#endif