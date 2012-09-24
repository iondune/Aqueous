#include "CGUIGlyphControlWidget.h"

#include "CProgramContext.h"
#include "CMainState.h"
#include "CTerrainSceneObject.h"

#include <Gwen/Controls.h>
#include <Gwen/Controls/ComboBox.h>


CGUIGlyphControlWidget::CGUIGlyphControlWidget()
{
	Window = new Gwen::Controls::WindowControl(GUIManager->getCanvas());
	Window->SetDeleteOnClose(false);
	Window->SetBounds(1200, 670, 330, 160);
	Window->SetTitle("Glyph Controls");
	Window->SetHidden(true);

	EnableButton = new Gwen::Controls::Button(Window);
	EnableButton->SetBounds(15, 10, 290, 25);
	EnableButton->SetText("Enable Glyph Visual");
	EnableButton->onPress.Add(this, & CGUIGlyphControlWidget::OnToggleGlyphs);

	// Slider Panel
	{
		Gwen::Controls::Label * SliderLabel = new Gwen::Controls::Label(Window);
		SliderLabel->SetFont(GUIManager->getRegularFont());
		SliderLabel->SetText(L"Mode:");
		SliderLabel->SetBounds(10, 10 + 45, 300, 40);
		SliderLabel->SetTextColor(Gwen::Color(50, 20, 20, 215));

		Gwen::Controls::Button * PointsButton = new Gwen::Controls::Button(Window);
		PointsButton->SetBounds(15, 10 + 45 + 25, 140, 25);
		PointsButton->SetText("Source Points");
		PointsButton->onPress.Add(this, & CGUIGlyphControlWidget::OnSelectPoint);

		Gwen::Controls::Button * GridButton = new Gwen::Controls::Button(Window);
		GridButton->SetBounds(140 + 15 + 10, 10 + 45 + 25, 140, 25);
		GridButton->SetText("Floor Samples");
		GridButton->onPress.Add(this, & CGUIGlyphControlWidget::OnSelectGrid);

		// Wire Up Events
		//EmphasisSlider->onValueChanged.Add(		this,	& CGUIVolumeControlWidget::OnEmphasisSlider);
		//IntensitySlider->onValueChanged.Add(	this,	& CGUIVolumeControlWidget::OnIntensitySlider);
		//MinimumAlphaSlider->onValueChanged.Add(	this,	& CGUIVolumeControlWidget::OnMinimumAlphaSlider);
		//LocalRangeSlider->onValueChanged.Add(	this,	& CGUIVolumeControlWidget::OnLocalRangeSlider);
	}
}

void CGUIGlyphControlWidget::SetButtonTitle()
{
	CProgramContext * Context = & CProgramContext::get();
	
	if (Context->Scene.GridObject->isVisible() || Context->Scene.PointCloudObject->isVisible())
		EnableButton->SetText("Disable Glyph Visual");
	else
		EnableButton->SetText("Enable Glyph Visual");
}

void CGUIGlyphControlWidget::OnToggleGlyphs(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::get();

	if (! Context->Scene.PointCloudObject->isVisible() && ! Context->Scene.GridObject->isVisible())
	{
		Context->Scene.PointCloudObject->setVisible(true);
		Context->Scene.GridObject->setVisible(true);
		GUIContext->getConsole()->addMessage("Glyph View Enabled");
		SetButtonTitle();
	}
	else
	{
		Context->Scene.PointCloudObject->setVisible(false);
		Context->Scene.GridObject->setVisible(false);
		GUIContext->getConsole()->addMessage("Glyph View Disabled");
		SetButtonTitle();
	}
}

void CGUIGlyphControlWidget::OnSelectPoint(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::get();
	Context->Scene.PointCloudObject->setVisible(! Context->Scene.PointCloudObject->isVisible());
	SetButtonTitle();
}

void CGUIGlyphControlWidget::OnSelectGrid(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::get();
	Context->Scene.GridObject->setVisible(! Context->Scene.GridObject->isVisible());
	SetButtonTitle();
}

void CGUIGlyphControlWidget::toggle()
{
	Window->SetHidden(Window->Visible());
}
