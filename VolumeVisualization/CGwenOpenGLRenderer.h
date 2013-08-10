
#pragma once

#include "Gwen/Gwen.h"

class CGwenOpenGLRenderer : public Gwen::Renderer::Base
{
public:

	struct Vertex
	{
		float x, y, z;
		float u, v;
		unsigned char r, g, b, a;
	};

	CGwenOpenGLRenderer();
	~CGwenOpenGLRenderer();

	virtual void Init();

	virtual void Begin();
	virtual void End();

	virtual void SetDrawColor( Gwen::Color color );
	virtual void DrawFilledRect( Gwen::Rect rect );

	void StartClip();
	void EndClip();

	void DrawTexturedRect( Gwen::Texture* pTexture, Gwen::Rect pTargetRect, float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f );
	void LoadTexture( Gwen::Texture* pTexture );
	void FreeTexture( Gwen::Texture* pTexture );
	Gwen::Color PixelColour( Gwen::Texture* pTexture, unsigned int x, unsigned int y, const Gwen::Color & col_default );

protected:

	static const int MaxVerts = 1024;

	void Flush();
	void AddVert(int x, int y, float u = 0.0f , float v = 0.0f);

	Gwen::Color m_Color;
	int m_iVertNum;
	Vertex m_Vertices[ MaxVerts ];

public:

	virtual bool InitializeContext( Gwen::WindowProvider* pWindow );
	virtual bool ShutdownContext( Gwen::WindowProvider* pWindow );
	virtual bool PresentContext( Gwen::WindowProvider* pWindow );
	virtual bool ResizedContext( Gwen::WindowProvider* pWindow, int w, int h );
	virtual bool BeginContext( Gwen::WindowProvider* pWindow );
	virtual bool EndContext( Gwen::WindowProvider* pWindow );

	void*	m_pContext;
};

class CGwenOpenGLRendererNull : public Gwen::Renderer::Base
{
public:

	CGwenOpenGLRendererNull();
	~CGwenOpenGLRendererNull();

	virtual void Init();

	virtual void Begin();
	virtual void End();

	virtual void SetDrawColor( Gwen::Color color );
	virtual void DrawFilledRect( Gwen::Rect rect );

	void StartClip();
	void EndClip();

	void DrawTexturedRect( Gwen::Texture* pTexture, Gwen::Rect pTargetRect, float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f );
	void LoadTexture( Gwen::Texture* pTexture );
	void FreeTexture( Gwen::Texture* pTexture );
	Gwen::Color PixelColour( Gwen::Texture* pTexture, unsigned int x, unsigned int y, const Gwen::Color & col_default );

public:

	virtual bool InitializeContext( Gwen::WindowProvider* pWindow );
	virtual bool ShutdownContext( Gwen::WindowProvider* pWindow );
	virtual bool PresentContext( Gwen::WindowProvider* pWindow );
	virtual bool ResizedContext( Gwen::WindowProvider* pWindow, int w, int h );
	virtual bool BeginContext( Gwen::WindowProvider* pWindow );
	virtual bool EndContext( Gwen::WindowProvider* pWindow );
};

class CGwenOpenGLRendererFont : public CGwenOpenGLRenderer
{
public:

	CGwenOpenGLRendererFont();
	~CGwenOpenGLRendererFont();

	void Init();

	void RenderText(Gwen::Font* pFont, Gwen::Point pos, const Gwen::UnicodeString & text);
	Gwen::Point MeasureText(Gwen::Font* pFont, const Gwen::UnicodeString & text);

protected:

	void LoadFont(Gwen::Font * pFont);
};
