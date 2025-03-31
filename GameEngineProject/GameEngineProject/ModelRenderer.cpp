#include "ModelRenderer.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace GE {
	// Represents a vertex in the engine
	struct Vertex {
		// Location
		float x, y, z;
		float u, v;

		// Constructors
		// Sets the vertex to passed values
		Vertex(float _x, float _y, float _z, float _u, float _v) {
			// Location
			x = _x;
			y = _y;
			z = _z;

			// Colour
			u = _u;
			v = _v;
		}

		// Sets vertex to origin and no colour
		Vertex() {
			x = y = z = 0.0f;
			u = v = 0.0f;
		}
	};
	// Define the triangle's vertices
	//Vertex vertexDataM[] = {
	//			// xyz coords      rgba
	//	Vertex(-1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
	//	Vertex(1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
	//	Vertex(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f)
	//};

	/*GLfloat vertexData[] = {
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};*/

	//
	ModelRenderer::ModelRenderer()
	{
		// Initialise location, rotation and scale to default values
		pos_x = pos_y = pos_z = 0.0f;
		rot_x = rot_y = rot_z = 0.0f;
		scale_x = scale_y = scale_z = 1.0f;

		programId = 0;

		vertexLocation = 0;

		vertexUVLocation = 0;

		transformUniformId = 0;
		viewUniformId = 0;
		projectionUniformId = 0;
	}

	//
	ModelRenderer::~ModelRenderer()
	{

	}

	//
	//
	void displayShaderCompilerErrorM(GLuint shaderId) {
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

	void ModelRenderer::init() {
		//
		//
		//
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		//
		const GLchar* V_ShaderCode[] = {
			"#version 140\n"
			"in vec3 vertexPos3D;\n"
			"in vec2 vUV;\n"
			"out vec2 uv;\n"
			"uniform mat4 viewMat;\n"
			"uniform mat4 projMat;\n"
			"uniform mat4 transformMat;\n"
			"out float fog_amount;\n"
			"uniform float fog_density;\n"
			"void main() {\n"
			/*"vec4 v = vec4(vertexPos3D.xyz, 1);\n"
			"v = projMat * viewMat * transformMat * v;\n"
			"gl_Position = v;"*/
			"gl_Position = projMat * viewMat * transformMat * vec4(vertexPos3D.x, vertexPos3D.y, vertexPos3D.z, 1);\n"
			//"vec4 posInWorld = transformMat * gl_Position;\n"
			"vec4 v = vec4(vertexPos3D.xyz, 1);\n"
			"vec4 posInWorld = transformMat * gl_Position;\n"
			"uv = vUV;\n"
			"vec4 pos_rel_eye = viewMat * posInWorld;\n"
			"float distance = length(pos_rel_eye.xyz);\n"
			"fog_amount = 0.9 - exp(-pow(distance * fog_density, 2.0f));\n"
			"fog_amount = clamp(fog_amount, 0.0f, 1.0f);\n"
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

			displayShaderCompilerErrorM(vertexShader);

			return;
		}

		//
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		//
		const GLchar* F_ShaderCode[] = {
			"#version 140\n"
			"in vec2 uv;\n"
			"in float fog_amount;\n"
			"uniform vec3 fog_colour;\n"
			"uniform sampler2D sampler;\n"
			"out vec4 fragmentColour;\n"
			"void main()\n"
			"{\n"
			"vec4 colour = texture(sampler, uv).rgba;\n"
			"fragmentColour = mix(colour, vec4(fog_colour, 1.0f), fog_amount);\n"
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

			displayShaderCompilerErrorM(fragmentShader);

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
		vertexLocation = glGetAttribLocation(programId, "vertexPos3D");

		//
		if (vertexLocation == -1) {
			std::cerr << "Problem getting vertex3DPos" << std::endl;
		}

		// Now get a link to the vColour attribute in the vertex shader
		// so we can indicate to the pipeline where the colour data is to be 
		// inserted
		vertexUVLocation = glGetAttribLocation(programId, "vUV");

		// Check for errors
		if (vertexUVLocation == -1) {
			std::cerr << "Problem getting vUV" << std::endl;
		}

		// Get uniform id in shader so C++ program can send data to it
		viewUniformId = glGetUniformLocation(programId, "viewMat");
		projectionUniformId = glGetUniformLocation(programId, "projMat");
		// Transformation matrix uniform
		transformUniformId = glGetUniformLocation(programId, "transformMat");
		samplerId = glGetUniformLocation(programId, "sampler");

		// Link to fog uniforms
		fogColourId = glGetUniformLocation(programId, "fog_colour");

		// Exponential fog
		fogDensityId = glGetUniformLocation(programId, "fog_density");	

		// Set fog values in shader in init means the
		// fog doesn't change for the run of the  game
		// Do this if want constant fog
		glUseProgram(programId);
		glUniform1f(fogDensityId, 0.0015f);
		glm::vec3 fog_colour = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(fogColourId, 1, glm::value_ptr(fog_colour));
		// Select the program into the rendering context
		glUseProgram(0);

		//
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//
	void ModelRenderer::update() {

	}

	//
	void ModelRenderer::draw(Camera* cam, Model* model) {

		glEnable(GL_CULL_FACE);

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
		glBindBuffer(GL_ARRAY_BUFFER, model->getVertices());

		//
		glEnableVertexAttribArray(vertexLocation);

		//
		//
		glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));

		//
		glEnableVertexAttribArray(vertexUVLocation);

		//
		//
		glVertexAttribPointer(vertexUVLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));

		// Select the texture
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(samplerId, 0);
		glBindTexture(GL_TEXTURE_2D, tex->getTextureName());

		//
		glDrawArrays(GL_TRIANGLES, 0, model->getNumVertices());

		//
		glDisableVertexAttribArray(vertexLocation);

		//
		glDisableVertexAttribArray(vertexUVLocation);

		//
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//
		glUseProgram(0);

		glDisable(GL_CULL_FACE);
	}

	//
	void ModelRenderer::destroy() {
		glDeleteProgram(programId);
	}
}