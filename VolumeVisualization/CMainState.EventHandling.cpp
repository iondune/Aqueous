#include "CMainState.h"


void CMainState::OnKeyboardEvent(SKeyboardEvent const & Event)
{
	std::stringstream s;

    switch (Event.Key)
    {

		//////////////////
		// Camera Speed //
		//////////////////

	case EKey::NUM_3:

		if (! Event.Pressed)
			Context->Scene.Camera->setVelocity(50.f);

		break;

	case EKey::NUM_2:

		if (! Event.Pressed)
			Context->Scene.Camera->setVelocity(10.f);

		break;

	case EKey::NUM_1:

		if (! Event.Pressed)
			Context->Scene.Camera->setVelocity(2.5f);

		break;


		////////////////////
		// Widget Control //
		////////////////////

	case EKey::t:
		
		Context->GUIContext->getTerrainControl()->toggle();
		break;

	case EKey::v:

		Context->GUIContext->getVolumeControl()->toggle();
		break;

	case EKey::g:

		Context->GUIContext->getGlyphControl()->toggle();
		break;
		

		///////////////////////
		// Terrain Wireframe //
		///////////////////////

	case EKey::o:

		if (! Event.Pressed)
			Context->Scene.Terrain->enableDebugData(EDebugData::Wireframe);

		break;

	case EKey::p:

		if (! Event.Pressed)
			Context->Scene.Terrain->disableDebugData(EDebugData::Wireframe);

		break;

		
		////////////
		// Skybox //
		////////////

	case EKey::c:

		if (! Event.Pressed)
		{
			Context->Scene.SkyBox->setVisible(! Context->Scene.SkyBox->isVisible());
			s << "Skybox object ";
			if (Context->Scene.SkyBox->isVisible())
				s << "enabled.";
			else
				s << "disabled.";
			addConsoleMessage(s.str());
		}

		break;

	case EKey::b:

		if (! Event.Pressed)
		{
			if (Context->Scene.VolumeSceneObject->ShowVolume != 1)
				addConsoleMessage("Volume mode: legacy.");
			Context->Scene.VolumeSceneObject->ShowVolume = 1;
		}

		break;

	case EKey::m:
		if (! Event.Pressed)
		{
			SceneManager->setActiveCamera(Context->Scene.OrbitCamera);
		}
		break;
		
	case EKey::n:
		if (! Event.Pressed)
		{
			SceneManager->setActiveCamera(Context->Scene.Camera);
		}
		break;

    case EKey::r:
		if (! Event.Pressed)
		{
			Mode = 0;
			addConsoleMessage("Axis rotation mode enabled.");
		}
        break;

    case EKey::k:
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
