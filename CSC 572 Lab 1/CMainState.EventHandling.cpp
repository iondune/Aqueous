#include "CMainState.h"

void CMainState::OnKeyboardEvent(SKeyboardEvent const & Event)
{
	std::stringstream s;

    switch (Event.Key)
    {

	case EKey::x:

		if (! Event.Pressed)
			SoupObject->setVisible(! SoupObject->isVisible());
		s << "Point cloud object ";
		if (SoupObject->isVisible())
			s << "enabled.";
		else
			s << "disabled.";
		addConsoleMessage(s.str());

		break;

	case EKey::z:

		if (! Event.Pressed)
			VoxelObject->setVisible(! VoxelObject->isVisible());
		s << "Oct tree object ";
		if (VoxelObject->isVisible())
			s << "enabled.";
		else
			s << "disabled.";
		addConsoleMessage(s.str());

		break;

	case EKey::c:

		if (! Event.Pressed)
			SkyBox->setVisible(! SkyBox->isVisible());
		s << "Skybox object ";
		if (SkyBox->isVisible())
			s << "enabled.";
		else
			s << "disabled.";
		addConsoleMessage(s.str());

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
			ShowVolume ++;
			if (ShowVolume > 2)
			{
				ShowVolume = 0;
				s << "off.";
				addConsoleMessage(s.str());
			}
			else
			{
				s << ShowVolume;
				addConsoleMessage(s.str());
			}
		}

		break;

	case EKey::b:

		if (! Event.Pressed)
		{
			s << "Volume mode: ";
			ShowVolume --;
			if (ShowVolume < 0)
				ShowVolume = 2;
			
			if (ShowVolume != 0)
			{
				s << ShowVolume;
				addConsoleMessage(s.str());
			}
			else
			{
				s << "off.";
				addConsoleMessage(s.str());
			}
		}

		break;

    case EKey::r:
        Mode = 0;
        break;

    case EKey::t:
        Mode = 1;
        break;

    case EKey::e:
        Mode = 2;
        break;

	case EKey::y:
		Mode = 3;
		break;

    }
}
