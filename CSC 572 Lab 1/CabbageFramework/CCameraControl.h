#pragma once

#include "SVector3.h"
#include "SVector2.h"

#include "../CabbageScene/CabbageScene.h"
#include "CEventManager.h"
#include "CApplication.h"

class CCameraControl : public CPerspectiveCamera, public sigslot::has_slots<>
{

    CApplication & Application;
    CEventManager & EventManager;

    
    float MoveSpeed;

    int MouseLastX, MouseLastY;

    bool Tracking;


public:

    float Phi, Theta;

	CCameraControl(SVector3 const position = SVector3(0, 3, 2), float const theta = -1.5708f, float const phi = 0.f)
        : Application(CApplication::get()), EventManager(CApplication::get().getEventManager()), Phi(phi), Theta(theta), Tracking(false), MoveSpeed(2.5f)
	{
        Application.getEventManager().OnMouseEvent.connect(this, & CCameraControl::OnMouseEvent);
		Position = position;
	}

	void OnMouseEvent(SMouseEvent const & Event)
	{
        if ((Event.Pressed) && Event.Button.Value == Event.Button.Right)
			Tracking = true;

		if ((! Event.Pressed) && Event.Button.Value == Event.Button.Right)
			Tracking = false;

        if ((Event.Type.Value == Event.Type.Move))
		{
			if (Tracking)
			{
                Theta += (Event.Movement.X)*0.005f;
				Phi -= (Event.Movement.Y)*0.005f;

                if (Phi > 3.1415f/2.02f)
                    Phi = 3.1415f/2.02f;
                if (Phi < -3.1415f/2.02f)
                    Phi = -3.1415f/2.02f;
        	}

            MouseLastX = Event.Location.X;
			MouseLastY = Event.Location.Y;
		}
	}

	void update(float const TickTime)
	{
		LookDirection = SVector3(cos(Theta)*cos(Phi), sin(Phi), sin(Theta)*cos(Phi));

		if (EventManager.IsKeyDown[SDLK_w])
		{
			Position += LookDirection*MoveSpeed*TickTime;
		}

		if (EventManager.IsKeyDown[SDLK_a])
		{
			Position.X += cos(Theta - 1.57079633f)*MoveSpeed*TickTime;
			Position.Z += sin(Theta - 1.57079633f)*MoveSpeed*TickTime;
		}

		if (EventManager.IsKeyDown[SDLK_d])
		{
			Position.X += cos(Theta + 1.57079633f)*MoveSpeed*TickTime;
			Position.Z += sin(Theta + 1.57079633f)*MoveSpeed*TickTime;
		}

		if (EventManager.IsKeyDown[SDLK_s])
		{
			Position -= LookDirection*MoveSpeed*TickTime;
		}
	}

	SVector3 const & getPosition()
	{
		return Position;
	}

};
