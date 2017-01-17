#pragma once

#include <GL\glew.h>
#include <gl\glut.h>
#include <fstream>
#include <string>

using namespace std;

class CShaderReader
{
public:
	CShaderReader();
	~CShaderReader();

	GLuint LoadVertexShader(char* filename);
	GLuint LoadFragmentShader(char* filename);

private:

	string GetShaderContent(const char* filename);
};

