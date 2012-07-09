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

	case EKey::x:

		if (! Event.Pressed)
		{
			SoupObject->setVisible(! SoupObject->isVisible());
			s << "Point cloud object ";
			if (SoupObject->isVisible())
				s << "enabled.";
			else
				s << "disabled.";
			addConsoleMessage(s.str());
		}
		break;

	case EKey::z:

		if (! Event.Pressed)
		{
			VoxelObject->setVisible(! VoxelObject->isVisible());
			s << "Oct tree object ";
			if (VoxelObject->isVisible())
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
			ShowGUI = ! ShowGUI;
			s << "GUI ";
			if (ShowGUI)
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
			if (ShowVolume)
			{
				ShowVolume = 0;
				s << "disabled.";
				addConsoleMessage(s.str());
			}
			else
			{
				ShowVolume = 2;
				s << "enabled.";
				addConsoleMessage(s.str());
			}
		}

		break;

	case EKey::b:

		if (! Event.Pressed)
		{
			if (ShowVolume != 1)
				addConsoleMessage("Volume mode: legacy.");
			ShowVolume = 1;
		}

		break;

	case EKey::m:
		if (! Event.Pressed)
		{
			SceneManager.setActiveCamera(OrbitCamera);
		}
		break;
		
	case EKey::n:
		if (! Event.Pressed)
		{
			SceneManager.setActiveCamera(Camera);
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
