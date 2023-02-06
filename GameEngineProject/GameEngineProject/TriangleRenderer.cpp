#include "TriangleRenderer.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace GE {
	GLfloat vertexData[] = {
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};
	
	//
	TriangleRenderer::TriangleRenderer()
	{
		// Initialise location, rotation and scale to default values
		pos_x = pos_y = pos_z = 0.0f;
		rot_x = rot_y = rot_z = 0.0f;
		scale_x = scale_y = scale_z = 1.0f;
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
			"in vec3 vertexPos3D;\n"
			"uniform mat4 viewMat;\n"
			"uniform mat4 projMat;\n"
			"uniform mat4 transformMat;\n"
			"void main() {\n"
			"gl_Position = projMat * viewMat * transformMat * vec4(vertexPos3D.x, vertexPos3D.y, vertexPos3D.z, 1);\n"
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
		vertexPos3DLocation = glGetAttribLocation(programId, "vertexPos3D");

		//
		if (vertexPos3DLocation == -1) {
			std::cerr << "Problem getting vertex3DPos" << std::endl;
		}

		// Get uniform id in shader so C++ program can send data to it
		viewUniformId = glGetUniformLocation(programId, "viewMat");
		projectionUniformId = glGetUniformLocation(programId, "projMat");
		// Transformation matrix uniform
		transformUniformId = glGetUniformLocation(programId, "transformMat");

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
	void TriangleRenderer::draw(Camera* cam) {
		// Calculate the transformation matrix for the object. Start with the identity matrix
		glm::mat4 transformationMat = glm::mat4(1.0f);

		transformationMat = glm::translate(transformationMat, glm::vec3(pos_x, pos_y, pos_z));
		transformationMat = glm::rotate(transformationMat, glm::radians(rot_x), glm::vec3(1.0f, 0.0f, 0.0f));
		transformationMat = glm::rotate(transformationMat, glm::radians(rot_y), glm::vec3(0.0f, 1.0f, 0.0f));
		transformationMat = glm::rotate(transformationMat, glm::radians(rot_z), glm::vec3(0.0f, 0.0f, 1.0f));
		transformationMat = glm::scale(transformationMat, glm::vec3(scale_x, scale_y, scale_z));

		// Get the view and projection matrices
		glm::mat4 viewMat = cam->getViewMatrix();
		glm::mat4 projectionMat = cam->getProjectionMatrix();

		//
		glUseProgram(programId);

		// Set the uniforms in the shader
		glUniformMatrix4fv(transformUniformId, 1, GL_FALSE, glm::value_ptr(transformationMat));
		glUniformMatrix4fv(viewUniformId, 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(projectionMat));

		//
		glBindBuffer(GL_ARRAY_BUFFER, vboTriangle);

		//
		glEnableVertexAttribArray(vertexPos3DLocation);

		//
		//
		glVertexAttribPointer(vertexPos3DLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);

		//
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//
		glDisableVertexAttribArray(vertexPos3DLocation);

		//
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//
		glUseProgram(0);
	}

	//
	void TriangleRenderer::destroy() {
		glDeleteProgram(programId);

		glDeleteBuffers(1, &vboTriangle);
	}
}