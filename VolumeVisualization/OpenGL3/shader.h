/*
 *      GWEN
 *      Copyright (c) Shaun Reich <sreich@kde.org>
 *      See license in Gwen.h
 */

#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GL/gl.h>

namespace Gwen {
/**
* Represents the vertex, fragment shader handles,
* as well as the shader program handle itself.
*
* Also has built in logging for these and binding functions.
*/
class Shader
{
public:
    explicit Shader(const char* vertexShader, const char* fragmentShader);
    ~Shader();

    GLuint shaderProgram() const;
    GLuint vertexShader() const;
    GLuint fragmentShader() const;

    void bindProgram() const;
    void unbindProgram() const;

private:

    void loadShaders(const char* vertexShader, const char* fragmentShader);
    void printShaderInfoLog(GLuint shader);
    bool checkShaderCompileStatus(GLuint obj);
    bool checkProgramLinkStatus(GLuint obj);
    char* loadFile(const char* fname, GLint* fSize);

    GLuint m_shaderProgram;
    GLuint m_vertexShader;
    GLuint m_fragmentShader;
};

}

#endif
