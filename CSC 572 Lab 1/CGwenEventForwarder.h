#include <ionWindow.h>
#include <Gwen/Events.h>
#include <Gwen/Controls.h>

class CGwenEventForwarder : public CApplicationEventReceiver
{

	Gwen::Controls::Canvas * Canvas;

public:

	CGwenEventForwarder(Gwen::Controls::Canvas * pCanvas)
		: Canvas(pCanvas)
	{}

	virtual void OnKeyboardEvent(SKeyboardEvent const & Event)
	{

		if (Event.Key >= EKey::a && Event.Key <= EKey::z)
		{
			char key = 'a' + (Event.Key - EKey::a);
			if (Application->getEventManager().IsKeyDown[EKey::LSHIFT] || 
				Application->getEventManager().IsKeyDown[EKey::RSHIFT])
				key += 'A' - 'a';
			Canvas->InputCharacter(key);
		}
		else
		{
			int gwenkey = 0;
			switch (Event.Key)
			{
			case EKey::RETURN: gwenkey = Gwen::Key::Return; break;
			case EKey::BACKSPACE: gwenkey = Gwen::Key::Backspace; break;
			case EKey::DELETE_KEY: gwenkey = Gwen::Key::Delete; break;
			case EKey::LEFT: gwenkey = Gwen::Key::Left; break;
			case EKey::RIGHT: gwenkey = Gwen::Key::Right; break;
			case EKey::LSHIFT: gwenkey = Gwen::Key::Shift; break;
			case EKey::RSHIFT: gwenkey = Gwen::Key::Shift; break;
			case EKey::TAB: gwenkey = Gwen::Key::Tab; break;
			case EKey::SPACE: gwenkey = Gwen::Key::Space; break;
			case EKey::HOME: gwenkey = Gwen::Key::Home; break;
			case EKey::END: gwenkey = Gwen::Key::End; break;
			case EKey::LCTRL: gwenkey = Gwen::Key::Control; break;
			case EKey::RCTRL: gwenkey = Gwen::Key::Control; break;
			case EKey::UP: gwenkey = Gwen::Key::Up; break;
			case EKey::DOWN: gwenkey = Gwen::Key::Down; break;
			case EKey::ESCAPE: gwenkey = Gwen::Key::Escape; break;
			case EKey::LALT: gwenkey = Gwen::Key::Alt; break;
			case EKey::RALT: gwenkey = Gwen::Key::Alt; break;
			}
			Canvas->InputKey(gwenkey, Event.Pressed);
		}
	}

	virtual void OnMouseEvent(SMouseEvent const & Event)
	{
		switch (Event.Type.Value)
		{
		case SMouseEvent::EType::Move:

			Canvas->InputMouseMoved(Event.Location.X, Event.Location.Y, Event.Movement.X, Event.Movement.Y);

			break;
			
		case SMouseEvent::EType::Click:
			{
				int Button = -1;
				switch (Event.Button.Value)
				{
				case SMouseEvent::EButton::Left:
					Button = 0;
					break;
				
				case SMouseEvent::EButton::Right:
					Button = 1;
					break;

				case SMouseEvent::EButton::Middle:

					break;
				}

				Canvas->InputMouseButton(Button, Event.Pressed);
			}
			break;
		}
	}

	virtual void OnGameTickStart(float const Elapsed)
	{}
	virtual void OnGameTickEnd(float const Elapsed)
	{}

	virtual void OnRenderStart(float const Elapsed)
	{}
	virtual void OnRenderEnd(float const Elapsed)
	{}

	virtual void OnApplicationExit()
	{}

};