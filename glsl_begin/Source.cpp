#pragma comment (lib, "glew32s.lib")
//#pragma comment (lib, "SOIL.lib")
#define GLEW_STATIC
#include<gl/glew.h>
#include<SOIL.h>
#include"shader.h"
#include<GL/freeglut.h>
//#include<iostream>
#include<glm/glm.hpp>
#include<glm/ext.hpp>
//#include<fstream>
#include<vector>
#include<IL/il.h>
#include<IL/ilu.h>
#include"texture.h"
//#include<string>

texture t1;
texture t2;

GLuint VertexArrayID;
GLuint lightVAO;
GLuint normalVAO;

glm::mat4 View, Model, Projection;

GLint modelLoc, viewLoc, projLoc;

GLuint lightcolor_loc, materialcolor_loc;
GLuint lightposLoc;
GLint viewPosLoc;
GLuint MatrixID;
GLuint textureLoc;
glm::mat4 mvp;

GLuint imageID;

GLuint VertexBuffer;
GLuint VertexBuffer2;
GLuint normalBuffer;

//diffuse_map
GLuint texture1;
//specular_map
GLuint texture2;

shader shader_main;
shader shader_norm;
shader shader_light;
//GLuint g_ShaderProgram = 0;
//glGenVertexArrays(1, &VertexArrayID);

bool use_normals = false;
// Camera Position
float camX, camY, camZ;

// Mouse Tracking Variables
int startX, startY, tracking = 0;

// Camera Spherical Coordinates
float alpha = 40.0f, beta = 45.0f;
float r = 5.25f;

// Light attributes
glm::vec3 lightPos(0.0,1.0,-1.5);

/*GLuint LoadShader(GLenum shaderType, const std::string& shaderFile)
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
*/


// Create a shader program from a set of compiled shader objects.
/*GLuint CreateShaderProgram(std::vector<GLuint> shaders)
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
*/

//shader s;



void display1()
{
	// Camera matrix
	View = glm::lookAt(
		glm::vec3(camX, camY, camZ), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	//glUseProgram(g_ShaderProgram);
	//shader_main.Use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	

	

	
	shader_main.Use();
	//GLuint MatrixID = glGetUniformLocation(g_ShaderProgram, "MVP");
	// Send our transformation to the currently bound shader, in the "MVP" uniform
	// Get the uniform locations
	modelLoc = glGetUniformLocation(shader_main.program, "model");
	viewLoc = glGetUniformLocation(shader_main.program, "view");
	projLoc = glGetUniformLocation(shader_main.program, "projection");

	lightcolor_loc = glGetUniformLocation(shader_main.program, "lightColor");
	materialcolor_loc = glGetUniformLocation(shader_main.program, "materialcolor");
	lightposLoc = glGetUniformLocation(shader_main.program, "lightPos");
	viewPosLoc = glGetUniformLocation(shader_main.program, "viewPos");

	//textureLoc = glGetUniformLocation(shader_main.program,"mytexture");

	glUniform3f(viewPosLoc, 5.25 * sin(40 * 3.14f / 180.0f) * cos(45 * 3.14f / 180.0f), 5.25 * cos(40 * 3.14f / 180.0f) * cos(45 * 3.14f / 180.0f), 5.25*sin(45 * 3.14f / 180.0f));
	//glUniform3f(viewPosLoc, camX, camY, camZ);
	// //this is for old code, This is done in the main loop since each model will have a different MVP matrix (At least for the M part)

	glUniform3f(lightcolor_loc, 1, 1, 1);
	glUniform3f(materialcolor_loc, 1.0, .5, .3);
	glUniform3f(3, lightPos.x, lightPos.y, lightPos.z);
	Model = glm::mat4(1.0f);
	// Pass the matrices to the shader
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(View));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(Projection));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(Model));

	//glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);


	/**When we are using multiple texture it s neccessary to select the texture #no. by glActiveTexture(),
	followed by binding the texture, but in this case we are using single texture it s not needed**/

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glUniform1i(glGetUniformLocation(shader_main.program, "material.diffuse"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(shader_main.program, "material.specular"), 1);

	//glUniform1i(textureLoc, texture);
	
	

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
	//glFlush();

	/*
	shader_norm.Use();


	modelLoc = glGetUniformLocation(shader_norm.program, "model");
	viewLoc = glGetUniformLocation(shader_norm.program, "view");
	projLoc = glGetUniformLocation(shader_norm.program, "projection");

	// Set matrices

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(View));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(Projection));
	//Model = glm::mat4();
	//	Model = glm::translate(Model, lightPos);
	//	Model = glm::scale(Model, glm::vec3(0.1f)); // Make it a smaller cube
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(Model));

	// Draw the light object (using light's vertex attributes)
	glBindVertexArray(normalVAO);
	glDrawArrays(GL_LINES, 0, 72);
	glBindVertexArray(0);



	shader_light.Use();


	modelLoc = glGetUniformLocation(shader_light.program, "model");
	viewLoc = glGetUniformLocation(shader_light.program, "view");
	projLoc = glGetUniformLocation(shader_light.program, "projection");

	// Set matrices
	// Camera matrix
	View = glm::lookAt(
	glm::vec3(5.25 * sin(40 * 3.14f / 180.0f) * cos(45 * 3.14f / 180.0f), 5.25 * cos(40 * 3.14f / 180.0f) * cos(45 * 3.14f / 180.0f), 5.25*sin(45 * 3.14f / 180.0f)), // Camera is at (4,3,3), in World Space
	glm::vec3(0, 0, 0), // and looks at the origin
	glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(View));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(Projection));
	Model = glm::mat4();
	Model = glm::translate(Model, lightPos);
	Model = glm::scale(Model, glm::vec3(0.1f)); // Make it a smaller cube
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(Model));

	// Draw the light object (using light's vertex attributes)
	
	glBindVertexArray(lightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	*/
	


	glutSwapBuffers();
}


