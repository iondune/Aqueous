#include "CGUITitleLabelsWidget.h"

#include "CProgramContext.h"
#include "SciDataManager.h"

#include <iomanip>


CGUITitleLabelsWidget::CGUITitleLabelsWidget()
{
	// Top Label
	Gwen::Controls::Label * BigLabel = new Gwen::Controls::Label(GUIManager->getCanvas());
	BigLabel->SetFont(GUIManager->getLargeFont());
	BigLabel->SetText(Gwen::UnicodeString(L"Dataset: Hopavågen Bay - June 12, 2010 19:48 [T1]") + Gwen::UnicodeString());
	BigLabel->SetBounds(10, 10, 1590, 300);
	BigLabel->SetTextColor(Gwen::Color(235, 255, 235, 215));

	// Second Label
	Gwen::Controls::Label * MediumLabel = new Gwen::Controls::Label(GUIManager->getCanvas());
	MediumLabel->SetFont(GUIManager->getMediumFont());
	MediumLabel->SetText(Gwen::UnicodeString(L"Current Field: DO2 (microMol/L)") + Gwen::UnicodeString());
	MediumLabel->SetBounds(20, 70, 600, 300);
	MediumLabel->SetTextColor(Gwen::Color(235, 235, 255, 215));

	// Volume Range Label
	VolumeRangeIndicator = new Gwen::Controls::Label(GUIManager->getCanvas());
	VolumeRangeIndicator->SetFont(GUIManager->getMediumFont());
	VolumeRangeIndicator->SetBounds(20, 110, 900, 300);
	VolumeRangeIndicator->SetTextColor(Gwen::Color(255, 235, 235, 215));
}

void CGUITitleLabelsWidget::resetVolumeRangeIndicator(SciDataManager * DataManager)
{
	static Range ValueRange = DataManager->GridValues.getValueRange("o1", 5.0);
	std::wstringstream s;
	s << std::fixed;
	s << "Value Range: ";
	s << std::setprecision(1);
	s << (CProgramContext::get().Scene.VolumeSceneObject->Control.EmphasisLocation * (ValueRange.second - ValueRange.first) + ValueRange.first) / 100.f;
	s << " ± ";
	s << std::setprecision(2);
	s << (CProgramContext::get().Scene.VolumeSceneObject->Control.LocalRange / 2.f * (ValueRange.second - ValueRange.first)) / 100.f;
	s << " (Volume: " << std::endl;
	s << std::setprecision(1);
	s << DataManager->getGridVolume("o1", CProgramContext::get().Scene.VolumeSceneObject->Control.EmphasisLocation * (ValueRange.second - ValueRange.first) + ValueRange.first,
		CProgramContext::get().Scene.VolumeSceneObject->Control.LocalRange / 2.f * (ValueRange.second - ValueRange.first), 0);
	s << " or " << std::endl;
	s << DataManager->getGridVolume("o1", CProgramContext::get().Scene.VolumeSceneObject->Control.EmphasisLocation * (ValueRange.second - ValueRange.first) + ValueRange.first,
		CProgramContext::get().Scene.VolumeSceneObject->Control.LocalRange / 2.f * (ValueRange.second - ValueRange.first), 1);
	s << " m^3)";
	VolumeRangeIndicator->SetText(s.str());
}

void CGUITitleLabelsWidget::clearVolumeRangeIndicator()
{
	VolumeRangeIndicator->SetText(L"");
}
