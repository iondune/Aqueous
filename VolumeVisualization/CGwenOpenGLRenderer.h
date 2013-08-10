
#pragma once

#include "OpenGL3\OpenGL3.h"
#include <ionMath.h>

class CGwenOpenGLRendererFont : public Gwen::Renderer::OpenGL3
{
public:

	CGwenOpenGLRendererFont(vec2i const & Size);

	void RenderText(Gwen::Font* pFont, Gwen::Point pos, const Gwen::UnicodeString & text);
	Gwen::Point MeasureText(Gwen::Font* pFont, const Gwen::UnicodeString & text);

protected:

	void LoadFont(Gwen::Font * pFont);
};
