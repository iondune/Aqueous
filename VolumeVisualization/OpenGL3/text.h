#ifndef FONTRENDERER_H
#define FONTRENDERER_H

#include "GL/glew.h"

#include <vector>

#include "shader.h"
#include "freetype-gl/freetype-gl.h"

//FIXME: adopt freetype-gl 3.0 code from the 3.0 branch of their repo
class Text {
public:
    Text(texture_font_t *font, GLuint attribPos, GLuint posLoc);
    ~Text();

    void setText(wchar_t *text);
    void setPos(float x, float y) { m_x = x; m_y = y; }
    void render();

private:
    void computeText();

    Gwen::Shader* m_shader;

    std::vector<GLfloat> m_points;
    std::vector<GLuint> m_indices;

    GLuint m_pointsBuffer;
    GLuint m_indicesBuffer;

    GLuint m_coordPos;
    GLuint m_posLoc;

    texture_font_t *m_font;

    wchar_t *m_text;

    float m_x;
    float m_y;
    float m_width;

    bool m_newText;
};

#endif
