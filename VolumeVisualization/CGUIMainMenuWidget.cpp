#include "CGUIMainMenuWidget.h"

#include "CMainMenuState.h"
#include "dirent.h"

#include <sstream>

#include <Gwen/Controls/Menu.h>


void CGUIMainMenuWidget::createDataSetButtons()
{
	DIR * dir = opendir("Datasets/");

	if (dir)
	{
		dirent * ent;
		while (ent = readdir(dir))
		{
			std::string const FileName = ent->d_name;
			if (FileName == "." || FileName == "..")
				continue;

			ListBox->AddItem(FileName, FileName);
		}
		closedir(dir);
	}
}

class MenuDropDown : public Gwen::Event::Handler
{

	Gwen::Controls::MenuItem * MenuItem;

public:

	MenuDropDown(Gwen::Controls::MenuItem * menuItem)
		: MenuItem(menuItem)
	{}

	void OnPress()
	{
		Gwen::Controls::Menu * Menu = new Gwen::Controls::Menu(MenuItem->GetCanvas());
		Menu->MoveTo(MenuItem->GetPos().x, MenuItem->GetPos().y + 22);
		Menu->AddItem("Open");//->onPress.Add(new ActionItem());
		Gwen::Controls::MenuItem * New = Menu->AddItem("New");
		New->onPress.Add(this, & CGUIMainMenuWidget::OnCreateDataSet);
		Menu->AddItem("Close");
		Menu->AddItem("Quit");
	}

};

CGUIMainMenuWidget::CGUIMainMenuWidget()
	: NewDataSetButton(0)
{
	Menu = new Gwen::Controls::MenuStrip(GUIManager->getCanvas());
	Gwen::Controls::MenuItem * File = Menu->AddItem("File");
	File->onPress.Add(new MenuDropDown(File), & MenuDropDown::OnPress);

	Window = new Gwen::Controls::WindowControl(GUIManager->getCanvas());
	Window->SetTitle("Data Sets");
	Window->SetBounds(30, 60, 650 + 40 + 30, 550);
	Window->SetDeleteOnClose(false);
	Window->SetClosable(false);

	Gwen::Controls::Label * MediumLabel = new Gwen::Controls::Label(Window);
	MediumLabel->SetFont(GUIManager->getMediumFont());
	MediumLabel->SetText(L"Available Data Sets:");
	MediumLabel->SetBounds(10, 10, 600, 300);
	MediumLabel->SetTextColor(Gwen::Color(35, 35, 55, 215));

	ListBox = new Gwen::Controls::ListBox(Window);
	ListBox->SetBounds(10, 50, 690, 410);
	
	Gwen::Controls::Button * OKButton = new Gwen::Controls::Button(Window);
	OKButton->SetText("OK");
	OKButton->SetBounds(10+130, 60+400+16, 200, 30);
	OKButton->onPress.Add(this, & CGUIMainMenuWidget::OnSelectDataSet);

	Gwen::Controls::Button * CancelButton = new Gwen::Controls::Button(Window);
	CancelButton->SetText("Cancel");
	CancelButton->SetBounds(10+130+200+20, 60+400+16, 200, 30);

	createDataSetButtons();
}

void CGUIMainMenuWidget::OnSelectDataSet(Gwen::Controls::Base * Control)
{
	std::string const & FileName = ListBox->GetSelectedRowName();
	std::cout << "Filename selected: " << FileName;
	CMainMenuState::get().loadData(std::string(FileName.begin(), FileName.end()));
}

void CGUIMainMenuWidget::OnCreateDataSet(Gwen::Controls::Base * Control)
{	
	//Gwen::Controls::WindowControl * Window = new Gwen::Controls::WindowControl(GUIManager->getCanvas());
	//Window->SetBounds(300, 300, 150, 50);
	//Window->SetTitle("Please Select Data Set Name");
	CMainMenuState::get().createDataSet();

	ListBox->Clear();
	createDataSetButtons();
}
