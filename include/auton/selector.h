#ifndef AUTON_SELECTOR_H
#define AUTON_SELECTOR_H

#include <functional>
#include <utility>
#include <vector>

class AutonSelector {
  // static std::vector<std::pair<char *, std::function<void(void)>>> autonArr;
  static int autonPos;
  static void print();
  static void listener();
  static bool initialized;
  static void run();

public:
  static const void
  start(/* const std::vector<std::pair<char *, std::function<void(void)>>>
            autonArr1 */);
};

#endif