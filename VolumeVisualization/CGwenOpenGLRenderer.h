
#pragma once

#include "Gwen/Gwen.h"
#include "Gwen/Renderers/OpenGL.h"

class CGwenOpenGLRenderer : public Gwen::Renderer::OpenGL
{
public:

	CGwenOpenGLRenderer();
	~CGwenOpenGLRenderer();

	void Init();

	void RenderText( Gwen::Font* pFont, Gwen::Point pos, const Gwen::UnicodeString & text );
	Gwen::Point MeasureText( Gwen::Font* pFont, const Gwen::UnicodeString & text );

protected:

	void CreateDebugFont();
	void DestroyDebugFont();

	Gwen::Texture*	m_pFontTexture;
	float			m_fFontScale[2];
	float			m_fLetterSpacing;

};
