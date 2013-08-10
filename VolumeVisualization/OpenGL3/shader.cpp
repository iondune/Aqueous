/*
 *      GWEN
 *      Copyright (c) Shaun Reich <sreich@kde.org>
 *      See license in Gwen.h
 */

#include "shader.h"

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <vector>

namespace Gwen {

Shader::Shader(const char* vertexShader, const char* fragmentShader)
	: m_shaderProgram(0), m_vertexShader(0), m_fragmentShader(0)
{
    loadShaders(vertexShader, fragmentShader);

    printShaderInfoLog(m_vertexShader);
    printShaderInfoLog(m_fragmentShader);
}

Shader::~Shader()
{
    glDeleteProgram(m_shaderProgram);
    glDeleteShader(m_vertexShader);
    glDeleteShader(m_fragmentShader);
}

void Shader::bindProgram() const
{
    glUseProgram(m_shaderProgram);
}

void Shader::unbindProgram() const
{
    glUseProgram(0);
}

GLuint Shader::shaderProgram() const
{
    return m_shaderProgram;
}

GLuint Shader::vertexShader() const
{
    return m_vertexShader;
}

GLuint Shader::fragmentShader() const
{
    return m_fragmentShader;
}

// loadFile - loads text file into char* fname
// allocates memory - so need to delete after use
// size of file returned in fSize
char* Shader::loadFile(const char* fname, GLint* fSize)
{
    std::ifstream::pos_type size;
    char * memblock = 0;
    std::string text;

    // file read based on example in cplusplus.com tutorial
    std::ifstream file(fname, std::ios::in | std::ios::binary | std::ios::ate);
    if (file.is_open()) {
        size = file.tellg();
        *fSize = (GLuint) size;

        memblock = new char [(size_t) size];
        file.seekg(0, std::ios::beg);
        file.read(memblock, size);
        file.close();
        text.assign(memblock);

        std::cout << "shader : " << fname << " loaded successfully\n";
    } else {
        std::cout << "failed to load shader: " << fname << "\n";
    }

    return memblock;
}

void Shader::loadShaders(const char* vertexShader, const char* fragmentShader)
{
    m_vertexShader = glCreateShader(GL_VERTEX_SHADER);

    GLint vertLength;
    GLint fragLength;

    char* vertSource;
    char* fragSource;

    vertSource = loadFile(vertexShader, &vertLength);
    fragSource = loadFile(fragmentShader, &fragLength);

    const char* vertSourceConst = vertSource;
    const char* fragSourceConst = fragSource;

    glShaderSource(m_vertexShader, 1, &vertSourceConst, &vertLength);
    glCompileShader(m_vertexShader);

    if (!checkShaderCompileStatus(m_vertexShader)) {
        std::cout << "vertex shader failed to compile properly\n";
        assert(0);
    } else {
        std::cout << "vertex shader compiled!\n";
    }

    m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(m_fragmentShader, 1, &fragSourceConst, &fragLength);
    glCompileShader(m_fragmentShader);

    if (!checkShaderCompileStatus(m_fragmentShader)) {
        std::cout << "fragment shader failed to compile properly\n";
    } else {
        std::cout << "fragment shader compiled!\n";
    }

    m_shaderProgram = glCreateProgram();

    // attach shaders
    glAttachShader(m_shaderProgram, m_vertexShader);
    glAttachShader(m_shaderProgram, m_fragmentShader);

    // link the program and check for errors
    glLinkProgram(m_shaderProgram);

    if (checkProgramLinkStatus(m_shaderProgram)) {
        std::cout << "shader program linked!\n";
    } else {
        std::cout << "shader program link FAILURE\n";
    }

    delete [] vertSource;
    delete [] fragSource;
}

bool Shader::checkShaderCompileStatus(GLuint obj)
{
    GLint status;
    glGetShaderiv(obj, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
        GLint length;

        glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &length);

        std::vector<char> log(length);
        glGetShaderInfoLog(obj, length, &length, &log[0]);

        std::cout << &log[0] << "\n";
        return false;
    }
    return true;
}

bool Shader::checkProgramLinkStatus(GLuint obj)
{
    GLint status;
    glGetProgramiv(obj, GL_LINK_STATUS, &status);

    if (status == GL_FALSE) {
        GLint length;

        glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &length);

        std::vector<char> log(length);

        glGetProgramInfoLog(obj, length, &length, &log[0]);

        std::cout << &log[0] << "\n";
        return false;
    }
    return true;
}

void Shader::printShaderInfoLog(GLuint shader)
{
    int infoLogLen = 0;
    int charsWritten = 0;
    GLchar *infoLog;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

    // should additionally check for OpenGL errors here

    if (infoLogLen > 0) {
        infoLog = new GLchar[infoLogLen];
        glGetShaderInfoLog(shader, infoLogLen, &charsWritten, infoLog);

        std::cout << "Shader info log: " << infoLog << "\n";

        delete [] infoLog;
    }

    // should additionally check for OpenGL errors here
}

}
