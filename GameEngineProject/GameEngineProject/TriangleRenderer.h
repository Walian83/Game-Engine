#pragma once

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

namespace GE {
	class TriangleRenderer {
	public:
		TriangleRenderer();
		virtual ~TriangleRenderer();

		// init method to create the shaders and vertex buffer object
		void init();

		//
		//
		//
		void update();

		//
		void draw();

		//
		void destroy();

	private:
		//
		//
		GLuint programId;

		//
		//
		GLint vertexPos2dLocation;

		//
		//
		GLuint vboTriangle;
	};
}