void init() {

	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);


/*	ILboolean success;

	ilInit();
	std::string filename = "wooden_texture.jpg";
	// generate an image name
	ilGenImages(1, &imageID);
	// bind it
	ilBindImage(imageID);
	// match image origin to OpenGL’s
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	// load  the image
	success = ilLoadImage((ILstring)filename.c_str());
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	// check to see if everything went OK
	if (!success) {
		ilDeleteImages(1, &imageID);
		std::cout << "Image not loaded";
		//return 0;
	}
	else {
		std::cout << "Image Loaded Successfully !!!";
	}


	GLint Width, Height;
	Width = ilGetInteger(IL_IMAGE_WIDTH);
	Height = ilGetInteger(IL_IMAGE_HEIGHT);

	std::cout << "Width" << Width << "\n";
	std::cout << "Height" << Height << "\n";
	ilBindImage(imageID);
	unsigned char* Data = ilGetData();

	//**it is used above all many vertex||color bind,databuffer and used this VetexarrayID at display func 
	//**where we use shader object and drawarray or element

	int width, height;
	unsigned char* image = SOIL_load_image("img_test.bmp",&width,&height,0,SOIL_LOAD_RGB);
//	std::cout << "Height" << height;
	//std::cout << "Width" << width;
	*/
	glGenTextures(1, &texture1);

	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t1.Width, t1.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, t1.Data);


	//**Atleast one parameter is required**//

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glGenTextures(1, &texture2);

	glBindTexture(GL_TEXTURE_2D,texture2);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t2.Width, t2.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, t2.Data);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//	glGenerateMipmap(GL_TEXTURE_2D);

	
