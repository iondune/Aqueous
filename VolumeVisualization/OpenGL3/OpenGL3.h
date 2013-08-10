/*
	GWEN
	Copyright (c) 2011 Facepunch Studios
	See license in Gwen.h
*/

#ifndef GWEN_RENDERERS_OPENGL3_H
#define GWEN_RENDERERS_OPENGL3_H

#include "GL/glew.h"

#include "Gwen/Gwen.h"
#include "Gwen/BaseRender.h"
#include "freetype-gl/texture-atlas.h"
#include "freetype-gl/freetype-gl.h"
#include "freetype-gl/texture-font.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/swizzle.hpp>

class FontRenderer;
namespace Gwen
{

class Shader;

namespace Renderer
{

class OpenGL3 : public Gwen::Renderer::Base
{
public:

    /**
     * Create the renderer with a viewport of the given screen width/height
     */
    OpenGL3(int screenWidth, int screenHeight);
    ~OpenGL3();

    virtual void Init();

    virtual void Begin();
    virtual void End();

    virtual void SetDrawColor( Gwen::Color color );
    virtual void DrawFilledRect( Gwen::Rect rect );

    virtual void LoadFont( Gwen::Font* pFont );
    virtual void FreeFont( Gwen::Font* pFont );
    virtual void RenderText( Gwen::Font* pFont, Gwen::Point pos, const Gwen::UnicodeString& text );
    virtual Gwen::Point MeasureText( Gwen::Font* pFont, const Gwen::UnicodeString& text );

    void StartClip();
    void EndClip();

    void DrawTexturedRect( Gwen::Texture* pTexture, Gwen::Rect pTargetRect, float u1=0.0f, float v1=0.0f, float u2=1.0f, float v2=1.0f );
    void LoadTexture( Gwen::Texture* pTexture );
    void FreeTexture( Gwen::Texture* pTexture );
    Gwen::Color PixelColour( Gwen::Texture* pTexture, unsigned int x, unsigned int y, const Gwen::Color& col_default );

protected:

    void AddRect(Gwen::Rect rect, float u = 0.0f , float v = 0.0f );

    Gwen::Color			m_Color;

public:

    //
    // Self Initialization
    //

    virtual bool InitializeContext( Gwen::WindowProvider* pWindow );
    virtual bool ShutdownContext( Gwen::WindowProvider* pWindow );
    virtual bool PresentContext( Gwen::WindowProvider* pWindow );
    virtual bool ResizedContext( Gwen::WindowProvider* pWindow, int w, int h );
    virtual bool BeginContext( Gwen::WindowProvider* pWindow );
    virtual bool EndContext( Gwen::WindowProvider* pWindow );

private:
    /* Each vertex is:
     * two floats for the 2d coordinate
     * four u8s for the color
     * two f32s for the texcoords
     * the vbo contains data of the aforementioned elements interleaved.
     * Each sprite has four vertices.
     * */
    struct Vertex {
        float x, y;
        unsigned int color; // packed with 4 u8s (unsigned chars) for color
        float u, v;
    };

    typedef unsigned int u32;
    typedef float f32;

    void addQuad(const Gwen::Rect& rect, const Gwen::Color& color, float u1, float v1, float u2, float v2);
    void finalizeDraw();

    int m_currentQuadCount;

    int m_maxSpriteCount;

    void initGL();
    void checkGLError();

    glm::mat4 m_modelMatrix;
    glm::mat4 m_projectionMatrix;

    Gwen::Shader* m_shader;
    Gwen::Shader* m_fontShader;

    GLuint m_whiteTexture;
    GLuint m_currentBoundTexture;

    struct FontWrapper {
        texture_atlas_t* atlas;
        texture_font_t* font;
    };

    GLuint m_vao; // vertex array object
    GLuint m_vbo; // vertex buffer object
    GLuint m_ebo; // element buffer object

    int m_screenWidth;
    int m_screenHeight;
};

}
}
#endif
