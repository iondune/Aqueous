#include "CMainState.h"

void CMainState::OnKeyboardEvent(SKeyboardEvent const & Event)
{
    switch (Event.Key)
    {

	case EKey::x:

		if (! Event.Pressed)
			SoupObject->setVisible(! SoupObject->isVisible());

		break;

	case EKey::z:

		if (! Event.Pressed)
			VoxelObject->setVisible(! VoxelObject->isVisible());

		break;

	case EKey::c:

		if (! Event.Pressed)
			SkyBox->setVisible(! SkyBox->isVisible());

		break;

	case EKey::g:

		if (! Event.Pressed)
			ShowGUI = ! ShowGUI;

		break;

	case EKey::v:

		if (! Event.Pressed)
		{
			ShowVolume ++;
			if (ShowVolume > 2)
				ShowVolume = 0;
		}

		break;

	case EKey::b:

		if (! Event.Pressed)
		{
			ShowVolume --;
			if (ShowVolume < 0)
				ShowVolume = 2;
		}

		break;

    case EKey::n:

		if (! Event.Pressed)
		{
			if (Tyra->isDebugDataEnabled(EDebugData::Normals))
				Tyra->disableDebugData(EDebugData::Normals);
			else
				Tyra->enableDebugData(EDebugData::Normals);
		}

        break;

    case EKey::m:

        if (! Event.Pressed)
		{
			if (Tyra->isDebugDataEnabled(EDebugData::NormalColors))
				Tyra->disableDebugData(EDebugData::NormalColors);
			else
				Tyra->enableDebugData(EDebugData::NormalColors);
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