//	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint TexCoorBuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &VertexBuffer);
	glGenBuffers(1, &normalBuffer);
	glGenBuffers(1, &TexCoorBuffer);



	// set the camera position based on its spherical coordinates
	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r *   						     sin(beta * 3.14f / 180.0f);


	// some GL settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//Not neccessary to be written inside init(),usually written in main()

	//So we need three 3D points in order to make a triangle
	static const GLfloat g_Vertex_Buffer_data[] = {
		-1.0f,-1.0f,0.0f,
		1.0f,-1.0f,0.0f,
		0.0f,1.0f,0.0f,
	};

	GLfloat nor_vertices[] = {
		
		0.5f,  0.5f,  0.5f, 1.0f,  0.5f,  0.5f,
		0.5f,  0.5f, -0.5f, 1.0f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f, 1.0f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f, 1.0f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f, 1.0f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f, 1.0f,  0.5f,  0.5f,



		-0.5f,  0.5f,  0.5f, -1.0f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f, -1.0f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f, -1.0f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f, -1.0f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.5f,  0.5f,

		- 0.5f, -0.5f,  0.5f, -0.5f, -0.5f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  1.0f,
		0.5f,  0.5f,  0.5f, 0.5f,  0.5f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  1.0f,
		-0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  1.0f,
		-0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  1.0f,

		-0.5f, -0.5f, -0.5f, -0.5, -0.5f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.5f, 0.5f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.5f, 0.5f, -1.0f,
		-0.5f,  0.5f, -0.5f,  -0.5f, 0.5f, -1.0f,
		-0.5f, -0.5f, -0.5f,  -0.5f, -0.5f, -1.0f,
	
		

	
		-0.5f, -0.5f, -0.5f, -0.5f, -1.0f, -0.5f,
		0.5f, -0.5f, -0.5f,  0.5f, -1.0f, -0.5f,
		0.5f, -0.5f,  0.5f,  0.5f, -1.0f,  0.5f,
		0.5f, -0.5f,  0.5f,  0.5f, -1.0f,  0.5f,
		-0.5f, -0.5f,  0.5f,-0.5f, -1.0f,  0.5f,
		-0.5f, -0.5f, -0.5f,-0.5f,-1.0f, -0.5f,

		-0.5f,  0.5f, -0.5f,-0.5f,  1.0f, -0.5f,
		0.5f,  0.5f, -0.5f, 	0.5f,  1.0f, -0.5f,
		0.5f,  0.5f,  0.5f,0.5f,  1.0f,  0.5f,
		0.5f,  0.5f,  0.5f, 0.5f,  1.0f,  0.5f,
		-0.5f,  0.5f,  0.5f, -0.5f,  1.0f,  0.5f,
		-0.5f,  0.5f, -0.5f, -0.5f,  1.0f, -0.5f
		
	};

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {


		// Positions          // Normals           // Texture Coords

		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	GLfloat TexCoord[] = {
		1.0f,1.0f,
		1.0f,0.0f,
		0.0f,1.0f,
		0.0f,1.0f,
		0.0f,0.0f,
		1.0f,0.0f,

		1.0f,1.0f,
		1.0f,0.0f,
		0.0f,1.0f,
		0.0f,1.0f,
		0.0f,0.0f,
		1.0f,0.0f,

		1.0f,1.0f,
		1.0f,0.0f,
		0.0f,1.0f,
		0.0f,1.0f,
		0.0f,0.0f,
		1.0f,0.0f,

		1.0f,1.0f,
		1.0f,0.0f,
		0.0f,1.0f,
		0.0f,1.0f,
		0.0f,0.0f,
		1.0f,0.0f,

		1.0f,1.0f,
		1.0f,0.0f,
		0.0f,1.0f,
		0.0f,1.0f,
		0.0f,0.0f,
		1.0f,0.0f,

		1.0f,1.0f,
		1.0f,0.0f,
		0.0f,1.0f,
		0.0f,1.0f,
		0.0f,0.0f,
		1.0f,0.0f

	};


	
	//need to create a Vertex Array Object 
	glGenVertexArrays(1, &normalVAO);
	glGenVertexArrays(1, &VertexArrayID);

	glBindVertexArray(VertexArrayID);

	//bind & data comes together//not compulsory..
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

	//**Data entering starts from here**//

	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);



	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//glBindBuffer(GL_ARRAY_BUFFER, TexCoorBuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoord), TexCoord, GL_STATIC_DRAW);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))

	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0); // Note that we skip over the normal vectors
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	glBindVertexArray(normalVAO);
	
	glBindBuffer(GL_ARRAY_BUFFER,normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(nor_vertices), nor_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	//second vertex buffer for inverted triangle
	//how to display them


	Projection = glm::perspective(glm::radians(45.0f), (float)640 / (float)480, 0.1f, 100.0f);

	// Or, for an ortho camera :
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates



	// Model matrix : an identity matrix (model will be at the origin)
	Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around



}
//void runMainLoop(int val) {
//	display1();
//	glutTimerFunc(1000 / 60, runMainLoop, 0);
//}

