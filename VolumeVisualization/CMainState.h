
#pragma once

#include "CProgramContext.h"

#include <CGUIEventManager.h>


class CMainState : public CContextState<CMainState, CProgramContext>
{

public:

	CMainState();

	void Begin();
	void End();

	void Update(f32 const Elapsed);

    void OnEvent(SKeyboardEvent & Event);
	void OnEvent(SWindowResizedEvent & Event);

	void addConsoleMessage(std::string const & Message, Gwen::Color const & Color = Gwen::Color(255, 255, 255, 255));


	bool ShowDepth;

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
