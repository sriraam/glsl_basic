#pragma comment (lib, "glew32s.lib")
#define GLEW_STATIC
//#include<gl/glew.h>
#include<GL/glew.h>
#include<GL/freeglut.h>
#include<iostream>
#include<glm/glm.hpp>
#include<glm/ext.hpp>
#include<fstream>
#include<vector>
#include<string>

GLuint VertexArrayID;

glm::mat4 View, Model, Projection;

GLint modelLoc, viewLoc, projLoc;

GLuint lightcolor_loc,materialcolor_loc;
GLuint lightposLoc;
GLuint MatrixID;
glm::mat4 mvp;

GLuint VertexBuffer;
GLuint VertexBuffer2;

GLuint g_ShaderProgram = 0;
//glGenVertexArrays(1, &VertexArrayID);

// Light attributes
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

GLuint LoadShader(GLenum shaderType, const std::string& shaderFile)
{
	std::ifstream ifs;

	// Load the shader.
	ifs.open(shaderFile);

	if (!ifs)
	{
		std::cerr << "Can not open shader file: \"" << shaderFile << "\"" << std::endl;
		return 0;
	}

	std::string source(std::istreambuf_iterator<char>(ifs), (std::istreambuf_iterator<char>()));
	ifs.close();

	// Create a shader object.
	GLuint shader = glCreateShader(shaderType);

	// Load the shader source for each shader object.
	const GLchar* sources[] = { source.c_str() };
	glShaderSource(shader, 1, sources, NULL);

	// Compile the shader.
	glCompileShader(shader);

	// Check for errors
	GLint compileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE)
	{
		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* infoLog = new GLchar[logLength];
		glGetShaderInfoLog(shader, logLength, NULL, infoLog);

//#ifdef _WIN32
	//	OutputDebugString(infoLog);
//#else
		std::cerr << infoLog << std::endl;
//#endif
		delete infoLog;
		return 0;
	}

	return shader;
}



// Create a shader program from a set of compiled shader objects.
GLuint CreateShaderProgram(std::vector<GLuint> shaders)
{
	// Create a shader program.
	GLuint program = glCreateProgram();

	// Attach the appropriate shader objects.
	for (GLuint shader : shaders)
	{
		glAttachShader(program, shader);
	}

	// Link the program
	glLinkProgram(program);

	// Check the link status.
	GLint linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE)
	{
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* infoLog = new GLchar[logLength];

		glGetProgramInfoLog(program, logLength, NULL, infoLog);

//#ifdef _WIN32
	//	OutputDebugString(infoLog);
//#else
		std::cerr << infoLog << std::endl;
//#endif

		delete infoLog;
		return 0;
	}

	return program;
}



void display1()
{
	
	glUseProgram(g_ShaderProgram);
	
	//GLuint MatrixID = glGetUniformLocation(g_ShaderProgram, "MVP");
	// Send our transformation to the currently bound shader, in the "MVP" uniform
	// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
	glUniform3f(lightcolor_loc, 1, 1, 1);
	glUniform3f(materialcolor_loc,1.0,.5,.3);
	glUniform3f(lightposLoc,lightPos.x,lightPos.y,lightPos.z);

	// Pass the matrices to the shader
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(View));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(Projection));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(Model));

	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	//glEnableVertexAttribArray(0);

	//glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	
	glBindVertexArray(VertexArrayID);


	/*glVertexAttribPointer(//layout location of vertexPosition_modelspace must be same,that is set for glenablever..
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3*sizeof(float),                  // stride
		(void*)0            // array buffer offset
	);*/
	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 36); // Starting from vertex 0; 3 vertices total -> 1 triangle
	//glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	glFlush();
}


void init() {

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);


	//**it is used above all many vertex||color bind,databuffer and used this VetexarrayID at display func 
	//**where we use shader object and drawarray or element


	//why we are usig this ? 
	//need to create a Vertex Array Object 

	glGenVertexArrays(1, &VertexArrayID);

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &VertexBuffer);


	//Not neccessary to be written inside init(),usually written in main()
	
	//lightcolor_loc=glGetUniformLocation(g_ShaderProgram,"lightcolor");

//	glUniform3f(lightcolor_loc, 1, 1, 1);

	//materialcolor_loc = glGetUniformLocation(g_ShaderProgram, "materialcolor");

	//glUniform3f(lightcolor_loc, 1, 1, 1);


	//So we need three 3D points in order to make a triangle
	static const GLfloat g_Vertex_Buffer_data[] = {
		-1.0f,-1.0f,0.0f,
		1.0f,-1.0f,0.0f,
		0.0f,1.0f,0.0f,
	};


	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

 
	

	//bind & data comes together//not compulsory..
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(VertexArrayID);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);


	//second vertex buffer for inverted triangle
	//how to display them



	 Projection = glm::perspective(glm::radians(45.0f), (float)640 / (float)480, 0.1f, 100.0f);

	// Or, for an ortho camera :
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
	
	// Camera matrix
	 View = glm::lookAt(
		glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// Model matrix : an identity matrix (model will be at the origin)
	 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around

	

}
//void runMainLoop(int val) {
//	display1();
//	glutTimerFunc(1000 / 60, runMainLoop, 0);
//}




int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitWindowSize(640, 480);
	glutCreateWindow("simple rectangle");
	glewExperimental = true; // Needed in core profile
	

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
	init();
	glEnable(GL_DEPTH_TEST);
	//load shaders
	GLuint vertexShader = LoadShader(GL_VERTEX_SHADER, "vertexshader.vert");
	GLuint fragmentShader = LoadShader(GL_FRAGMENT_SHADER, "fragmentshader.frag");
	
	std::vector<GLuint> shaders;
	shaders.push_back(vertexShader);
	shaders.push_back(fragmentShader);
	
	// Create the shader program.
	g_ShaderProgram = CreateShaderProgram(shaders);
	//assert(g_ShaderProgram != 0);

	// Get the uniform locations
	 modelLoc = glGetUniformLocation(g_ShaderProgram, "model");
	 viewLoc = glGetUniformLocation(g_ShaderProgram, "view");
	 projLoc = glGetUniformLocation(g_ShaderProgram, "projection");


	MatrixID = glGetUniformLocation(g_ShaderProgram, "MVP");
	lightcolor_loc = glGetUniformLocation(g_ShaderProgram, "lightcolor");
	materialcolor_loc = glGetUniformLocation(g_ShaderProgram, "materialcolor");
	lightposLoc = glGetUniformLocation(g_ShaderProgram, "lightpos");
	glutDisplayFunc(display1);
	//glutTimerFunc(1000 / 60, runMainLoop, 0);
	
	glutMainLoop();
	
}
