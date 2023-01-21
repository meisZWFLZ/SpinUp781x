#include <string>
#include <vector>

#ifndef RECORD_PATHS_H
#define RECORD_PATHS_H

class RecordingPaths {
private:
  static const char *path;
  static const char delimiter;
  static std::vector<std::string> paths;
  static void populatePaths();

public:
  static const std::vector<std::string> get();
  static void write(const std::vector<std::string>);
  static void add(std::string newPath);
  static void remove(const int index);
};

#endif