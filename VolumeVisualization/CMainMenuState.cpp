#include "CMainMenuState.h"

#include "CMainState.h"
#include "ColorMappers.h"
#include "CGUILoadingWidget.h"
#include "CDataLoadingThread.h"
#include "SciDataManager.h"


CMainMenuState::CMainMenuState()
{}

void CMainMenuState::begin()
{
	Context->GUIContext->setupMenuState();
}

void CMainMenuState::end()
{
	Context->GUIContext->clear();
}

#include "CGlyphSceneObject.h"

void CMainMenuState::OnRenderStart(float const Elapsed)
{
	// Let loading thread run
	sf::sleep(sf::seconds(0.05f));
	Thread.Sync();

	Context->GUIContext->draw(Elapsed, true);
	CApplication::get().swapBuffers();
}

void CMainMenuState::OnWindowResized(SWindowResizedEvent const & Event)
{
	Context->GUIContext->getCanvas()->SetSize(Event.Size.X, Event.Size.Y);
	Context->GUIContext->getCanvas()->Invalidate();
	Context->GUIContext->getCanvas()->InvalidateChildren(true);
}

void CMainMenuState::loadData(std::string const & FileName)
{
	DataSetName = FileName;

	std::stringstream s;
	s << "Datasets/";
	s << FileName;

	Thread.Context = Context;
	Context->GUIContext->addWidget(Thread.LoadingWidget = new CGUILoadingWidget("Loading data and initializing scene elements"));
	Thread.Run(s.str());
}

void CMainMenuState::createDataSet()
{
	SciDataParserCSV * Parser1 = new SciDataParserSmartTether();
	Parser1->Manager = Context->DataManager;
	Parser1->FieldDelim = ',';
	Parser1->ValueDelim = ',';
	/*Parser1->FieldNames = false;
	std::vector<std::string> Fields;
	Fields.push_back("GPGGA Base Position");
	Fields.push_back("x");
	Fields.push_back("y");
	Fields.push_back("n");
	Fields.push_back("z");
	Fields.push_back("e");
	Parser1->Fields = Fields;*/
	Parser1->load("smarttether10.csv");

	int counter = 0;
	for (auto Value : Context->DataManager->getRawValues().getValues())
		Value.addField("timeStamp") = counter++;

	//Context->DataManager->createGridDataFromRawValues(FullRange, 5.0, "Avg Oxy");

	Context->DataManager->writeToFile("Datasets/VallettaMCC1.dat");
}
