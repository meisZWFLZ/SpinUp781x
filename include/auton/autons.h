#ifndef AUTONS_H
#define AUTONS_H

#include <functional>
#include <utility>
#include <vector>

namespace auton {
extern const std::vector<
    std::pair<const char *, const std::function<void(void)>>>
    autons;
}
#endif