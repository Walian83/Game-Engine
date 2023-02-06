#include "TriangleRenderer.h"
#include <iostream>

namespace GE {
	GLfloat vertexData[] = {
		-1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f
	};
	
	//
	TriangleRenderer::TriangleRenderer()
	{

	}

	//
	TriangleRenderer::~TriangleRenderer()
	{

	}

	//
	//
	void displayShaderCompilerError(GLuint shaderId) {
		//
		GLint MsgLen = 0;

		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &MsgLen);

		//
		if (MsgLen > 1) {
			//
			GLchar* Msg = new GLchar[MsgLen + 1];

			//
			//
			glGetShaderInfoLog(shaderId, MsgLen, NULL, Msg);

			//
			//
			std::cerr << "Error compiling shader" << Msg << std::endl;

			//
			delete[] Msg;
		}
	}

	void TriangleRenderer::init() {
		//
		//
		//
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		//
		const GLchar* V_ShaderCode[] = {
			"#version 140\n"
			"in vec2 vertexPos2D;\n"
			"void main() {\n"
			"gl_Position = vec4(vertexPos2D.x, vertexPos2D.y, 0, 1);\n"
			"}\n" };

		//
		glShaderSource(vertexShader, 1, V_ShaderCode, NULL);

		//
		glCompileShader(vertexShader);

		//
		//
		GLint isShaderCompiledOK = GL_FALSE;
		
		//
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isShaderCompiledOK);

		//
		if (isShaderCompiledOK != GL_TRUE) {
			//
			std::cerr << "Unable to compile vertex shader" << std::endl;

			displayShaderCompilerError(vertexShader);

			return;
		}

		//
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		//
		const GLchar* F_ShaderCode[] = {
			"#version 140\n"
			"out vec4 fragmentColour;\n"
			"void main()\n"
			"{\n"
			"fragmentColour = vec4(1.0, 0.0, 0.0, 1.0);\n"
			"}\n"
		};

		//
		glShaderSource(fragmentShader, 1, F_ShaderCode, NULL);

		//
		glCompileShader(fragmentShader);

		//
		//
		isShaderCompiledOK = GL_FALSE;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isShaderCompiledOK);

		if (isShaderCompiledOK != GL_TRUE) {
			std::cerr << "Unable to compile fragment shader" << std::endl;

			displayShaderCompilerError(fragmentShader);

			return;
		}

		//
		programId = glCreateProgram();

		//
		glAttachShader(programId, vertexShader);
		glAttachShader(programId, fragmentShader);

		//
		//
		//
		glLinkProgram(programId);

		//
		GLint isProgramLinked = GL_FALSE;
		glGetProgramiv(programId, GL_LINK_STATUS, &isProgramLinked);
		if (isProgramLinked != GL_TRUE) {
			std::cerr << "Failed to link program" << std::endl;
		}

		//
		//
		vertexPos2DLocation = glGetAttribLocation(programId, "vertex2D");

		//
		if (vertexPos2DLocation == -1) {
			std::cerr << "Problem getting vertex2DPos" << std::endl;
		}

		//
		glGenBuffers(1, &vboTriangle);
		glBindBuffer(GL_ARRAY_BUFFER, vboTriangle);

		//
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

		//
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//
	void TriangleRenderer::update() {

	}

	//
	void TriangleRenderer::draw() {
		//
		glUseProgram(programId);

		//
		glBindBuffer(GL_ARRAY_BUFFER, vboTriangle);

		//
		glEnableVertexAttribArray(vertexPos2DLocation);

		//
		//
		glVertexAttribPointer(vertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);

		//
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//
		glDisableVertexAttribArray(vertexPos2DLocation);

		//
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//
		glUseProgram(0);
	}
}