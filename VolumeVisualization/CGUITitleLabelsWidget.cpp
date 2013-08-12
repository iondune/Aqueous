#include "CGUITitleLabelsWidget.h"

#include "CProgramContext.h"
#include "SciDataManager.h"
#include "CMainMenuState.h"

#include <iomanip>


CGUITitleLabelsWidget::CGUITitleLabelsWidget(SciDataManager * DataManager)
{
	static Range ValueRange = DataManager->getRawValues().getValueRange("o1", 5.0);

	std::wstringstream s;
	s << std::fixed;
	s << "Range (";
	s << std::setprecision(3);
	s << ValueRange.first;
	s << ", ";
	s << ValueRange.second;
	s << ")";

	// Top Label
	Gwen::Controls::Label * BigLabel = new Gwen::Controls::Label(GUIManager->getCanvas());
	BigLabel->SetFont(GUIManager->getLargeFont());
	BigLabel->SetText(Gwen::UnicodeString(L"Dataset: ") + Gwen::UnicodeString(CMainMenuState::get().DataSetName.begin(), CMainMenuState::get().DataSetName.end()));
	BigLabel->SetBounds(10, 10, 1590, 300);
	BigLabel->SetTextColor(Gwen::Color(235, 255, 235, 215));
	BigLabel->SetHidden(true);

	// Second Label
	Gwen::Controls::Label * MediumLabel = new Gwen::Controls::Label(GUIManager->getCanvas());
	MediumLabel->SetFont(GUIManager->getMediumFont());
	MediumLabel->SetText(Gwen::UnicodeString(L"Current Field: Avg Oxy - ") + Gwen::UnicodeString(s.str()));
	MediumLabel->SetBounds(20, 70, 1000, 300);
	MediumLabel->SetTextColor(Gwen::Color(235, 235, 255, 215));
	MediumLabel->SetHidden(true);

	// Volume Range Label
	VolumeRangeIndicator = new Gwen::Controls::Label(GUIManager->getCanvas());
	VolumeRangeIndicator->SetFont(GUIManager->getMediumFont());
	VolumeRangeIndicator->SetBounds(20, 110, 1000, 300);
	VolumeRangeIndicator->SetTextColor(Gwen::Color(255, 235, 235, 215));
	VolumeRangeIndicator->SetHidden(true);

	// Volume Range Label
	VolumeCalculationIndicator = new Gwen::Controls::Label(GUIManager->getCanvas());
	VolumeCalculationIndicator->SetFont(GUIManager->getMediumFont());
	VolumeCalculationIndicator->SetBounds(20, 150, 1000, 300);
	VolumeCalculationIndicator->SetTextColor(Gwen::Color(255, 235, 235, 215));
	VolumeCalculationIndicator->SetHidden(true);
}

void CGUITitleLabelsWidget::resetVolumeRangeIndicator(SciDataManager * DataManager)
{
	static Range ValueRange = DataManager->getRawValues().getValueRange("o1", 5.0);

	{
		std::wstringstream s;
		s << std::fixed;
		s << "Value Range: ";
		s << std::setprecision(3);
		s << (CProgramContext::get().Scene.VolumeSceneObject->Control.EmphasisLocation * (ValueRange.second - ValueRange.first) + ValueRange.first) / 100.f;
		s << " ± ";
		s << std::setprecision(4);
		s << (CProgramContext::get().Scene.VolumeSceneObject->Control.LocalRange / 2.f * (ValueRange.second - ValueRange.first)) / 100.f;
		VolumeRangeIndicator->SetText(s.str());
	}
	
	{
		static Range ValueRange = DataManager->getGridValues().getValueRange("o1", 5.0);
		static Range XValueRange = DataManager->getRawValues().getValueRange("x", 5.0);
		static Range YValueRange = DataManager->getRawValues().getValueRange("y", 5.0);
		YValueRange.first = 0.0;
		static Range ZValueRange = DataManager->getRawValues().getValueRange("z", 5.0);

		double EntireVolume = 1.0;
		EntireVolume *= XValueRange.second - XValueRange.first;
		EntireVolume *= YValueRange.second - YValueRange.first;
		EntireVolume *= ZValueRange.second - ZValueRange.first;

		double UnitVolume = EntireVolume / 24.0 / 24.0 / 24.0;
		//printf("Entire Volume: %f UnitVolume %f\n", EntireVolume, UnitVolume);

		std::wstringstream s;
		s << "Volume: ";
		s << std::setprecision(3);
		s << std::scientific;
		s << DataManager->getGridVolume("Avg Oxy", CProgramContext::get().Scene.VolumeSceneObject->Control.EmphasisLocation * (ValueRange.second - ValueRange.first) + ValueRange.first,
			CProgramContext::get().Scene.VolumeSceneObject->Control.LocalRange / 2.f * (ValueRange.second - ValueRange.first), 2) * UnitVolume;
		s << " m^3";
		VolumeCalculationIndicator->SetText(s.str());
	}
}

void CGUITitleLabelsWidget::clearVolumeRangeIndicator()
{
	VolumeRangeIndicator->SetText(L"");
	VolumeCalculationIndicator->SetText(L"");
}
