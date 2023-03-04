// #include <fstream>
// #include <iostream>
// #include <regex>
// #include <string>
// #include <vector>


// // This function takes the path to a JSON file as a parameter and returns a
// // vector of AutonNode structures. It first opens the file and checks if it is
// // open. If it is, it
// struct AutonPosition {
//   float x;
//   float y;
//   float heading;
//   float marginOfError;
// };

// enum class AUTON_ACTION {
//   SHOOT,
//   PISTON_SHOOT,
//   INTAKE,
//   ROLLER,
//   EXPAND,
// };

// struct AutonNode {
//   AutonPosition position;
//   std::vector<AUTON_ACTION> actions;
// };

// std::vector<AutonNode> readJson(std::string jsonPath) {
//   std::ifstream file(jsonPath);
//   std::vector<AutonNode> nodes;

//   if (file.is_open()) {
//     std::string line;
//     while (std::getline(file, line)) {
//       size_t start = line.find("{");
//       size_t end = line.find("}");

//       if (start != std::string::npos && end != std::string::npos) {
//         std::string nodeJson = line.substr(start, end - start + 1);

//         size_t pos = nodeJson.find(":");
//         std::string positionJson = nodeJson.substr(pos + 1);
//         positionJson = positionJson.substr(0, positionJson.find(","));

//         pos = positionJson.find("[");
//         std::string xJson = positionJson.substr(pos + 1);
//         xJson = xJson.substr(0, xJson.find(","));

//         pos = positionJson.find(",", pos + 1);
//         std::string yJson = positionJson.substr(pos + 1);
//         yJson = yJson.substr(0, yJson.find(","));

//         pos = positionJson.find(",", pos + 1);
//         std::string headingJson = positionJson.substr(pos + 1);
//         headingJson = headingJson.substr(0, headingJson.find(","));

//         pos = positionJson.find(",", pos + 1);
//         std::string marginOfErrorJson = positionJson.substr(pos + 1);
//         marginOfErrorJson =
//             marginOfErrorJson.substr(0, marginOfErrorJson.find("]"));

//         AutonPosition position = {std::stof(xJson), std::stof(yJson),
//                                   std::stof(headingJson),
//                                   std::stof(marginOfErrorJson)};

//         pos = nodeJson.find(":");
//         std::string actionsJson = nodeJson.substr(pos + 1);
//         actionsJson = actionsJson.substr(0, actionsJson.length() - 1);

//         pos = actionsJson.find("[");
//         std::string actionJson = actionsJson.substr(pos + 1);
//         actionJson = actionJson.substr(0, actionJson.find(","));

//         std::vector<AUTON_ACTION> actions;
//         while (actionJson != actionsJson) {
//           actions.push_back(static_cast<AUTON_ACTION>(std::stoi(actionJson)));
//           pos = actionsJson.find(",", pos + 1);
//           actionJson = actionsJson.substr(pos + 1);
//           actionJson = actionJson.substr(0, actionJson.find(","));
//         }

//         nodes.push_back({position, actions});
//       }
//     }
//   } else {
//     std::cerr << "Error opening file: " << jsonPath << std::endl;
//   }

//   return nodes;
// }
