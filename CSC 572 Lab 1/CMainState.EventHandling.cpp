#include "CMainState.h"

void CMainState::OnKeyboardEvent(SKeyboardEvent const & Event)
{
	std::stringstream s;

    switch (Event.Key)
    {

	case EKey::NUM_3:

		if (! Event.Pressed)
			Camera->setVelocity(200.f);

		break;

	case EKey::NUM_2:

		if (! Event.Pressed)
			Camera->setVelocity(40.f);

		break;

	case EKey::NUM_1:

		if (! Event.Pressed)
			Camera->setVelocity(2.5f);

		break;

	case EKey::k:

		Terrain->DebugHeight = Event.Pressed;
		break;

	case EKey::o:

		if (! Event.Pressed)
			Terrain->enableDebugData(EDebugData::Wireframe);

		break;

	case EKey::p:

		if (! Event.Pressed)
			Terrain->disableDebugData(EDebugData::Wireframe);

		break;

	case EKey::x:

		if (! Event.Pressed)
		{
			GridObject->setVisible(! GridObject->isVisible());
			s << "Point cloud object ";
			if (GridObject->isVisible())
				s << "enabled.";
			else
				s << "disabled.";
			addConsoleMessage(s.str());
		}
		break;

	case EKey::z:

		if (! Event.Pressed)
		{
			PointCloudObject->setVisible(! PointCloudObject->isVisible());
			s << "Oct tree object ";
			if (PointCloudObject->isVisible())
				s << "enabled.";
			else
				s << "disabled.";
			addConsoleMessage(s.str());
		}
		break;

	case EKey::c:

		if (! Event.Pressed)
		{
			SkyBox->setVisible(! SkyBox->isVisible());
			s << "Skybox object ";
			if (SkyBox->isVisible())
				s << "enabled.";
			else
				s << "disabled.";
			addConsoleMessage(s.str());
		}

		break;

	case EKey::g:

		if (! Event.Pressed)
		{
			GUIManager->ShowGUI = ! GUIManager->ShowGUI;
			s << "GUI ";
			if (GUIManager->ShowGUI)
				s << "enabled.";
			else
				s << "disabled.";
			addConsoleMessage(s.str());
		}

		break;

	case EKey::v:

		if (! Event.Pressed)
		{
			s << "Volume mode: ";
			if (VolumeSceneObject->ShowVolume)
			{
				VolumeSceneObject->ShowVolume = 0;
				s << "disabled.";
				addConsoleMessage(s.str());
			}
			else
			{
				VolumeSceneObject->ShowVolume = 2;
				s << "enabled.";
				addConsoleMessage(s.str());
			}
		}

		break;

	case EKey::b:

		if (! Event.Pressed)
		{
			if (VolumeSceneObject->ShowVolume != 1)
				addConsoleMessage("Volume mode: legacy.");
			VolumeSceneObject->ShowVolume = 1;
		}

		break;

	case EKey::m:
		if (! Event.Pressed)
		{
			SceneManager->setActiveCamera(OrbitCamera);
		}
		break;
		
	case EKey::n:
		if (! Event.Pressed)
		{
			SceneManager->setActiveCamera(Camera);
		}
		break;

    case EKey::r:
		if (! Event.Pressed)
		{
			Mode = 0;
			addConsoleMessage("Axis rotation mode enabled.");
		}
        break;

    case EKey::t:
        if (! Event.Pressed)
		{
			Mode = 1;
			addConsoleMessage("Translation mode enabled.");
		}
        break;

    case EKey::e:
        if (! Event.Pressed)
		{
			Mode = 2;
			addConsoleMessage("Scale mode enabled.");
		}
        break;

	case EKey::y:
		if (! Event.Pressed)
		{
			Mode = 3;
			addConsoleMessage("Light translation mode enabled.");
		}
		break;

    }
}
