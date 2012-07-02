#include "CMainState.h"

glm::vec3 CMainState::makeSphereVec(int x, int y)
{
    float ix = 2*(x / (float) CApplication::get().getWindowSize().X) - 1.f;
    float iy = 1.f - 2*(y / (float) CApplication::get().getWindowSize().X);

    float length = sqrt(ix*ix + iy*iy);

    if (length > 1.f)
    {
        return glm::vec3(ix / length, iy / length, 0);
    }
    else
    {
        return glm::vec3(ix, iy, sqrt(1 - ix*ix - iy*iy));
    }
}

void CMainState::OnMouseEvent(SMouseEvent const & Event)
{
    switch (Event.Type.Value)
    {

    case SMouseEvent::EType::Click:

        LastVec = makeSphereVec(Event.Location.X, Event.Location.Y);

        break;

    case SMouseEvent::EType::Move:
        {
            float difX = (float) Event.Movement.X;
            float difY = (float) Event.Movement.Y;
            glm::vec3 NewVec = makeSphereVec(Event.Location.X, Event.Location.Y);
            glm::mat4 ViewMatrix;
			if (& CApplication::get().getSceneManager())
				ViewMatrix = CApplication::get().getSceneManager().getActiveCamera()->getViewMatrix();

            if (CApplication::get().getEventManager().IsMouseDown[Event.Button.Left])
            {
	            if (Mode == 0)
	            {
		            glm::vec3 cross = glm::cross(LastVec, NewVec);
		            glm::vec4 axis(cross.x, cross.y, cross.z, 0);
		            float dot = 180.f / 3.1415f * acos(glm::dot(LastVec, NewVec));

		            if (cross.x == cross.x && cross.y == cross.y && cross.z == cross.z && dot == dot && (cross.x != 0 || cross.y != 0 || cross.z != 0))
		            {
			
                        glm::mat4 inverse = glm::inverse(ViewMatrix);
			            axis = inverse * axis;

			            glm::vec3 rotAxis(axis.x, axis.y, axis.z);
						glm::mat4 Transformation = glm::rotate(glm::mat4(1.f), dot, rotAxis);
			            Rotation = Transformation*Rotation;
						if (! equals(difX, 0.f) && ! equals(difY, 0.f) && Application.getEventManager().IsKeyDown[EKey::q])
						{
							glm::vec4 v = Transformation * glm::vec4(CVolumeControlsHandler::Instance->AxisVector.getGLMVector(), 0.f);//(Rotation * glm::vec4(1.f, 0.f, 0.f, 0.f));//
							CVolumeControlsHandler::Instance->AxisVector = SVector3f(v.x, v.y, v.z);
							CVolumeControlsHandler::Instance->resetVolumeData();
						
							printf("Setting new axis vector! %f %f %f \n", v.x, v.y, v.z);
						}
		            }
	            }
	            else if (Mode == 1)
	            {
		            float const moveSpeed = 0.01f;
		            glm::vec4 trans(difX*moveSpeed, -difY*moveSpeed, 0, 0);
		            trans = glm::inverse(ViewMatrix) * trans;
		            Translation.X += trans.x;
		            Translation.Y += trans.y;
		            Translation.Z += trans.z;
	            }
	            else if (Mode == 2)
	            {
		            float const scaleSpeed = 0.01f;
                    Scale = SVector3f(std::max(std::min(Scale.X + difX*scaleSpeed, 2.f), 0.1f));
	            }
				else if (Mode == 3)
	            {
		            float const moveSpeed = 0.01f;
		            glm::vec4 trans(difX*moveSpeed, -difY*moveSpeed, 0, 0);
		            trans = glm::inverse(ViewMatrix) * trans;
					LightPosition.X += trans.x;
		            LightPosition.Y += trans.y;
		            LightPosition.Z += trans.z;
	            }
            }

            LastVec = NewVec;

            break;
        }
    }
}