void processKeys(unsigned char key, int xx, int yy)
{
	switch (key) {

	case 27:
		glutLeaveMainLoop();
		break;

	case 'c':
		printf("Camera Spherical Coordinates (%f, %f, %f)\n", alpha, beta, r);
		break;

	}

	//  uncomment this if not using an idle func
	glutPostRedisplay();
}
// Mouse Events
//

void processMouseButtons(int button, int state, int xx, int yy)
{
	// start tracking the mouse
	if (state == GLUT_DOWN) {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
	}

	//stop tracking the mouse
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha -= (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			r += (yy - startY) * 0.01f;
			if (r < 0.1f)
				r = 0.1f;
		}
		tracking = 0;
	}
}

// Track mouse motion while buttons are pressed

void processMouseMotion(int xx, int yy)
{

	int deltaX, deltaY;
	float alphaAux, betaAux;
	float rAux;

	deltaX = -xx + startX;
	deltaY = yy - startY;

	// left mouse button: move camera
	if (tracking == 1) {


		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0f)
			betaAux = 85.0f;
		else if (betaAux < -85.0f)
			betaAux = -85.0f;
		rAux = r;
	}
	// right mouse button: zoom
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r + (deltaY * 0.01f);
		if (rAux < 0.1f)
			rAux = 0.1f;
	}



	camX = rAux * sin(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	camZ = rAux * cos(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	camY = rAux *   						       sin(betaAux * 3.14f / 180.0f);

	//  uncomment this if not using an idle func
	glutPostRedisplay();
}

int main(int argc, char** argv)
{


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowSize(640, 480);
	glutCreateWindow("simple rectangle");


	glewExperimental = true; // Needed in core profile


	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	t1.loadtexture("container2.png");
	t2.loadtexture("container1.png");
	//shader shader_main;
	shader_main.loadshader("vertexshader.vert", "fragmentshader.frag");
	shader_light.loadshader("ver_lamp.vert", "frag_lamp.frag");
	shader_norm.loadshader("ver_normal.vert", "frag_normal.frag");
	init();
	// glEnable(GL_DEPTH_TEST);
	//load shaders
	//GLuint vertexShader = LoadShader(GL_VERTEX_SHADER, "vertexshader.vert");
	//GLuint fragmentShader = LoadShader(GL_FRAGMENT_SHADER, "fragmentshader.frag");

	ilInit();

	//std::vector<GLuint> shaders;
	//shaders.push_back(vertexShader);
	//shaders.push_back(fragmentShader);

	// Create the shader program.
	//g_ShaderProgram = CreateShaderProgram(shaders);
	//assert(g_ShaderProgram != 0);

	//	Mouse and Keyboard Callbacks
	glutKeyboardFunc(processKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);

	//	glutMouseWheelFunc(mouseWheel);
	//	return from main loop
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);





	//MatrixID = glGetUniformLocation(shader_main.program, "MVP");
	


	glutDisplayFunc(display1);
	//glutIdleFunc(display1);


	//glutTimerFunc(1000 / 60, runMainLoop, 0);

	glutMainLoop();

}