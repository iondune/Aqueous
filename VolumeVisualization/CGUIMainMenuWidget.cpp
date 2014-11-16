
#include "CGUIMainMenuWidget.h"

#include "CMainMenuState.h"
#include "CProgramContext.h"
#include "CSite.h"
#include "CGUISiteWidget.h"

#include <sstream>

#include <Gwen/Controls/Menu.h>


void CGUIMainMenuWidget::CreateSitePanels()
{
	auto Canvas = GUIManager->GetCanvas();

	auto ScrollControl = new Gwen::Controls::ScrollControl(Canvas);
	ScrollControl->Dock(Gwen::Pos::Fill);
	ScrollControl->SetMargin(Gwen::Margin(0, 60, 0, 0));

	int Offset = 10;
	for (auto FileName : Directory::Read("Sites/"))
	{
		auto SiteButton = new Gwen::Controls::Button(ScrollControl);
		SiteButton->SetText(FileName);
		SiteButton->SetBounds(10, Offset, 200, 30);
		SiteButton->onPress.Add(this, & CGUIMainMenuWidget::OnSelectSite);

		Offset += 30 + 10;
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
	Menu = new Gwen::Controls::MenuStrip(GUIManager->GetCanvas());
	Gwen::Controls::MenuItem * File = Menu->AddItem("File");
	File->onPress.Add(new MenuDropDown(File), & MenuDropDown::OnPress);

	auto Canvas = GUIManager->GetCanvas();

	
	//PageControl = new Gwen::Controls::PageControl(Canvas);
	//PageControl->SetUseFinishButton(false);
	//PageControl->RemoveAllChildren();
	//PageControl->Dock(Gwen::Pos::Fill);
	//PageControl->SetPageCount(2);
	//SitePage = PageControl->GetPage(0);
	
	Gwen::Controls::Label * Title = new Gwen::Controls::Label(Canvas);
	Title->SetFont(GUIManager->GetMediumFont());
	Title->SetText(L"Available Sites:");
	Title->SetBounds(10, 40, 600, 100);
	Title->SetTextColor(Gwen::Color(235, 235, 255, 215));

	CreateSitePanels();

	/*Window = new Gwen::Controls::WindowControl(GUIManager->GetCanvas());
	Window->SetTitle("Sites");
	Window->SetBounds(30, 60, 650 + 40 + 30, 550);
	Window->SetDeleteOnClose(false);
	Window->SetClosable(false);

	Gwen::Controls::Label * MediumLabel = new Gwen::Controls::Label(Window);
	MediumLabel->SetFont(GUIManager->GetMediumFont());
	MediumLabel->SetText(L"Available Data Sets:");
	MediumLabel->SetBounds(10, 10, 600, 300);
	MediumLabel->SetTextColor(Gwen::Color(35, 35, 55, 215));

	ListBox = new Gwen::Controls::ListBox(Window);
	ListBox->SetBounds(10, 50, 690, 410);

	
	Gwen::Controls::Button * OKButton = new Gwen::Controls::Button(Window);
	OKButton->SetText("Select Cite");
	OKButton->SetBounds(10+130, 60+400+16, 200, 30);
	OKButton->onPress.Add(this, & CGUIMainMenuWidget::OnSelectDataSet);*/
}

void CGUIMainMenuWidget::OnSelectSite(Gwen::Controls::Base * Control)
{
	SingletonPointer<CProgramContext> Context;

	string const & SiteName = ((Gwen::Controls::Button *) Control)->GetText().c_str();
	cout << "Site selected: " << SiteName << endl;
	
	auto Site = Context->CurrentSite = new CSite(SiteName);
	Site->ReadConfiguration();
	CGUISiteWidget * SiteWidget = new CGUISiteWidget(Site);

	//PageControl->NextPage();

	//auto Window = new Gwen::Controls::WindowControl(GUIManager->GetCanvas());
	//Window->SetTitle(SiteName);
	//Window->SetBounds(30, 60, 650 + 40 + 30, 550);
	//Window->MakeModal();

	//auto OKButton = new Gwen::Controls::Button(Window);
	//OKButton->SetText("Open");
	//OKButton->SetBounds(10+130, 60+400+16, 200, 30);
	//OKButton->onPress.Add(this, & CGUIMainMenuWidget::OnSelectDataSet);
}

void CGUIMainMenuWidget::OnCreateDataSet(Gwen::Controls::Base * Control)
{	
	//Gwen::Controls::WindowControl * Window = new Gwen::Controls::WindowControl(GUIManager->getCanvas());
	//Window->SetBounds(300, 300, 150, 50);
	//Window->SetTitle("Please Select Data Set Name");
	CMainMenuState::Get().CreateDataSet();

	ListBox->Clear();
	//CreateSitePanels();
}
