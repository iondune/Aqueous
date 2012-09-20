#include "CGUITitleLabelsWidget.h"

#include "CProgramContext.h"
#include "SciDataManager.h"

#include <iomanip>


CGUITitleLabelsWidget::CGUITitleLabelsWidget()
{
	// Top Label
	Gwen::Controls::Label * BigLabel = new Gwen::Controls::Label(GUIManager->getCanvas());
	BigLabel->SetFont(GUIManager->getLargeFont());
	BigLabel->SetText(Gwen::UnicodeString(L"Dataset: Denmark [Mission 1]") + Gwen::UnicodeString());
	BigLabel->SetBounds(10, 10, 1590, 300);
	BigLabel->SetTextColor(Gwen::Color(235, 255, 235, 215));

	// Second Label
	Gwen::Controls::Label * MediumLabel = new Gwen::Controls::Label(GUIManager->getCanvas());
	MediumLabel->SetFont(GUIManager->getMediumFont());
	MediumLabel->SetText(Gwen::UnicodeString(L"Current Field: Avg Oxy") + Gwen::UnicodeString());
	MediumLabel->SetBounds(20, 70, 600, 300);
	MediumLabel->SetTextColor(Gwen::Color(235, 235, 255, 215));

	// Volume Range Label
	VolumeRangeIndicator = new Gwen::Controls::Label(GUIManager->getCanvas());
	VolumeRangeIndicator->SetFont(GUIManager->getMediumFont());
	VolumeRangeIndicator->SetBounds(20, 110, 900, 300);
	VolumeRangeIndicator->SetTextColor(Gwen::Color(255, 235, 235, 215));

	// Volume Range Label
	VolumeCalculationIndicator = new Gwen::Controls::Label(GUIManager->getCanvas());
	VolumeCalculationIndicator->SetFont(GUIManager->getMediumFont());
	VolumeCalculationIndicator->SetBounds(20, 150, 900, 300);
	VolumeCalculationIndicator->SetTextColor(Gwen::Color(255, 235, 235, 215));
}

void CGUITitleLabelsWidget::resetVolumeRangeIndicator(SciDataManager * DataManager)
{
	static Range ValueRange = DataManager->GridValues.getValueRange("\"Avg Oxy\"", 5.0);

	{
		std::wstringstream s;
		s << std::fixed;
		s << "Value Range: ";
		s << std::setprecision(3);
		s << (CProgramContext::get().Scene.VolumeSceneObject->Control.EmphasisLocation * (ValueRange.second - ValueRange.first) + ValueRange.first);
		s << " ± ";
		s << std::setprecision(4);
		s << (CProgramContext::get().Scene.VolumeSceneObject->Control.LocalRange / 2.f * (ValueRange.second - ValueRange.first));
		VolumeRangeIndicator->SetText(s.str());
	}
	
	if (false) {
		std::wstringstream s;
		s << "Volume: ";
		s << std::setprecision(3);
		s << std::scientific;
		s << DataManager->getGridVolume("\"Avg Oxy\"", CProgramContext::get().Scene.VolumeSceneObject->Control.EmphasisLocation * (ValueRange.second - ValueRange.first) + ValueRange.first,
			CProgramContext::get().Scene.VolumeSceneObject->Control.LocalRange / 2.f * (ValueRange.second - ValueRange.first), 2) * 20.0 * 20.0;
		s << " m^3";
		VolumeCalculationIndicator->SetText(s.str());
	}
}

void CGUITitleLabelsWidget::clearVolumeRangeIndicator()
{
	VolumeRangeIndicator->SetText(L"");
}
