
#include "CGUISiteWidget.h"
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
	OKButton->onPress.Add(this, & CGUIMainMenuWidget::OnSelectDataSet);

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
		Panel->SetBounds(10, Offset, 1000-50, 250);

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

		Offset += 300;
	}
}
