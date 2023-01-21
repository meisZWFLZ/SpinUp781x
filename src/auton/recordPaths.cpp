#include "auton/recordPaths.h"
#include <algorithm>
#include <fstream>
#include <sstream>

const char *RecordingPaths::path = "paths.txt";
std::vector<std::string> RecordingPaths::paths = {};
const char RecordingPaths::delimiter = ',';

const std::vector<std::string> RecordingPaths::get() {
  if (paths.empty())
    populatePaths();
  return paths;
};

void RecordingPaths::populatePaths() {
  std::ifstream loadFile(path, std::ifstream::binary);
  if (loadFile.is_open()) {
    paths = {};
    std::string token;
    while (getline(loadFile, token, delimiter) && loadFile)
      paths.push_back(token);
  } else {
    printf("Couldn't open file\n");
  }
};

void RecordingPaths::write(const std::vector<std::string> pathArr) {
  std::ofstream myfile(path, std::ofstream::binary);
  if (myfile.is_open()) {

    std::vector<std::string> pathArr1 = pathArr;
    std::sort(pathArr1.begin(), pathArr1.end());
    pathArr1.erase(std::unique(pathArr1.begin(), pathArr1.end()),
                   pathArr1.end());

    for (auto str : pathArr1)
      myfile << str << delimiter;
    populatePaths();
  } else {
    printf("Couldn't open file\n");
  }
}

void RecordingPaths::add(std::string newPath) {
  paths.push_back(newPath);
  write(paths);
}

void RecordingPaths::remove(const int index) {
  paths.erase(paths.begin() + index);
  write(paths);
}