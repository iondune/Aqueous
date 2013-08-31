#include "CGUIConsoleWidget.h"

#include "CProgramContext.h"


CGUIConsoleWidget::CGUIConsoleWidget(int const size)
	: Size(0), Accumulator(0.f)
{
	setSize(size);
}

void CGUIConsoleWidget::setSize(int const size)
{
	Size = size;

	Messages.resize(Size, 0);
	MessageColors.resize(Size, Gwen::Color(0, 0, 0, 0));

	for (int i = 0; i < Size; ++ i)
	{
		if (Messages[i])
			continue;

		Messages[i] = new Gwen::Controls::Label(GUIManager->GetCanvas());
		Messages[i]->SetPos(20, CApplication::Get().GetWindow().GetSize().Y - 50 - 25 * i);
		Messages[i]->SetSize(1500, 30);
	}
}

void CGUIConsoleWidget::update(float const Elapsed)
{
	static float const AlphaTick = 0.1f;
	Accumulator += Elapsed;

	while (Accumulator > AlphaTick)
	{
		for (int i = 0; i < Size; ++ i)
		{
			static int const Decrement = 3;
			Gwen::Color & c = MessageColors[i];

			if (c.a >= Decrement)
				c.a -= Decrement;
			else
				c.a = 0;
				
			Messages[i]->SetTextColor(c);
		}

		Accumulator -= AlphaTick;
	}
}

void CGUIConsoleWidget::addMessage(std::string const & Message, Gwen::Color const & Color)
{
	for (int i = Size - 1; i > 0; -- i)
	{
		Messages[i]->SetText(Messages[i-1]->GetText());
		MessageColors[i] = MessageColors[i-1];
		Messages[i]->SetTextColor(MessageColors[i]);
	}

	Messages[0]->SetText(Message);
	MessageColors[0] = Color;
	Messages[0]->SetTextColor(Color);
}
