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

GLuint MatrixID;
glm::mat4 mvp;

GLuint VertexBuffer;
GLuint VertexBuffer2;

GLuint g_ShaderProgram = 0;
//glGenVertexArrays(1, &VertexArrayID);


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

/*
GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path) {




	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

*/
//GLuint programID = LoadShaders("vertexshader.vertexshader", "fragmentshader.fragmentshader");

   

void display1()
{
	
	glUseProgram(g_ShaderProgram);
	
	//GLuint MatrixID = glGetUniformLocation(g_ShaderProgram, "MVP");
	// Send our transformation to the currently bound shader, in the "MVP" uniform
	// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnableVertexAttribArray(0);

	//glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer2);



	glVertexAttribPointer(//layout location of vertexPosition_modelspace must be same,that is set for glenablever..
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
	glFlush();
}


void init() {

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);


	//**it is used above all many vertex||color bind,databuffer and used this VetexarrayID at display func 
	//**where we use shader object and drawarray or element


	//why we are usig this ? 
	//need to create a Vertex Array Object 

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	//Not neccessary to be written inside init(),usually written in main()
	


	//So we need three 3D points in order to make a triangle
	static const GLfloat g_Vertex_Buffer_data[] = {
		-1.0f,-1.0f,0.0f,
		1.0f,-1.0f,0.0f,
		0.0f,1.0f,0.0f,

	};

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &VertexBuffer);

	//bind & data comes together//not compulsory..
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_Vertex_Buffer_data), g_Vertex_Buffer_data, GL_STATIC_DRAW);

	//second vertex buffer for inverted triangle
	//how to display them
	static const GLfloat g_Vertex_Buffer_data2[] = {
		-1.0f,1.0f,0.0f,
		1.0f,1.0f,0.0f,
		0.0f,-1.0f,0.0f,
	};

	glGenBuffers(1, &VertexBuffer2);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer2);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_Vertex_Buffer_data2), g_Vertex_Buffer_data2, GL_STATIC_DRAW);


	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)640 / (float)480, 0.1f, 100.0f);

	// Or, for an ortho camera :
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
	
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around

	 
	// std::cout << MatrixID;
	 //glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

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

	//load shaders
	GLuint vertexShader = LoadShader(GL_VERTEX_SHADER, "vertexshader.vert");
	GLuint fragmentShader = LoadShader(GL_FRAGMENT_SHADER, "fragmentshader.frag");
	
	std::vector<GLuint> shaders;
	shaders.push_back(vertexShader);
	shaders.push_back(fragmentShader);

	// Create the shader program.
	g_ShaderProgram = CreateShaderProgram(shaders);
	//assert(g_ShaderProgram != 0);
	MatrixID = glGetUniformLocation(g_ShaderProgram, "MVP");

	glutDisplayFunc(display1);
	//glutTimerFunc(1000 / 60, runMainLoop, 0);
	
	glutMainLoop();
	
}
