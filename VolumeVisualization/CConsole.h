#ifndef _CONSOLE_H_INCLUDED_
#define _CONSOLE_H_INCLUDED_


#include <Gwen/Gwen.h>
#include <Gwen/Controls.h>


class CConsole
{

	int Size;
	float Accumulator;
	std::vector<Gwen::Color> MessageColors;
	std::vector<Gwen::Controls::Label *> Messages;
	Gwen::Controls::Canvas * Canvas;

public:

	CConsole(Gwen::Controls::Canvas * canvas, int const size = 5)
		: Canvas(canvas), Size(0), Accumulator(0.f)
	{
		setSize(5);
	}

	void setSize(int const size)
	{
		Size = size;

		Messages.resize(Size, 0);
		MessageColors.resize(Size, Gwen::Color(0, 0, 0, 0));

		for (int i = 0; i < Size; ++ i)
		{
			if (Messages[i])
				continue;

			Messages[i] = new Gwen::Controls::Label(Canvas);
			Messages[i]->SetPos(20, 900 - 50 - 25 * i);
			Messages[i]->SetSize(1500, 30);
		}
	}

	void update(float const Elapsed)
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

	void addMessage(std::string const & Message, Gwen::Color const & Color = Gwen::Color(255, 255, 255, 255))
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


};

#endif
