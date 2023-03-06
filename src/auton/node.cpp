#include "./auton/node.h"
#include "conversions.h"
#include "position.h"
#include <fstream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

using namespace auton;

std::string readFile(const std::string &filePath) {
  std::ifstream inputFileStream(filePath);

  if (!inputFileStream.is_open()) {
    return "";
  }

  std::stringstream buffer;
  buffer << inputFileStream.rdbuf();

  return buffer.str();
}

auton::Position::Position(double x, double y, double heading,
                          float marginOfError)
    : NormalPosition(x, y, heading), marginOfError(marginOfError) {}

Node::Node(auton::Position pos, std::vector<ACTION> actions) : pos(pos) {
  this->actions = actions;
}
std::vector<Node> Node::fromJSON(const std::string path) {
  const std::string fileContent = readFile(path);

  static const std::regex nodeRegex(
      "\\{\"position\":\\{\"x\":(\\d+),\"y\":(\\d+),\"heading\":(-?\\d*),"
      "\"marginOfError\":(\\d+\\.?\\d*)\\}(?:,\"actions\":\\[((?:\\d,?)*)\\])"
      "?\\}",
      std::regex_constants::ECMAScript);
  static const std::regex actionsRegex("\\d+");

  auto matches_begin =
      std::sregex_iterator(fileContent.begin(), fileContent.end(), nodeRegex);
  auto matches_end = std::sregex_iterator();

  std::vector<Node> nodes = {};
  for (std::sregex_iterator i = matches_begin; i != matches_end; ++i) {
    std::smatch m = *i;
    std::vector<ACTION> acts = {};
    if (m[5].matched) {
      std::string actMatch = m[5].str();
      auto actions_begin =
          std::sregex_iterator(actMatch.begin(), actMatch.end(), actionsRegex);
      for (std::sregex_iterator j = actions_begin; j != matches_end; ++j)
        if ((*j)[0].matched)
          acts.push_back(static_cast<ACTION>(std::atoi((*j)[0].str().c_str())));
    }
    nodes.push_back(
        {{std::atof(m[1].str().c_str()), std::atof(m[2].str().c_str()),
          Conversions::Degrees::toRadians(std::atof(m[3].str().c_str())),
          static_cast<float>(std::atof(m[4].str().c_str()))},
         acts});
  }
  return nodes;
}
std::string Node::toString(Node *node) {
  std::stringstream str;

  str << "x:" << node->pos.x << ",y:" << node->pos.y
      << ",h:" << node->pos.heading << ",e:" << node->pos.marginOfError
      << ",acts:[";
  for (auto i = node->actions.begin(); i != node->actions.end(); ++i) {
    str << (int)(*i) << ",";
  }
  str << "]";

  return str.str();
}