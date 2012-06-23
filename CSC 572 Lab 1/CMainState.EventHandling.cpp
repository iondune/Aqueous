#include "CMainState.h"

void CMainState::OnKeyboardEvent(SKeyboardEvent const & Event)
{
    switch (Event.Key)
    {

	case SDLK_x:

		if (! Event.Pressed)
		{
			SoupObject->setVisible(! SoupObject->isVisible());
		}

		break;

	case SDLK_z:

		if (! Event.Pressed)
		{
			VoxelObject->setVisible(! VoxelObject->isVisible());
		}

		break;

    case SDLK_n:

		if (! Event.Pressed)
		{
			if (Tyra->isDebugDataEnabled(EDebugData::Normals))
				Tyra->disableDebugData(EDebugData::Normals);
			else
				Tyra->enableDebugData(EDebugData::Normals);
		}

        break;

    case SDLK_m:

        if (! Event.Pressed)
		{
			if (Tyra->isDebugDataEnabled(EDebugData::NormalColors))
				Tyra->disableDebugData(EDebugData::NormalColors);
			else
				Tyra->enableDebugData(EDebugData::NormalColors);
		}

        break;

    case SDLK_r:
        Mode = 0;
        break;

    case SDLK_t:
        Mode = 1;
        break;

    case SDLK_e:
        Mode = 2;
        break;

	case SDLK_y:
		Mode = 3;
		break;

    }
}
