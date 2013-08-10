#include "text.h"

#include <iostream>
#include <assert.h>

Text::Text(texture_font_t *font, GLuint coordPos, GLuint posLoc)
    : m_font(font),
    m_coordPos(coordPos),
    m_posLoc(posLoc),
    m_text(NULL), 
    m_x(0), 
    m_y(0),
    m_width(0),
    m_shader(0),
    m_newText(false)
{

}

Text::~Text()
{
    glDeleteBuffers(1, &m_pointsBuffer);
    glDeleteBuffers(1, &m_indicesBuffer);
    glDeleteVertexArrays(1, &m_coordPos);
}

void Text::setText(wchar_t *text) {
    m_text = text;
    m_newText = true;

    //FIXME: leaks buffers presently when you set the text to something else.
    //FIXME: also, instead of gen'ing the buffers and deleting them when this happpens,
    //FIXME: which is slow, lets use glbuffersubdata. Additionally, lets use GL_DYNAMIC_DRAW
    //delete the buffers so that compute() can remake them for the new text
//    glDeleteBuffers(1, &m_pointsBuffer);
//    glDeleteBuffers(1, &m_indicesBuffer);
//    glDeleteVertexArrays(1, &m_attribPos);
}

void Text::computeText() {
    m_points.clear();
    m_indices.clear();

    float x = 0;
    float y = 0;

    unsigned int len = wcslen(m_text);

    m_points.reserve(len * 4 * 4);
    m_indices.reserve(len * 3 * 2);

    for(unsigned int i = 0; i < len; i++) {
        texture_glyph_t *glyph = texture_font_get_glyph(m_font, m_text[i]);

        if(glyph != NULL) {
            int kerning = 0;
            if(i > 0) {
                kerning = texture_glyph_get_kerning(glyph, m_text[i-1]);
            }

            if (m_text[i] == '\n') {
                x = 0;
                y += glyph->height;
            } else {
                x += kerning;

                int x0 = x + glyph->offset_x;
                int y0 = y + glyph->offset_y;
                int x1 = x0 + glyph->width;
                int y1 = y0 - glyph->height;

                float s0 = glyph->s0;
                float t0 = glyph->t0;
                float s1 = glyph->s1;
                float t1 = glyph->t1;

                unsigned int size = m_points.size() / 4;
                m_indices.push_back(size); m_indices.push_back(size + 1); m_indices.push_back(size + 2);
                m_indices.push_back(size); m_indices.push_back(size + 2); m_indices.push_back(size + 3);
                m_points.push_back(x0); m_points.push_back(y0); m_points.push_back(s0); m_points.push_back(t0);
                m_points.push_back(x0); m_points.push_back(y1); m_points.push_back(s0); m_points.push_back(t1);
                m_points.push_back(x1); m_points.push_back(y1); m_points.push_back(s1); m_points.push_back(t1);
                m_points.push_back(x1); m_points.push_back(y0); m_points.push_back(s1); m_points.push_back(t0);

                x += glyph->advance_x;
            }
        }
    }

    m_width = x;

    glGenBuffers(1, &m_pointsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_pointsBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_points.size() * sizeof(GLfloat), m_points.data(), GL_STATIC_DRAW); 

    glGenBuffers(1, &m_indicesBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_indicesBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW); 
}

void Text::render() {
    if(m_text == NULL) {
        return;
    }

    if(m_newText) {
        computeText();
        m_newText = false;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_pointsBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBuffer);

    glVertexAttribPointer(m_coordPos, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(m_coordPos);
    glUniform2f(m_posLoc, m_x, m_y);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(m_coordPos);
}
