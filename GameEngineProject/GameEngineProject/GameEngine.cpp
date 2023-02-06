#include "GameEngine.h"
#include <iostream>

namespace GE {
	GameEngine::GameEngine() {

	}

	GameEngine::~GameEngine() {

	}

	bool GameEngine::init() {
		// Initialise SDL video subsystem
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			std::cerr << "Unable to initialise SDL! SDL error: " << SDL_GetError() << std::endl;
			
			return false;
		}
		// Set the OpenGL version for the program
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		// Set the type of profile which is core meaning modern OpenGL
		// means no legacy features for backwards compatibility
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		// Create the window and frame features
		// In this case window has a fixed size and prepares window for OpenGL to render into
		window = SDL_CreateWindow("SDL OpenGL", 50, 50, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

		// Check window was created
		if (window == nullptr) {
			// Display an error if window wasn't created
			//
			//
			std::cerr << "Unable to create window! SDL error: " << SDL_GetError() << std::endl;

			// Return failure so caller can work out what to do
			return false;
		}
		//
		//
		glContext = SDL_GL_CreateContext(window);

		//
		if (glContext == nullptr) {
			//
			std::cerr << "SDL could not create GL context! SDL error: " << SDL_GetError() << std::endl;
			return false;
		}
		//
		//
		//
		GLenum status = glewInit();

		//
		if (status != GLEW_OK) {
			std::cerr << "Error initialising GLEW! Error: " << glewGetErrorString(status) << std::endl;

			return false;
		}
		//
		//
		/*if (SDL_GL_SetSwapInterval(1) != 0) {
			std::cerr << "Warning: unable to set VSync! Error: " << SDL_GetError() << std::endl;

			return false;
		}*/
		//

		// Create camera object
		cam = new Camera(glm::vec3(0.0f, 0.0f, 5.0f), // Position
			glm::vec3(0.0f, 0.0f, 0.0f), // Look at
			glm::vec3(0.0f, 1.0f, 0.0f), // Up direction
			45.0f, 640.0f / 480.0f, 0.1f, 100.0f); // fov, aspect ratio based on window dimensions, near and far clip planes

		// Create the TriangleRenderer object
		triangle = new TriangleRenderer();

		// Initialise the object
		triangle->init();

		triangle->setPos(0.0f, 0.0f, 0.0f);
		triangle->setScale(1.0f, 1.0f, 1.0f);

		return true;
	}

	//
	//
	bool GameEngine::keep_running() {
		//
		SDL_PumpEvents();

		SDL_Event evt;

		//
		if (SDL_PeepEvents(&evt, 1, SDL_GETEVENT, SDL_QUIT, SDL_QUIT)) {
			//
			//
			return false;
		}

		//
		return true;
	}

	//
	//
	void GameEngine::update() {
		triangle->setRotation(0.0f, triangle->getRotY() + 2.5f, 0.0f);
	}

	//
	//
	void GameEngine::draw() {
		glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Render the triangle
		//triangle->draw();

		/*glBegin(GL_TRIANGLES);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex2f(-1.0f, 0.0f);
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex2f(1.0f, 0.0f);
			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex2f(0.0f, 1.0f);

			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex2f(-1.0f, 0.0f);
			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex2f(0.0f, -1.0f);
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex2f(1.0f, 0.0f);*/

		/*glBegin(GL_POLYGON);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(-0.75, -0.75);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(0.75, -0.75);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(0.75, 0.75);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(-0.75, 0.75);*/

		// Vertex3f
		/*glBegin(GL_TRIANGLES);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(-1.0f, 0.0f, 0.0f);
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(1.0f, 0.0f, 0.0f);
			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex3f(0.0f, 1.0f, 0.0f);*/

		//glEnd();

		// Render the triangle
		triangle->draw(cam);

		// Show frame
		SDL_GL_SwapWindow(window);
	}

	//
	void GameEngine::shutdown() {
		// Get the triangle object to release its resources
		triangle->destroy();

		SDL_DestroyWindow(window);

		window = nullptr;

		SDL_Quit();
	}

	void GameEngine::setwindowtitle(const char* new_title) {
		SDL_SetWindowTitle(window, new_title);
	}

	void display_info_message(const char* msg) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Basic Game Engine", msg, nullptr);
	}



}