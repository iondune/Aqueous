#ifndef _CMAINSTATE_H_INCLUDED_
#define _CMAINSTATE_H_INCLUDED_

#include "CProgramContext.h"

#include <Rocket\Core.h>

class RocketSFMLRenderer;
class RocketSFMLSystemInterface;
class ShellFileInterface;


class CMainState : public CContextState<CMainState, CProgramContext>
{

	bool ShowDepth;

	Rocket::Core::Context* RocketContext;
	//RocketSFMLRenderer * Renderer;
	//RocketSFMLSystemInterface * SystemInterface;
	ShellFileInterface * FileInterface;

public:

	CMainState();

	void begin();
	void end();

	void OnRenderStart(float const Elapsed);

    void OnKeyboardEvent(SKeyboardEvent const & Event);
	void OnWindowResized(SWindowResizedEvent const & Event);

	//void OnSFMLEvent(sf::Event const & Event);

	//void addConsoleMessage(std::string const & Message, Gwen::Color const & Color = Gwen::Color(255, 255, 255, 255));

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
    void OnMouseEvent(SMouseEvent const & Event);
    void OnUncaughtMouseEvent(SMouseEvent const & Event);

};

#endif
