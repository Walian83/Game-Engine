#include "GameEngine.h"
#include <iostream>
#include <sstream>

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
		window = SDL_CreateWindow("SDL OpenGL", 50, 50, 1024, 580, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

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
		cam = new Camera(glm::vec3(0.0f, 0.0f, 10.0f), // Position
			glm::vec3(0.0f, 0.0f, 0.0f), // Look at
			glm::vec3(0.0f, 1.0f, 0.0f), // Up direction
			45.0f, 1024.0f / 580.0f, 0.1f, 100.0f); // fov, aspect ratio based on window dimensions, near and far clip planes

		// Create the Model object
		m = new Model();
		m2 = new Model();
		m3 = new Model();
		m4 = new Model();
		m5 = new Model();
		m6 = new Model();
		m7 = new Model();
		m8 = new Model();

		// Load vertices from monkey.obj file
		bool result = m->loadFromFile("space_frigate.obj");
		bool result2 = m2->loadFromFile("alien_plant_1.fbx");
		bool result3 = m3->loadFromFile("fungi.fbx");
		bool result4 = m4->loadFromFile("JellyFish.fbx");
		bool result5 = m5->loadFromFile("rock_formation.fbx");
		bool result6 = m6->loadFromFile("terrain.obj");
		bool result7 = m7->loadFromFile("alienplant.fbx");
		bool result8 = m8->loadFromFile("planta.fbx");

		// Check it was loaded okay
		if (!result || !result3) {
			std::cerr << "Failed to load model" << std::endl;
		}

		tex = new Texture("space_frigate_color.jpg");
		tex2 = new Texture("small_tree_plant.jpg");
		tex3 = new Texture("hat_albedo.jpeg");
		tex4 = new Texture("jellyfish.png");
		tex5 = new Texture("rock.png");
		tex6 = new Texture("grass03.png");
		tex7 = new Texture("alienplant.jpeg");
		tex8 = new Texture("planta.png");

		// Create a ModelRenderer passing the Model object just loaded
		mr = new ModelRenderer();
		mr2 = new ModelRenderer();
		mr3 = new ModelRenderer();
		mr4 = new ModelRenderer();
		mr5 = new ModelRenderer();
		mr6 = new ModelRenderer();
		mr7 = new ModelRenderer();
		mr8 = new ModelRenderer();

		// Initialise the ModelRenderer, reduce the scale
		mr->init();
		mr->setPos(50.0f, 10.0f, -35.0f);
		mr->setTexture(tex);
		mr->setScale(0.5f, 0.5f, 0.5f);

		mr2->init();
		mr2->setPos(6.0f, -3.5f, -20.0f);
		mr2->setTexture(tex2);
		mr2->setScale(0.5f, 0.5f, 0.5f);

		mr3->init();
		mr3->setPos(14.0f, -3.5f, -14.0f);
		mr3->setTexture(tex3);
		mr3->setScale(1.0f, 1.0f, 1.0f);

		mr4->init();
		mr4->setPos(-6.0f, -1.5f, -13.0f);
		mr4->setTexture(tex4);
		mr4->setScale(0.008f, 0.008f, 0.008f);

		mr5->init();
		mr5->setPos(-14.0f, 3.0f, -6.0f);
		mr5->setTexture(tex5);
		mr5->setScale(1.0f, 1.0f, 1.0f);

		mr6->init();
		mr6->setPos(-30.0f, -2.5f, -50.0f);
		mr6->setTexture(tex6);
		mr6->setScale(60.0f, 0.5f, 60.0f);

		mr7->init();
		mr7->setPos(-20.0f, -2.2f, -18.0f);
		mr7->setTexture(tex7);
		mr7->setScale(0.01f, 0.01f, 0.01f);

		mr8->init();
		mr8->setPos(-6.0f, -2.0f, -26.0f);
		mr8->setTexture(tex8);
		mr8->setScale(0.1f, 0.03f, 0.1f);

		skybox = new SkyboxRenderer("front.jpg", "back.jpg",
									"right.jpg", "left.jpg",
									"top.jpg", "bottom.jpg");

		// Create the TriangleRenderer object
		triangle = new TriangleRenderer();
		cube = new CubeRenderer();

		// Initialise the object
		//triangle->init();

		//triangle->setPos(-3.5f, 0.0f, 0.0f);
		//triangle->setScale(1.0f, 1.0f, 1.0f);

		//cube->init();

		//cube->setPos(3.5f, 0.0f, 0.0f);
		//cube->setScale(1.0f, 1.0f, 1.0f);

		// Load a texture for the billboard
		bbTex = new Texture("./galaxy.png");
		bbTex2 = new Texture("./planet.png");
		bbTex3 = new Texture("./earth.png");
		bbTex4 = new Texture("./spacestation.png");
		bbTex5 = new Texture("./astronaut.png");

		// By default billboard will be at 0,0,0
		// in front of the camera
		bb = new Billboard(bbTex);
		bb2 = new Billboard(bbTex2);
		bb3 = new Billboard(bbTex3);
		bb4 = new Billboard(bbTex4);
		bb5 = new Billboard(bbTex5);

		// Scale billboard as only 1.0x1.0
		bb->setScaleX(7.0f);
		bb->setScaleY(7.0f);
		bb2->setScaleX(10.0f);
		bb2->setScaleY(10.0f);
		bb3->setScaleX(5.0f);
		bb3->setScaleY(5.0f);
		bb4->setScaleX(10.0f);
		bb4->setScaleY(10.0f);
		bb5->setScaleX(3.0f);
		bb5->setScaleY(3.0f);

		// Set it in front of the model
		bb->setZ(70.0f); bb->setX(35.0f); bb->setY(15.0f);
		bb2->setZ(-80.0f); bb2->setY(15.0f); bb2->setX(-15.0f);
		bb3->setZ(80.0f); bb3->setX(-15.0f); bb3->setY(30.0f);
		bb4->setZ(-50.0f); bb4->setY(20.0f); bb4->setX(25.0f);
		bb5->setZ(-50.0f); bb5->setX(25.0f); bb5->setY(30.0f);

		// Create the billboard renderer
		bbr = new BillboardRenderer();
		bbr2 = new BillboardRenderer();
		bbr3 = new BillboardRenderer();
		bbr4 = new BillboardRenderer();
		bbr5 = new BillboardRenderer();

		// Initialise the renderer
		bbr->init();
		bbr2->init();
		bbr3->init();
		bbr4->init();
		bbr5->init();

		// Create the GUIRenderer object
		gr = new GUIRenderer();

		gr->init(cam->getWidth(), cam->getHeight());

		// Create a text object for the FPS
		FPSMsg = new GUIText(472, 20, "FPS goes here", "./Oswald-Regular.ttf");

		// Load a texture for the "minimap" and create a GUIImage object for texture
		Texture* miniMapTex = new Texture("./grass-minimap.png");

		// Likewise, load a texture for the player and create a GUIImage for it
		miniMapImg = new GUIImage(0, 0, miniMapTex);

		Texture* miniMapPlayerTex = new Texture("./player-mini-map-img.png");

		miniMapPlayerImg = new GUIImage(0, 0, miniMapPlayerTex);

		// Load cockpit image for the hud and create GUIImage for it
		Texture* cockpitTex = new Texture("./halohud.png");

		cockpitImg = new GUIImage(0, 0, cockpitTex);

		last_time = current_time = SDL_GetTicks();

		frame_count = 0;

		int32_t cursorData[2] = { 0, 0 };
		cursor = SDL_CreateCursor((Uint8*)cursorData, (Uint8*)cursorData, 8, 8, 4, 4);
		SDL_SetCursor(cursor);

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
		cube->setRotation(cube->getRotX() + 2.5f, cube->getRotY() + 2.5f, 0.0f);
		//mr->setRotation(0.0f, mr->getRotY() + 0.5f, 0.0f);
		mr->setPos(mr->getPosX() - 0.08f, 10.0f, -35.0f);
		if (mr->getPosX() <= -70.0f) {
			mr->setPos(50.0f, 10.0f, -35.0f);

		}

		mr4->setRotation(0.0f, mr4->getRotY() + 2.5f, 0.0f);

		miniMapPlayerImg->setX((cam->getPos().x * 1.95f) + 55);
		miniMapPlayerImg->setY((cam->getPos().z * 1.95f) + 92);

		current_time = SDL_GetTicks();

		if (current_time - last_time > 1000) {
			// Create a variable to store a string
			std::ostringstream msg;

			// Create a message based on the value in frame_count
			msg << "FPS = " << frame_count;

			// Set the GUI text to the new frame count
			FPSMsg->setText(msg.str());

			// Reset for next frame
			frame_count = 0;

			last_time = current_time;
		}

		frame_count++;
	}

	//
	//
	void GameEngine::draw() {
		glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render skybox
		skybox->draw(cam);

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
		//triangle->draw(cam);
		//cube->draw(cam);

		// Render the model using the model renderer
		mr->draw(cam, m);
		mr2->draw(cam, m2);
		mr3->draw(cam, m3);
		mr4->draw(cam, m4);
		mr5->draw(cam, m5);
		mr6->draw(cam, m6);
		mr7->draw(cam, m7);
		mr8->draw(cam, m8);

		bbr->draw(bb, cam);
		bbr2->draw(bb2, cam);
		bbr3->draw(bb3, cam);
		bbr4->draw(bb4, cam);
		bbr5->draw(bb5, cam);

		// Draw GUI objects
		gr->drawImage(cockpitImg);
		gr->drawImage(miniMapImg);
		gr->drawText(FPSMsg);
		gr->drawImage(miniMapPlayerImg);

		// Show frame
		SDL_GL_SwapWindow(window);
	}

	//
	void GameEngine::shutdown() {
		// Get the triangle object to release its resources
		//triangle->destroy();
		//
		cube->destroy();

		mr->destroy();
		mr2->destroy();
		mr3->destroy();
		mr4->destroy();
		mr5->destroy();
		mr6->destroy();
		mr7->destroy();
		mr8->destroy();

		skybox->destroy();

		delete mr;
		delete mr2;
		delete mr3;
		delete mr4;
		delete mr5;
		delete mr6;
		delete mr7;
		delete mr8;

		delete m;
		delete m2;
		delete m3;
		delete m4;
		delete m5;
		delete m6;
		delete m7;
		delete m8;

		delete cam;

		// Free billboard objects
		delete bbr;
		delete bb;
		delete bbTex;

		delete bbr2;
		delete bb2;
		delete bbTex2;

		delete bbr3;
		delete bb3;
		delete bbTex3;

		delete bbr4;
		delete bb4;
		delete bbTex4;

		delete bbr5;
		delete bb5;
		delete bbTex5;

		// GUI objects
		// FPSMsg text object
		delete FPSMsg;

		// Minimap objects
		delete miniMapImg;
		delete miniMapPlayerImg;

		// Hud image
		delete cockpitImg;

		// GUI Renderer
		delete gr;

		SDL_FreeCursor(cursor);

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

	void GameEngine::processInput() {
		// Camera and mouse speed should be influenced by deltaTime for hardware independance
		const float camSpeed = 0.2f; // adjust accordingly - should be part of the engine variables
		const float mouseSens = 0.1f; // adjust accordingly - should be part of the engine variables

		// Get current mouse x and y
		int mouse_x, mouse_y;
		SDL_GetMouseState(&mouse_x, &mouse_y);
		//
		int diffx = mouse_x - (cam->getWidth() / 2);
		int diffy = (cam->getHeight() / 2) - mouse_y;

		// Update the yaw and pitch based on the mouse differential
		cam->setYaw(cam->getYaw() + diffx * mouseSens);
		cam->setPitch(cam->getPitch() + diffy * mouseSens);

		// Set the mouse back to the centre
		SDL_WarpMouseInWindow(window, cam->getWidth() / 2, cam->getHeight() / 2);

		//
		//
		//
		glm::vec3 direction;
		direction.x = cos(glm::radians(cam->getYaw())) * cos(glm::radians(cam->getPitch()));
		direction.y = sin(glm::radians(cam->getPitch()));
		direction.z = sin(glm::radians(cam->getYaw())) * cos(glm::radians(cam->getPitch()));
		cam->setTarget(glm::normalize(direction));

		// Array to hold states of key presses we're interested in (standard WASD)
		bool keyStates[4] = { 0,0,0,0 };
		// Indices to the above array to store respective key presses
		int UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3;

		// Poll for SDL event - better ways to handle key input for smoother behaviour
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			// If there has been a key pressed, check if it is UP, DOWN, LEFT or RIGHT
			// Mark the array to indicate this key is held down
			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.scancode) {
				case SDL_SCANCODE_UP:
					keyStates[UP] = true;
					break;
				case SDL_SCANCODE_DOWN:
					keyStates[DOWN] = true;
					break;
				case SDL_SCANCODE_LEFT:
					keyStates[LEFT] = true;
					break;
				case SDL_SCANCODE_RIGHT:
					keyStates[RIGHT] = true;
					break;
				}
			}
			// If there has been a key lifted, check if it is UP, DOWN, LEFT or RIGHT
			// Mark the array to indicate this key is no longer being pressed
			if (e.type == SDL_KEYUP) {
				switch (e.key.keysym.scancode) {
				case SDL_SCANCODE_UP:
					keyStates[UP] = false;
					break;
				case SDL_SCANCODE_DOWN:
					keyStates[DOWN] = false;
					break;
				case SDL_SCANCODE_LEFT:
					keyStates[LEFT] = false;
					break;
				case SDL_SCANCODE_RIGHT:
					keyStates[RIGHT] = false;
					break;
				}
			}

			// Handle camera movement based upon the relevant key press
			if (keyStates[UP]) {
				cam->setPos(cam->getPos() + cam->getTarget() * camSpeed);
			}
			if (keyStates[DOWN]) {
				cam->setPos(cam->getPos() - cam->getTarget() * camSpeed);
			}
			if (keyStates[LEFT]) {
				// Take away the cross product of the facing and up vector from the camera position - strafe left
				cam->setPos(cam->getPos() - glm::normalize(glm::cross(cam->getTarget(), cam->getUpDir())) * camSpeed);
			}
			if (keyStates[RIGHT]) {
				// Take away the cross product of the facing and up vector from the camera position - strafe right
				cam->setPos(cam->getPos() + glm::normalize(glm::cross(cam->getTarget(), cam->getUpDir())) * camSpeed);
			}
		}

		// Call to update the camera view matrix
		// Projection is also updated in case effects like zoom are used (e.g. right click to aim, shrink the fov a little)
		cam->updateCamMatrices();
	}
}
