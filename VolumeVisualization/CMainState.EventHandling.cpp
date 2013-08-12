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

	case EKey::Num6:

		if (! Event.Pressed)
			Context->Scene.Camera->setVelocity(500.f);
		break;

	case EKey::Num5:

		if (! Event.Pressed)
			Context->Scene.Camera->setVelocity(50.f);
		break;

	case EKey::Num4:

		if (! Event.Pressed)
			Context->Scene.Camera->setVelocity(10.f);
		break;

	case EKey::Num3:

		if (! Event.Pressed)
			Context->Scene.Camera->setVelocity(2.5f);
		break;

	case EKey::Num2:

		if (! Event.Pressed)
			Context->Scene.Camera->setVelocity(0.75f);
		break;

	case EKey::Num1:

		if (! Event.Pressed)
			Context->Scene.Camera->setVelocity(0.1f);
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
    }
}

void CMainState::OnWindowResized(SWindowResizedEvent const & Event)
{
	Context->GUIContext->getCanvas()->SetSize(Event.Size.X, Event.Size.Y);
	Context->GUIContext->getCanvas()->Invalidate();
	Context->GUIContext->getCanvas()->InvalidateChildren(true);

	Context->Scene.Camera->SetAspectRatio(CApplication::get().getAspectRatio());
}
