
#pragma once
#include "CProgramContext.h"
#include "GifWriter.h"


class CMainState : public CContextState<CMainState, CProgramContext>
{

protected:

	GifWriter * gifWriter;

	IFont * Font;

public:

	CMainState();

	void Begin();
	void End();

	void Update(f32 const Elapsed);

	void OnEvent(IEvent & Event);
	void OnEvent(SKeyboardEvent & Event);
	void OnEvent(SWindowResizedEvent & Event);

	void AddConsoleMessage(std::string const & Message, color4i const & Color = Colors::White);

	void CalculateDataAlignment();

	bool ShowDepth;
	int ProjectionMode;

	void SetSite(int site);

	void BeginGifDraw();
	void EndGifDraw();
	f32 OrbitCameraTimer;

	bool GUIEnabled;
	bool ShowKey = false;

	//////////////////////////////
	// Mouse Tracking Varaibles //
	//////////////////////////////

	glm::vec3 LastVec;
	int Mode;
	SVector3f Translation, Scale;
	glm::mat4 Rotation;


	////////////////////////////
	// Mouse Tracking Methods //
	////////////////////////////

	glm::vec3 makeSphereVec(int x, int y);
	void OnEvent(SMouseEvent & Event);

};
