
#include "CGwenOpenGLRenderer.h"


CGwenOpenGLRendererNull::CGwenOpenGLRendererNull()
{
}

CGwenOpenGLRendererNull::~CGwenOpenGLRendererNull()
{
}

void CGwenOpenGLRendererNull::Init()
{
}

void CGwenOpenGLRendererNull::Begin()
{
}

void CGwenOpenGLRendererNull::End()
{
}

void CGwenOpenGLRendererNull::DrawFilledRect( Gwen::Rect rect )
{
}

void CGwenOpenGLRendererNull::SetDrawColor( Gwen::Color color )
{
}

void CGwenOpenGLRendererNull::StartClip()
{
}

void CGwenOpenGLRendererNull::EndClip()
{
}

void CGwenOpenGLRendererNull::DrawTexturedRect( Gwen::Texture* pTexture, Gwen::Rect rect, float u1, float v1, float u2, float v2 )
{
}

void CGwenOpenGLRendererNull::LoadTexture( Gwen::Texture* pTexture )
{
}

void CGwenOpenGLRendererNull::FreeTexture( Gwen::Texture* pTexture )
{
}

Gwen::Color CGwenOpenGLRendererNull::PixelColour( Gwen::Texture* pTexture, unsigned int x, unsigned int y, const Gwen::Color & col_default )
{
	return Gwen::Color();
}

bool CGwenOpenGLRendererNull::InitializeContext( Gwen::WindowProvider* pWindow )
{
	return true;
}

bool CGwenOpenGLRendererNull::ShutdownContext( Gwen::WindowProvider* pWindow )
{
	return true;
}

bool CGwenOpenGLRendererNull::PresentContext( Gwen::WindowProvider* pWindow )
{
	return true;
}

bool CGwenOpenGLRendererNull::ResizedContext( Gwen::WindowProvider* pWindow, int w, int h )
{
	return true;
}

bool CGwenOpenGLRendererNull::BeginContext( Gwen::WindowProvider* pWindow )
{
	return true;
}

bool CGwenOpenGLRendererNull::EndContext( Gwen::WindowProvider* pWindow )
{
	return true;
}
