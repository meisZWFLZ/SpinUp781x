#include "coordinate.h"

namespace elements {

const Coordinate DISC[60] = {
    {12, 126},     {50.5, 26.5}, {36, 50.5}, {45.5, 50.5},   {50.5, 45.5},
    {50.5, 36},    {50.5, 26.5}, {24, 120},  {36, 108},      {36, 108},
    {36, 108},     {36, 84},     {36, 84},   {36, 84},       {48, 96},

    {60, 108},     {60, 84},     {60, 60},   {72, 48},       {72, 96},
    {84, 84},      {84, 60},     {84, 36},   {117.5, 117.5}, {117.5, 108},
    {117.5, 98.5}, {96, 48},     {108, 36},  {108, 36},      {108, 36},

    {108, 60},     {108, 60},    {108, 60},  {98.5, 93.5},   {108, 93.5},
    {117.5, 93.5}, {120, 24},    {132, 12},  {12, 72},       {12, 72},
    {12, 72},      {12, 72},     {12, 72},   {12, 72},       {12, 72},

    {12, 72},      {12, 72},     {12, 72},   {12, 72},       {0, 0},
    {0, 0},        {0, 0},       {0, 0},     {0, 0},         {0, 0},
    {0, 0},        {0, 0},       {0, 0},     {0, 0},         {0, 0},
};

class ROLLER {
public:
  static const Coordinate arr[4];
  // from the perspective of the right side of blue high goal
  static const Coordinate LEFT;  // 0
  static const Coordinate FRONT; // 1
  static const Coordinate RIGHT; // 2
  static const Coordinate BACK;  // 3

  // from the perspective of respective high goal
  static const Coordinate BLUE_LEFT;  // 0
  static const Coordinate BLUE_RIGHT; // 3
  static const Coordinate RED_LEFT;   // 2
  static const Coordinate RED_RIGHT;  // 1
};

class GOAL {
public:
  static const Coordinate RED;
  static const Coordinate BLUE;
};
// const Coordinate GOAL[2] = {
//     {0, 0}, // red
//     {0, 0}, // blue
// };
} // namespace elements