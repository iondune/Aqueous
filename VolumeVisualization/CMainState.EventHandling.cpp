#include "CMainState.h"


void CMainState::OnKeyboardEvent(SKeyboardEvent const & Event)
{
	std::stringstream s;

    switch (Event.Key)
    {

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

	case EKey::k:

		Context->Scene.Terrain->DebugHeight = Event.Pressed;
		break;

	case EKey::v:

		Context->GUIContext->getVolumeControl()->enable();
		break;

	case EKey::o:

		if (! Event.Pressed)
			Context->Scene.Terrain->enableDebugData(EDebugData::Wireframe);

		break;

	case EKey::p:

		if (! Event.Pressed)
			Context->Scene.Terrain->disableDebugData(EDebugData::Wireframe);

		break;

	case EKey::x:

		if (! Event.Pressed)
		{
			Context->Scene.GridObject->setVisible(! Context->Scene.GridObject->isVisible());
			s << "Point cloud object ";
			if (Context->Scene.GridObject->isVisible())
				s << "enabled.";
			else
				s << "disabled.";
			addConsoleMessage(s.str());
		}
		break;

	case EKey::z:

		if (! Event.Pressed)
		{
			Context->Scene.PointCloudObject->setVisible(! Context->Scene.PointCloudObject->isVisible());
			s << "Oct tree object ";
			if (Context->Scene.PointCloudObject->isVisible())
				s << "enabled.";
			else
				s << "disabled.";
			addConsoleMessage(s.str());
		}
		break;

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
