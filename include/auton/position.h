class Position {
public:
  double x; // inches
  double y; // inches
  double heading;

  Position(double x, double y, double heading);
  Position();

  static double distance(const Position &pos1, const Position &pos2);

  void goTo();
};