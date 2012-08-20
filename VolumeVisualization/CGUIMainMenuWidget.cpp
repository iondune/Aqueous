#include "CGUIMainMenuWidget.h"

#include "CProgramContext.h"
#include "CMainState.h"
#include "CLoadContext.h"

#include "dirent.h"


CGUIMainMenuWidget::CGUIMainMenuWidget()
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

			Height += 35;
		}
		closedir(dir);
	}
}

void CGUIMainMenuWidget::OnSelectDataSet(Gwen::Controls::Base * Control)
{
	CApplication::get().getStateManager().setState(& CMainState::get());
	
	CProgramContext::get().LoadContext->addLabel(L"Loading Science Data...");
	CProgramContext::get().LoadContext->loadData();
	CProgramContext::get().LoadContext->addLabel(L"Application is Starting...");
}
