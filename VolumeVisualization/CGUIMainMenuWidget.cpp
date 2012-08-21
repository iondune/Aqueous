#include "CGUIMainMenuWidget.h"

#include "CMainMenuState.h"
#include "dirent.h"

#include <sstream>


void CGUIMainMenuWidget::createDataSetButtons()
{
	DIR * dir;
	dirent * ent;

	int Height = 150;

	dir = opendir("Datasets/");
	if (dir != NULL)
	{
		while ((ent = readdir (dir)) != NULL)
		{
			std::string const FileName = ent->d_name;
			if (FileName == "." || FileName == "..")
				continue;

			Gwen::Controls::Button * Button = new Gwen::Controls::Button(GUIManager->getCanvas());
			Button->SetBounds(50, Height, 290, 25);
			Button->SetText(FileName);
			Button->onPress.Add(this, & CGUIMainMenuWidget::OnSelectDataSet);
			DataSetButtons.push_back(Button);

			Height += 35;
		}
		closedir(dir);
	}

	Height += 35;
	NewDataSetButton = new Gwen::Controls::Button(GUIManager->getCanvas());
	NewDataSetButton->SetBounds(50, Height, 290, 25);
	NewDataSetButton->SetText("Create New Data Set");
	NewDataSetButton->onPress.Add(this, & CGUIMainMenuWidget::OnSelectDataSet);
}

CGUIMainMenuWidget::CGUIMainMenuWidget()
	: NewDataSetButton(0)
{
	// Top Label
	Gwen::Controls::Label * BigLabel = new Gwen::Controls::Label(GUIManager->getCanvas());
	BigLabel->SetFont(GUIManager->getLargeFont());
	BigLabel->SetText(L"Main Menu");
	BigLabel->SetBounds(10, 10, 1590, 300);
	BigLabel->SetTextColor(Gwen::Color(235, 255, 235, 215));

	// Second Label
	Gwen::Controls::Label * MediumLabel = new Gwen::Controls::Label(GUIManager->getCanvas());
	MediumLabel->SetFont(GUIManager->getMediumFont());
	MediumLabel->SetText(L"Available Data Sets:");
	MediumLabel->SetBounds(20, 70, 600, 300);
	MediumLabel->SetTextColor(Gwen::Color(235, 235, 255, 215));

	createDataSetButtons();
}

void CGUIMainMenuWidget::OnSelectDataSet(Gwen::Controls::Base * Control)
{
	if (Control == NewDataSetButton)
	{
		//Gwen::Controls::WindowControl * Window = new Gwen::Controls::WindowControl(GUIManager->getCanvas());
		//Window->SetBounds(300, 300, 150, 50);
		//Window->SetTitle("Please Select Data Set Name");
		CMainMenuState::get().createDataSet();

		for (auto it = DataSetButtons.begin(); it != DataSetButtons.end(); ++ it)
		{
			(* it)->DelayedDelete();
		}

		NewDataSetButton->DelayedDelete();
		NewDataSetButton = 0;
		DataSetButtons.clear();

		createDataSetButtons();
	}
	else
	{
		std::wstring const & FileName = ((Gwen::Controls::Button *) Control)->GetText();
		CMainMenuState::get().loadData(std::string(FileName.begin(), FileName.end()));
	}
}
