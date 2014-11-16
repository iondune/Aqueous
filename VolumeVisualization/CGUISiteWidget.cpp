
#include "CGUISiteWidget.h"
#include "CMainMenuState.h"
#include "CSite.h"


CGUISiteWidget::CGUISiteWidget(CSite * Site)
{
	this->Site = Site;

	auto Canvas = GUIManager->GetCanvas();

	auto Window = new Gwen::Controls::WindowControl(Canvas);
	Window->SetTitle(Site->GetName());
	Window->SetBounds(30, 60, 1000, 750);
	Window->MakeModal();

	auto OKButton = new Gwen::Controls::Button(Window);
	OKButton->SetText("Open");
	OKButton->SetBounds(10, 10, 200, 30);
	OKButton->onPress.Add(this, & CGUISiteWidget::OnOpenSite);

	auto TabControl = new Gwen::Controls::TabControl(Window);
	TabControl->Dock(Gwen::Pos::Fill);
	TabControl->SetMargin(Gwen::Margin(0, 60, 0, 0));
	auto LocationsTab = TabControl->AddPage("Locations")->GetPage();
	auto DataSetsTab = TabControl->AddPage("Data Sets")->GetPage();

	auto LocationScrollControl = new Gwen::Controls::ScrollControl(LocationsTab);
	LocationScrollControl->Dock(Gwen::Pos::Fill);

	int Offset = 0;
	int Index = 1;
	for (auto Location : Site->GetLocations())
	{
		auto Panel = new Gwen::Controls::GroupBox(LocationScrollControl);
		stringstream s;
		s << "Location " << Index;
		Panel->SetText(s.str());
		Panel->SetBounds(10, Offset, 1000-50, 250);

		auto Label1 = new Gwen::Controls::Label(Panel);
		Label1->SetBounds(10, 30, 1000, 1000);
		Label1->SetText("Lower Bound");

		auto Label2 = new Gwen::Controls::Label(Panel);
		Label2->SetBounds(10, 30+30, 1000, 1000);
		Label2->SetText("Longitude:");

		auto Text1 = new Gwen::Controls::TextBox(Panel);
		Text1->SetBounds(10+150, 30+30, 100, 20);
		s.str(string());
		s << Location->LowerBound.Longitude;
		Text1->SetText(s.str());

		auto Label3 = new Gwen::Controls::Label(Panel);
		Label3->SetBounds(10, 30+60, 1000, 1000);
		Label3->SetText("Latitude:");

		auto Text2 = new Gwen::Controls::TextBox(Panel);
		Text2->SetBounds(10+150, 30+60, 100, 20);
		s.str(string());
		s << Location->LowerBound.Latitude;
		Text2->SetText(s.str());

		auto Label4 = new Gwen::Controls::Label(Panel);
		Label4->SetBounds(10+500, 30, 1000, 1000);
		Label4->SetText("Upper Bound");

		auto Label5 = new Gwen::Controls::Label(Panel);
		Label5->SetBounds(10+500, 30+30, 1000, 1000);
		Label5->SetText("Longitude:");

		auto Label6 = new Gwen::Controls::Label(Panel);
		Label6->SetBounds(10+500, 30+60, 1000, 1000);
		Label6->SetText("Latitude:");

		auto Text5 = new Gwen::Controls::TextBox(Panel);
		Text5->SetBounds(10+500+150, 30+30, 100, 20);
		s.str(string());
		s << Location->UpperBound.Longitude;
		Text5->SetText(s.str());

		auto Text6 = new Gwen::Controls::TextBox(Panel);
		Text6->SetBounds(10+500+150, 30+60, 100, 20);
		s.str(string());
		s << Location->UpperBound.Latitude;
		Text6->SetText(s.str());

		auto Label7 = new Gwen::Controls::Label(Panel);
		Label7->SetBounds(10, 30+120, 1000, 1000);
		Label7->SetText("Color File:");

		auto Text7 = new Gwen::Controls::TextBox(Panel);
		Text7->SetBounds(10+150, 30+120, 450, 20);
		s.str(string());
		s << Location->ColorFile;
		Text7->SetText(s.str());

		auto Label8 = new Gwen::Controls::Label(Panel);
		Label8->SetBounds(10, 30+150, 1000, 1000);
		Label8->SetText("Height File:");

		auto Text8 = new Gwen::Controls::TextBox(Panel);
		Text8->SetBounds(10+150, 30+150, 450, 20);
		s.str(string());
		s << Location->HeightFile;
		Text8->SetText(s.str());

		auto Label9 = new Gwen::Controls::Label(Panel);
		Label9->SetBounds(10, 30+180, 1000, 1000);
		Label9->SetText("Bathy File:");

		auto Text9 = new Gwen::Controls::TextBox(Panel);
		Text9->SetBounds(10+150, 30+180, 450, 20);
		s.str(string());
		s << Location->BathymetryFile;
		Text9->SetText(s.str());

		Offset += 300;
	}

	auto DataSetsScrollControl = new Gwen::Controls::ScrollControl(DataSetsTab);
	DataSetsScrollControl->Dock(Gwen::Pos::Fill);

	Offset = 0;
	Index = 1;
	for (auto DataSet : Site->GetDataSets())
	{
		auto Panel = new Gwen::Controls::GroupBox(DataSetsScrollControl);
		stringstream s;
		s << "Data Set " << Index;
		Panel->SetText(s.str());
		Panel->SetBounds(10, Offset, 1000-50, 320);

		auto Label1 = new Gwen::Controls::Label(Panel);
		Label1->SetBounds(10, 30, 1000, 1000);
		Label1->SetText("Position Field (X):");

		auto Text1 = new Gwen::Controls::TextBox(Panel);
		Text1->SetBounds(10+150, 30, 100, 20);
		s.str(string());
		s << DataSet->Traits.PositionXField;
		Text1->SetText(s.str());

		auto Label2 = new Gwen::Controls::Label(Panel);
		Label2->SetBounds(10, 30+30, 1000, 1000);
		Label2->SetText("Position Field (Y):");

		auto Text2 = new Gwen::Controls::TextBox(Panel);
		Text2->SetBounds(10+150, 30+30, 100, 20);
		s.str(string());
		s << DataSet->Traits.PositionYField;
		Text2->SetText(s.str());

		auto Label3 = new Gwen::Controls::Label(Panel);
		Label3->SetBounds(10, 30+60, 1000, 1000);
		Label3->SetText("Position Field (Z):");

		auto Text3 = new Gwen::Controls::TextBox(Panel);
		Text3->SetBounds(10+150, 30+60, 100, 20);
		s.str(string());
		s << DataSet->Traits.PositionZField;
		Text3->SetText(s.str());

		auto AssetsScrollControl = new Gwen::Controls::ScrollControl(Panel);
		AssetsScrollControl->SetBounds(10, 30+90, 920, 150);

		int InnerOffset = 0;
		int InnerIndex = 0;
		for (auto Asset : DataSet->Assets)
		{
			auto Panel = new Gwen::Controls::GroupBox(AssetsScrollControl);
			stringstream s;
			s << "Asset " << ++InnerIndex;
			Panel->SetText(s.str());
			Panel->SetBounds(10, InnerOffset, 920-50, 125);

			auto Label1 = new Gwen::Controls::Label(Panel);
			Label1->SetBounds(10, 30, 1000, 1000);
			Label1->SetText("File:");

			auto Text1 = new Gwen::Controls::TextBox(Panel);
			Text1->SetBounds(10+150, 30, 600, 20);
			Text1->SetText(Asset.File);

			auto Label2 = new Gwen::Controls::Label(Panel);
			Label2->SetBounds(10, 30+30, 1000, 1000);
			Label2->SetText("Parser:");

			auto Text2 = new Gwen::Controls::TextBox(Panel);
			Text2->SetBounds(10+150, 30+30, 600, 20);
			Text2->SetText(Asset.Parser);

			InnerOffset += 150;
		}

		auto AddAssetButton = new Gwen::Controls::Button(AssetsScrollControl);
		AddAssetButton->SetText("New");
		AddAssetButton->SetBounds(10, InnerOffset, 200, 30);
		
		Offset += 300;
	}
}

void CGUISiteWidget::OnOpenSite(Gwen::Controls::Base * Control)
{
	SingletonPointer<CProgramContext> Context;

	Context->CurrentSite = Site;
	CMainMenuState::Get().LoadData();
}
