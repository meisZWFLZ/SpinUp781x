#include "auton/elements.h"

using namespace elements;

const Coordinate ROLLER::arr[] = {{21.5, 106.5}, {26.5, 126.7}, {139.8, 41.7}, {132, 43}};

const Coordinate ROLLER::LEFT = ROLLER::arr[0];
const Coordinate ROLLER::FRONT = ROLLER::arr[1];
const Coordinate ROLLER::RIGHT = ROLLER::arr[2];
const Coordinate ROLLER::BACK = ROLLER::arr[3];

const Coordinate ROLLER::BLUE_LEFT = ROLLER::arr[0];
const Coordinate ROLLER::BLUE_RIGHT = ROLLER::arr[3];
const Coordinate ROLLER::RED_LEFT = ROLLER::arr[2];
const Coordinate ROLLER::RED_RIGHT = ROLLER::arr[1];

const Coordinate GOAL::RED = {0, 0};
const Coordinate GOAL::BLUE = {0, 0};