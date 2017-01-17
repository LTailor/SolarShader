#include "stdafx.h"
#include "ShaderReader.h"

using namespace std;

CShaderReader::CShaderReader()
{
}


CShaderReader::~CShaderReader()
{
}

GLuint CShaderReader::LoadVertexShader(char* filename)
{
	
	string shaderContent = GetShaderContent(filename);
	GLuint shader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* codeArray[] = { shaderContent.data() };
	glShaderSource(shader, 1, codeArray, NULL);
	glCompileShader(shader);

	return shader;
}
GLuint CShaderReader::LoadFragmentShader(char* filename)
{
	string shaderContent = GetShaderContent(filename);
	GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* codeArray[] = { shaderContent.data() };
	glShaderSource(shader, 1, codeArray, NULL);
	glCompileShader(shader);

	return shader;
}

string CShaderReader::GetShaderContent(const char* filename)
{
	ifstream shaderFile(filename);
	char line[1024] = {};
	string shaderContent;

	if (!shaderFile)
	{
		printf("Error opening shader");
	}

	bool eof = false;
	do
	{
		eof = shaderFile.getline(line, 1024).eof();
		shaderContent.append(string(line));
		memset(line, 0, 1024);
	} while (!eof);

	return shaderContent;
}