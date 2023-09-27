#include "postprocessing.h"

const float PostProcessing::rectangleVerts[] = {
	// Coords    // texCoords
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f,

	 1.0f,  1.0f,  1.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f
};

PostProcessing::PostProcessing()
{
    width = 0;
    height = 0;
}

void PostProcessing::Initialize(int _width, int _height, const char* vShader_Path, const char* fShader_Path)
{
    width = _width;
    height = _height;

    // Prepare framebuffer rectangle VBO and VAO
	unsigned int rectVBO;
	glGenVertexArrays(1, &rectVAO);
	glGenBuffers(1, &rectVBO);
	glBindVertexArray(rectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVerts), &rectangleVerts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// Create Frame Buffer Object
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Create Framebuffer Texture
	glGenTextures(1, &framebufferTexture);
	glBindTexture(GL_TEXTURE_2D, framebufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

	// Create Render Buffer Object
	unsigned int RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	// Error checking framebuffer
	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer error: " << fboStatus << std::endl;

	// Create _program
	std::string vFrameBuffer;
	std::string fFrameBuffer;
    loadFileToBuffer(vShader_Path, vFrameBuffer);
    loadFileToBuffer(fShader_Path, fFrameBuffer);

	framebuffer_shader = new PostProcessing_Shader();
	framebuffer_shader->CreateProgram(vFrameBuffer, fFrameBuffer);

	// get screen resolution uniform locations
	uniformScreenResolution = glGetUniformLocation(framebuffer_shader->program(), "screenResolution");
}

void PostProcessing::StartProcessing()
{
	// Bind the custom framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Enable depth testing since it's disabled when drawing the framebuffer rectangle
	glEnable(GL_DEPTH_TEST);
}

void PostProcessing::EndProcessing()
{
	// Bind the default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Draw the framebuffer rectangle
    framebuffer_shader->bind();

	// glUniform1i(uniformScreenWidth, width);
	// glUniform1i(uniformScreenHeight, height);

	glUniform2f(uniformScreenResolution, width, height);

	glBindVertexArray(rectVAO);
	glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
	glBindTexture(GL_TEXTURE_2D, framebufferTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

PostProcessing::~PostProcessing()
{

}

// PostProcessing Shader class
PostProcessing_Shader::PostProcessing_Shader()
{
    _program = 0;
}

void PostProcessing_Shader::CreateProgram(std::string vShader, std::string fShader)
{
    compile(vShader.c_str(), fShader.c_str());
}

void PostProcessing_Shader::compile(const char* vShaderCode, const char* fShaderCode)
{
    _program = glCreateProgram();

	if (!_program) {
		std::cerr << "Error creating _program program\n";
		return;
	}

	addShader(_program, vShaderCode, GL_VERTEX_SHADER);
	addShader(_program, fShaderCode, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar errLog[1024] = { 0 };

	glLinkProgram(_program);
	glGetProgramiv(_program, GL_LINK_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(_program, sizeof(errLog), NULL, errLog);
		std::cerr << "Error linking program: '" << errLog << "'\n";
		return;
	}

	glValidateProgram(_program);
	glGetProgramiv(_program, GL_VALIDATE_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(_program, sizeof(errLog), NULL, errLog);
		std::cerr << "Error validating program: '" << errLog << "'\n";
		return;
	}
}

void PostProcessing_Shader::addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar errLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(errLog), NULL, errLog);
		std::cerr << "Error compiling the " << shaderType << " _program: '" << errLog << "'\n";
		return;
	}

	glAttachShader(theProgram, theShader);
}

PostProcessing_Shader::~PostProcessing_Shader()
{

}