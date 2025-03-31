#include "GameEngine.h"

#include <sstream>

using namespace GE;

int main(int argc, char* argv[]) {
	// Create a game engine object
	GameEngine ge;

	// Initialise it
	if (!ge.init()) {
		display_info_message("Couldn't start SDL. Check console output for more details");

		return -1;
	}

	Uint32 last_time = SDL_GetTicks(), current_time = 0;

	int frame_count = 0;

	// Main loop
	while (ge.keep_running()) {
		// Update game state
		ge.update();

		// Handle input
		ge.processInput();

		// Draw
		ge.draw();

		frame_count++;

		current_time = SDL_GetTicks();

		if (current_time - last_time > 1000) {
			std::ostringstream msg;

			msg << "FPS = " << frame_count;

			ge.setwindowtitle(msg.str().c_str());

			frame_count = 0;

			last_time = current_time;
		}
	}

	// Destroy game engine
	ge.shutdown();

	return 0;
}