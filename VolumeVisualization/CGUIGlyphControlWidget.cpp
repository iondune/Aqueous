#include "CGUIGlyphControlWidget.h"

#include "CProgramContext.h"
#include "CMainState.h"
#include "CTerrainSceneObject.h"
#include "CGlyphSceneObject.h"

#include <Gwen/Controls.h>
#include <Gwen/Controls/ComboBox.h>


CGUIGlyphControlWidget::CGUIGlyphControlWidget()
{
	CProgramContext & Context = CProgramContext::Get();

	Window = new Gwen::Controls::WindowControl(GUIManager->GetCanvas());
	Window->SetDeleteOnClose(false);
	Window->SetBounds(1200, 670, 330, 215);
	Window->SetTitle("Glyph Controls");
	//Window->SetHidden(true);

	EnableButton = new Gwen::Controls::Button(Window);
	EnableButton->SetBounds(15, 10, 290, 25);
	EnableButton->SetText(Context.Scene.GlyphSceneObject->isVisible() ? "Disable Glyph Visual" : "Enable Glyph Visual");
	EnableButton->onPress.Add(this, & CGUIGlyphControlWidget::OnToggleGlyphs);

	// Slider Panel
	{
		Gwen::Controls::Label * SliderLabel = new Gwen::Controls::Label(Window);
		SliderLabel->SetFont(GUIManager->GetRegularFont());
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
		
		SliderLabel = new Gwen::Controls::Label(Window);
		SliderLabel->SetFont(GUIManager->GetRegularFont());
		SliderLabel->SetText(L"Glyph Size:");
		SliderLabel->SetBounds(10, 10 + 45 + 25 + 45, 300, 40);
		SliderLabel->SetTextColor(Gwen::Color(50, 20, 20, 215));

		Gwen::Controls::HorizontalSlider * GlyphSizeSlider = new Gwen::Controls::HorizontalSlider(Window);
		GlyphSizeSlider->SetBounds(10, 10 + 45 + 25 + 15 + 45, 300, 40);
		GlyphSizeSlider->SetRange(1 / 256.f, 1 / 24.f);
		GlyphSizeSlider->SetFloatValue(1 / 64.f);

		// Wire Up Events
		GlyphSizeSlider->onValueChanged.Add(this, & CGUIGlyphControlWidget::OnSizeSlider);
	}
}

void CGUIGlyphControlWidget::SetButtonTitle()
{
	CProgramContext * Context = & CProgramContext::Get();
	
	if (Context->Scene.GlyphSceneObject->isVisible())
		EnableButton->SetText("Disable Glyph Visual");
	else
		EnableButton->SetText("Enable Glyph Visual");
}

void CGUIGlyphControlWidget::OnToggleGlyphs(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::Get();

	if (! Context->Scene.GlyphSceneObject->isVisible())
	{
		Context->Scene.GlyphSceneObject->setVisible(true);
		GUIContext->GetConsole()->addMessage("Glyph View Enabled");
		SetButtonTitle();
	}
	else
	{
		Context->Scene.GlyphSceneObject->setVisible(false);
		GUIContext->GetConsole()->addMessage("Glyph View Disabled");
		SetButtonTitle();
	}
}

void CGUIGlyphControlWidget::OnSelectPoint(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::Get();
	Context->Scene.GlyphSceneObject->setVisible(! Context->Scene.GlyphSceneObject->isVisible());
	SetButtonTitle();
}

void CGUIGlyphControlWidget::OnSelectGrid(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::Get();
	Context->Scene.GlyphSceneObject->setVisible(! Context->Scene.GlyphSceneObject->isVisible());
	SetButtonTitle();
}

void CGUIGlyphControlWidget::OnSizeSlider(Gwen::Controls::Base * Control)
{
	CProgramContext & Context = CProgramContext::Get();
	Gwen::Controls::Slider * Bar = (Gwen::Controls::Slider *) Control;
	Context.Scene.GlyphSceneObject->GlyphSize = Bar->GetFloatValue();
}

void CGUIGlyphControlWidget::toggle()
{
	Window->SetHidden(Window->Visible());
}
