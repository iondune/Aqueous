
#include "CGwenOpenGLRenderer.h"
#include "Gwen/Utility.h"
#include "Gwen/Font.h"
#include "Gwen/Texture.h"

#include <math.h>
#include "GL/glew.h"

#include <FreeType.h>


CGwenOpenGLRenderer::CGwenOpenGLRenderer()
{}

void CGwenOpenGLRenderer::Init()
{}

CGwenOpenGLRenderer::~CGwenOpenGLRenderer()
{}


void CGwenOpenGLRenderer::LoadFont(Gwen::Font * pFont)
{
	if (pFont->data)
		return;

	freetype::font_data * font;

	pFont->data = font = new freetype::font_data;
	font->init(Gwen::Utility::UnicodeToString(pFont->facename).c_str(), pFont->size);
}

void CGwenOpenGLRenderer::RenderText(Gwen::Font * pFont, Gwen::Point pos, Gwen::UnicodeString const & text)
{
	LoadFont(pFont);
	Translate(pos.x, pos.y);
	Gwen::String String = Gwen::Utility::UnicodeToString(text);

	freetype::print(* (freetype::font_data *) pFont->data, pos.x, pos.y, String.c_str());
}

Gwen::Point CGwenOpenGLRenderer::MeasureText(Gwen::Font * pFont, Gwen::UnicodeString const & text)
{
	LoadFont(pFont);

	Gwen::String String = Gwen::Utility::UnicodeToString(text);

	Gwen::Point pos;
	freetype::measure(* (freetype::font_data *) pFont->data, & pos.x, & pos.y, String.c_str());
	return pos;
}
