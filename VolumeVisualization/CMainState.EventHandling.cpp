#include "CMainState.h"

#include "SciDataManager.h"


void CMainState::OnKeyboardEvent(SKeyboardEvent const & Event)
{
	std::stringstream s;

    switch (Event.Key)
    {

	case EKey::Escape:

		if (! Event.Pressed)
			Application->close();
		break;

		//////////////////
		// Camera Speed //
		//////////////////

	case EKey::Num3:

		if (! Event.Pressed)
			Context->Scene.Camera->setVelocity(50.f);
		break;

	case EKey::Num2:

		if (! Event.Pressed)
			Context->Scene.Camera->setVelocity(10.f);
		break;

	case EKey::Num1:

		if (! Event.Pressed)
			Context->Scene.Camera->setVelocity(2.5f);
		break;


		////////////////////
		// Widget Control //
		////////////////////

	case EKey::T:
		
		if (! Event.Pressed)
			Context->GUIContext->getTerrainControl()->toggle();
		break;

	case EKey::V:

		if (! Event.Pressed)
			Context->GUIContext->getVolumeControl()->toggle();
		break;

	case EKey::G:

		if (! Event.Pressed)
			Context->GUIContext->getGlyphControl()->toggle();
		break;

		
		////////////
		// Skybox //
		////////////

	case EKey::C:

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

	case EKey::Y:

		if (! Event.Pressed)
		{
			Context->DataManager->produceVolumeMaps();
		}

		break;

	case EKey::B:

		if (! Event.Pressed)
		{
			SceneManager->getEffectManager()->setEffectEnabled(ESE_BLOOM, ! SceneManager->getEffectManager()->isEffectEnabled(ESE_BLOOM));
		}

		break;

	case EKey::M:

		ShowDepth = Event.Pressed;

		break;

	case EKey::P:

		if (! Event.Pressed)
			Context->Scene.VolumeSceneObject->Control.Debug = ! Context->Scene.VolumeSceneObject->Control.Debug;
		break;

	/*case EKey::b:

		if (! Event.Pressed)
		{
			if (Context->Scene.VolumeSceneObject->ShowVolume != 1)
				addConsoleMessage("Volume mode: legacy.");
			Context->Scene.VolumeSceneObject->ShowVolume = 1;
		}

		break;*/

		// To Do : Camera control buttons
	case EKey::H:
		if (! Event.Pressed)
		{
			SceneManager->setActiveCamera(Context->Scene.OrbitCamera);
		}
		break;
		
	case EKey::N:
		if (! Event.Pressed)
		{
			SceneManager->setActiveCamera(Context->Scene.Camera);
		}
		break;

		// To Do : Toolbox for scene repositioning
    /*case EKey::r:
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
		break;*/

    }
}

void CMainState::OnWindowResized(SWindowResizedEvent const & Event)
{
	Context->GUIContext->getCanvas()->SetSize(Event.Size.X, Event.Size.Y);
	Context->GUIContext->getCanvas()->Invalidate();
	Context->GUIContext->getCanvas()->InvalidateChildren(true);

	Context->Scene.Camera->setProjection(60.f, CApplication::get().getAspectRatio(), 0.01f, 100.f);
}
