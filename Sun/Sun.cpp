// Sun.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cmath>
#include <GL\glew.h>
#include <gl\glut.h>
#include <vector>
#include "ShaderReader.h"
#include "Windows.h"
#include <iostream>

GLuint vrHandles[2];
GLuint renderTarget, bluredVB, renderTexture;

float positionData[18] = { -1.5f, -1, 0.5f,
-1.5f, 5, 0.5f,
1.5f, -1, 0.5f,
1.5f, -1, 0.5f,
-1.5f, 5, 0.5f,
1.5f, 5, 0.5f,
};

float colorData[] = {
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f,0.0f, 0.0f };

GLuint programHandle, haloProgramHandle, bluredProgramHandle;
SYSTEMTIME time;

void glutTimer(int value)
{
	static float i = 5;
		GLint time1 = glGetUniformLocation(programHandle, "time1");
		i += 0.5;
	glUniform1f(time1, i);
	glutPostRedisplay();
	glutTimerFunc(50, glutTimer, 0);
}
void render()
{

	glClear(GL_COLOR_BUFFER_BIT);
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, renderTarget);
	glViewport(0, 0, 512, 512);

	glBindVertexArray(vrHandles[1]);


	glLinkProgram(haloProgramHandle);
	glUseProgram(haloProgramHandle);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glFlush();


	glBindVertexArray(vrHandles[0]);


	GetSystemTime(&time);

	glLinkProgram(programHandle);
	glUseProgram(programHandle);
	glutTimerFunc(50, glutTimer, 0);


	glDrawArrays(GL_TRIANGLE_STRIP, 0, 41 * 41);
	glFlush();


	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glViewport(0, 0, 512, 512);
    glBindVertexArray(bluredVB);


	glLinkProgram(bluredProgramHandle);
	glUseProgram(bluredProgramHandle);

	GLuint texID = glGetUniformLocation(bluredProgramHandle, "renderedTexture");

	glUniform1i(texID, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderTexture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
				GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);


	glDrawArrays(GL_TRIANGLES, 0, 6);
	glFlush();

	
	glutSwapBuffers();

}

std::vector<GLfloat> generateSphere()
{
	std::vector<GLfloat> ballVerts;

	for (int i = 0; i <= 40; i++)
	{
		double lat0 = 3.14 * (-0.5 + (double)(i - 1) / 40);
		double z0 = sin(lat0);
		double zr0 = cos(lat0);

		double lat1 = 3.14 * (-0.5 + (double)i / 40);
		double z1 = sin(lat1);
		double zr1 = cos(lat1);

		for (int j = 0; j <= 40; j++)
		{
			double lng = 2 * 3.14 * (double)(j - 1) / 40;
			double x = cos(lng);
			double y = sin(lng);

			ballVerts.push_back(x * zr0); //X
			ballVerts.push_back(y * zr0); //Y
			ballVerts.push_back(z0);      //Z

			ballVerts.push_back(x * zr1); //X
			ballVerts.push_back(y * zr1); //Y
			ballVerts.push_back(z1);      //Z

		}
	}

	return ballVerts;
}


GLuint createRenderTarget()
{
	GLuint frameBuffer = 0;

	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glGenTextures(1, &renderTexture);
	glBindTexture(GL_TEXTURE_2D, renderTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTexture, 0);

	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return 0;

	return frameBuffer;

}

GLuint createBlurVertexBuffer()
{
	GLuint quad_VertexArrayID;
	glGenVertexArrays(1, &quad_VertexArrayID);
	glBindVertexArray(quad_VertexArrayID);

	static const GLfloat g_quad_vertex_buffer_data[] = {
		-2.5f, -1, 0.5f,
		-2.5f, 5, 0.5f,
		1.5f, -1, 0.5f,
		1.5f, -1, 0.5f,
		-2.5f, 5, 0.5f,
		1.5f, 5, 0.5f,
	};

	GLuint quad_vertexbuffer;
	glGenBuffers(1, &quad_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(0);

	return quad_VertexArrayID;
}
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{


	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutCreateWindow("The Sun");

	GLenum gl_err = glewInit();
	
	CShaderReader shaderReader;
	GLuint vertShader = shaderReader.LoadVertexShader("vertex.glsl");

	GLuint fragShader = shaderReader.LoadFragmentShader("fragment.glsl");
	if (0 == fragShader)
	{
		cout << "Error creating fragment shader.\n";
	}

	GLuint vertHaloShader = shaderReader.LoadVertexShader("halo_vertex.glsl");

	GLuint fragHaloShader = shaderReader.LoadFragmentShader("halo_fragment.glsl");
	if (0 == fragHaloShader)
	{
		std::cout << "Error creating vertex shader.\n";
	}


	glGenVertexArrays(2, vrHandles);
	glBindVertexArray(vrHandles[0]);

	GLuint bufHandles[1];

	glGenBuffers(1, bufHandles);

	std::vector<GLfloat> sphere = generateSphere();
	glBindBuffer(GL_ARRAY_BUFFER, bufHandles[0]);
	glBufferData(GL_ARRAY_BUFFER, 41 * 41 * 6 * sizeof(float), &sphere[0], GL_STATIC_DRAW);


	programHandle = glCreateProgram();
	glAttachShader(programHandle, vertShader);
	glAttachShader(programHandle, fragShader);

	glLinkProgram(programHandle);
	glUseProgram(programHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(0);


	glBindVertexArray(vrHandles[1]);

	GLuint haloHandle;
	glGenBuffers(1, &haloHandle);
	glBindBuffer(GL_ARRAY_BUFFER, haloHandle);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), positionData, GL_STATIC_DRAW);
	haloProgramHandle = glCreateProgram();
	glAttachShader(haloProgramHandle, vertHaloShader);
	glAttachShader(haloProgramHandle, fragHaloShader);

	glLinkProgram(haloProgramHandle);
	glUseProgram(haloProgramHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(0);

	renderTarget = createRenderTarget();

	bluredVB = createBlurVertexBuffer();

	GLuint vertBluredShader = shaderReader.LoadVertexShader("blured_vertex.glsl");

	GLuint fragBluredShader = shaderReader.LoadFragmentShader("blured_fragment.glsl");

	bluredProgramHandle = glCreateProgram();
	glAttachShader(bluredProgramHandle, vertBluredShader);
	glAttachShader(bluredProgramHandle, fragBluredShader);

	glLinkProgram(bluredProgramHandle);
	glUseProgram(bluredProgramHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	
	glEnableVertexAttribArray(0);
	glutTimerFunc(50, glutTimer, 0);

	glutDisplayFunc(render);
	

	glutMainLoop();

	return 0;
}


