#include "GameEngine.h"

using namespace GE;

int main(int argc, char* argv[]) {
	// Create a game engine object
	GameEngine ge;

	// Initialise it
	if (!ge.init()) {
		display_info_message("Couldn't start SDL. Check console output for more details");

		return -1;
	}

	// Main loop
	while (ge.keep_running()) {
		// Update game state
		ge.update();

		// Draw
		ge.draw();
	}

	// Destroy game engine
	ge.shutdown();

	return 0;
}