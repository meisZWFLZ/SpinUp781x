using namespace std;
using namespace std::chrono_literals;

#include <iostream>
#include <thread>

/*
NOlong ES:

- normal drivetrain?
- tank drive control scheme
- automatic shooting :(

- distance in inches

*/

class Position {
	double x; //inches
	double y; //inches
	double heading;

public:
	Position(double x, double y, double heading) : x(x), y(y), heading(std::fmod(heading, 360)) {};
	Position() : x(0), y(0), heading(0) {};

	static double distance(const Position &pos1, const Position &pos2) {
		return sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2));
	}
};


enum Button {
	LOAD_DISC, EXPANSION, SAFTEY_SWITCH /*enables and disables saftey mechanisms*/, SHOOT
};

const std::chrono::duration<long long, milli> LENGTH_OF_GAME = 12000ms;
const std::chrono::duration<long long, milli> LENGTH_OF_ENGAME = 10000ms;

const std::chrono::steady_clock::time_point TIME_OF_ENGAME = chrono::steady_clock::now() + LENGTH_OF_GAME - LENGTH_OF_ENGAME;

const std::chrono::duration<long long, milli> DISC_LOAD_DELAY = 500ms; //should be time piston takes to complete full cycle

const Position GOAL_POS = { 0,0,0 };

static bool expanded = false;
static bool saftey = false; //true -> disables saftey mechanisms
static std::chrono::steady_clock::time_point lastDiscLoad;


void endgameExpansion() {
	//expansion stuff
}

void loadDisc() {
	//piston stuff
}

Position getPosition() {
}

void turn(double newHeading) {
}

void aimTurret(int distance /*inches*/) {
}

void expansionButtonSubscriber() {
	//prevents expansion untill endgame
	if (saftey || std::chrono::steady_clock::now() > TIME_OF_ENGAME) endgameExpansion();
}

void loadDiscButtonSubscriber() {
	//ensures piston has enough time to contract
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	if (saftey || now - lastDiscLoad > DISC_LOAD_DELAY) {
		//loop allows for the button be held to repeat action
		do {
			lastDiscLoad = now;
			loadDisc();

			//wait for delay to repeat
			std::this_thread::sleep_for(DISC_LOAD_DELAY);
			now = std::chrono::steady_clock::now();
		} while (buttonPressed(Button::EXPANSION));
	}
}

void safteySwitchSubscriber() {
	saftey != saftey;
}

void subscribeButtonListener(Button button, void(*func)()) {

}

void shootButtonSubscriber() {
	Position pos = getPosition();
	turn(0/*too lazy*/);
	aimTurret(Position::distance(pos, GOAL_POS));
}

bool buttonPressed(Button button) {
	return true;
}

int main()
{
	subscribeButtonListener(Button::SHOOT, &shootButtonSubscriber);
	subscribeButtonListener(Button::SAFTEY_SWITCH, &safteySwitchSubscriber);
	subscribeButtonListener(Button::LOAD_DISC, &loadDiscButtonSubscriber);
	subscribeButtonListener(Button::EXPANSION, &expansionButtonSubscriber);
}