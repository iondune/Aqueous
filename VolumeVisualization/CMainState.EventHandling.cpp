#include "CMainState.h"

#include "SciDataManager.h"
#include "CTerrainSceneObject.h"


void CMainState::OnEvent(SKeyboardEvent & Event)
{
	std::stringstream s;

    switch (Event.Key)
    {

	case EKey::Escape:

		if (! Event.Pressed)
			Application->Close();
		break;


		//////////////////
		// Camera Speed //
		//////////////////

	case EKey::Num6:

		if (! Event.Pressed)
			Context->Scene.Camera->SetVelocity(500.f);
		break;

	case EKey::Num5:

		if (! Event.Pressed)
			Context->Scene.Camera->SetVelocity(50.f);
		break;

	case EKey::Num4:

		if (! Event.Pressed)
			Context->Scene.Camera->SetVelocity(10.f);
		break;

	case EKey::Num3:

		if (! Event.Pressed)
			Context->Scene.Camera->SetVelocity(2.5f);
		break;

	case EKey::Num2:

		if (! Event.Pressed)
			Context->Scene.Camera->SetVelocity(0.75f);
		break;

	case EKey::Num1:

		if (! Event.Pressed)
			Context->Scene.Camera->SetVelocity(0.1f);
		break;


		////////////////////
		// Widget Control //
		////////////////////

	case EKey::T:
		
		if (! Event.Pressed)
			Context->GUIContext->GetTerrainControl()->toggle();
		break;

	case EKey::V:

		if (! Event.Pressed)
			Context->GUIContext->GetVolumeControl()->toggle();
		break;

	case EKey::G:

		if (! Event.Pressed)
			Context->GUIContext->GetGlyphControl()->toggle();
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
			AddConsoleMessage(s.str());
		}

		break;

	case EKey::LeftBracket:

		if (! Event.Pressed)
			Context->Scene.Terrain->DebugMode ++;
		break;

	case EKey::RightBracket:

		if (! Event.Pressed)
			Context->Scene.Terrain->DebugMode --;
		break;

	//case EKey::Y:

		//if (! Event.Pressed)
		//	Context->DataManager->produceVolumeMaps();
		//break;

	case EKey::B:

		if (! Event.Pressed)
			SceneManager->getEffectManager()->setEffectEnabled(ESE_BLOOM, ! SceneManager->getEffectManager()->isEffectEnabled(ESE_BLOOM));
		break;


		/////////////////////
		// Camera Controls //
		/////////////////////

	case EKey::H:

		if (! Event.Pressed)
			SceneManager->setActiveCamera(Context->Scene.OrbitCamera);
		break;
		
	case EKey::N:

		if (! Event.Pressed)
			SceneManager->setActiveCamera(Context->Scene.Camera);
		break;
		
	case EKey::M:

		if (! Event.Pressed)
		{
			if (gifWriter)
				EndGifDraw();
			else
				BeginGifDraw();
		}
		break;

		// To Do : Camera control buttons


		//////////
		// Site //
		//////////

	case EKey::KeyPad1:
		
		if (! Event.Pressed)
			SetSite(0);
		break;

	case EKey::KeyPad2:
		
		if (! Event.Pressed)
			SetSite(1);
		break;

	case EKey::KeyPad3:
		
		if (! Event.Pressed)
			SetSite(2);
		break;

	case EKey::KeyPad4:
		
		if (! Event.Pressed)
			SetSite(3);
		break;

	case EKey::KeyPad5:
		
		if (! Event.Pressed)
			SetSite(4);
		break;

	case EKey::KeyPad6:
		
		if (! Event.Pressed)
			SetSite(5);
		break;
		
	case EKey::Comma:

		if (! Event.Pressed)
		{
			ProjectionMode = 0;
			CalculateDataAlignment();
		}
		break;
		
	case EKey::Period:

		if (! Event.Pressed)
		{
			ProjectionMode = 1;
			CalculateDataAlignment();
		}
		break;
		
	case EKey::Slash:

		if (! Event.Pressed)
		{
			ProjectionMode = 2;
			CalculateDataAlignment();
		}
		break;

		
	case EKey::BackSlash:

		if (! Event.Pressed)
			GUIEnabled = ! GUIEnabled;
		break;

    }
}

void CMainState::OnEvent(SWindowResizedEvent & Event)
{
	Context->GUIContext->GetCanvas()->SetSize(Event.Size.X, Event.Size.Y);
	Context->GUIContext->GetCanvas()->Invalidate();
	Context->GUIContext->GetCanvas()->InvalidateChildren(true);

	Context->Scene.Camera->SetAspectRatio(CApplication::Get().GetWindow().GetAspectRatio());
}
