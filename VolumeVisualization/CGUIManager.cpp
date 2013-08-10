#include "CGUIManager.h"

#include <Gwen/Renderers/OpenGL.h>
#include <Gwen/Skins/TexturedBase.h>
#include <Gwen/Skins/Simple.h>
#include <Gwen/Controls.h>

#include "CGwenOpenGLRenderer.h"
#include "OpenGL3\OpenGL3.h"


Gwen::Font * LoadFont(Gwen::UnicodeString const & File, float const Size)
{
	Gwen::Font * Font = new Gwen::Font();
	Font->facename = File;
	Font->size = Size;

	return Font;
}


CGUIManager::CGUIManager()
	: Application(CApplication::get())
{}

void CGUIManager::init()
{
	Gwen::Renderer::Base * pRenderer = new Gwen::Renderer::OpenGL3(CApplication::get().getWindowSize().X, CApplication::get().getWindowSize().Y);

	Gwen::Skin::TexturedBase * skin = new Gwen::Skin::TexturedBase(pRenderer);
	skin->Init("DefaultSkin.png");
	skin->SetDefaultFont(L"OpenSans.ttf", 12.f);

	LargeFont = LoadFont(L"OpenSans.ttf", 40.f);
	MediumFont = LoadFont(L"OpenSans.ttf", 24.f);
	RegularFont = LoadFont(L"OpenSans.ttf", 12.f);

	Canvas = new Gwen::Controls::Canvas(skin);
	Canvas->SetSize(Application.getWindowSize().X, Application.getWindowSize().Y);
}

void CGUIManager::draw(f32 const Elapsed, bool const ClearAll)
{
	for (auto it = Widgets.begin(); it != Widgets.end(); ++ it)
		(* it)->update(Elapsed);

	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if (ClearAll)
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		else
			glClear(GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			int left = 0, top = 0;
			int right = Application.getWindowSize().X, bottom = Application.getWindowSize().Y;
			glOrtho(left, right, bottom, top, -1.0, 1.0);

		glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glViewport(0, 0, right - left, bottom - top);
			
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	Canvas->RenderCanvas();
}

Gwen::Controls::Canvas * CGUIManager::getCanvas()
{
	return Canvas;
}
	
Gwen::Font * CGUIManager::getLargeFont()
{
	return LargeFont;
}

Gwen::Font * CGUIManager::getMediumFont()
{
	return MediumFont;
}

Gwen::Font * CGUIManager::getRegularFont()
{
	return RegularFont;
}

void CGUIManager::addWidget(CGUIWidget * Widget)
{
	Widgets.push_back(Widget);
}

void CGUIManager::removeWidget(CGUIWidget * Widget)
{
	for (auto it = Widgets.begin(); it != Widgets.end(); ++ it)
		delete (* it);
	Widgets.erase(std::remove(Widgets.begin(), Widgets.end(), Widget), Widgets.end());
}

void CGUIManager::removeAllWidgets()
{
	Widgets.clear();
}
