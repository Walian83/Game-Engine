#pragma once

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "TriangleRenderer.h"
#include "CubeRenderer.h"
#include "Camera.h"
#include "Model.h"
#include "ModelRenderer.h"
#include "SkyboxRenderer.h"
#include "Billboard.h"
#include "BillboardRenderer.h"
#include "GUIRenderer.h"

namespace GE {
	//Basic Game Engine class
	class GameEngine {
	public:
		GameEngine();            // Constructor
		
		virtual ~GameEngine();   // Destructor

		bool init();             // Object initialisation
		bool keep_running();     // Indicates if user has closed window and, hence, game
		void processInput();     // Process key presses
		void update();           // This is where we update game logic
		void draw();             // This is where a frame is rendered
		void shutdown();         // This is called when the game is ending
		                         // to release objects created during the game
								 // 
		// Extra methods
		void setwindowtitle(const char*);

	private:
		// Private data
		// SDL window object to represent
		SDL_Window* window;

		// SDL Gl context to represent OpenGL in the program
		SDL_GLContext glContext;

		// Camera
		Camera* cam;
		
		// Cursor
		SDL_Cursor* cursor; /* Make this variable visible in the point
					   where you exit the program */

		// GUI renderer object
		GUIRenderer* gr;

		// FPSMsg text object
		GUIText* FPSMsg;

		// Minimap objects
		GUIImage* miniMapImg;
		GUIImage* miniMapPlayerImg;

		// Hud image
		GUIImage* cockpitImg;

		Uint32 last_time, current_time, frame_count;

		// TriangleRenderer object variable
		TriangleRenderer* triangle;

		CubeRenderer* cube;

		Model* m;
		Model* m2;
		Model* m3;
		Model* m4;
		Model* m5;
		Model* m6;
		Model* m7;
		Model* m8;
		Texture* tex;
		Texture* tex2;
		Texture* tex3;
		Texture* tex4;
		Texture* tex5;
		Texture* tex6;
		Texture* tex7;
		Texture* tex8;
		ModelRenderer* mr;
		ModelRenderer* mr2;
		ModelRenderer* mr3;
		ModelRenderer* mr4;
		ModelRenderer* mr5;
		ModelRenderer* mr6;
		ModelRenderer* mr7;
		ModelRenderer* mr8;

		SkyboxRenderer* skybox;

		Texture* bbTex;
		Billboard* bb;
		BillboardRenderer* bbr;

		Texture* bbTex2;
		Billboard* bb2;
		BillboardRenderer* bbr2;

		Texture* bbTex3;
		Billboard* bb3;
		BillboardRenderer* bbr3;

		Texture* bbTex4;
		Billboard* bb4;
		BillboardRenderer* bbr4;

		Texture* bbTex5;
		Billboard* bb5;
		BillboardRenderer* bbr5;
	};

	// Helper function
	void display_info_message(const char*);
}