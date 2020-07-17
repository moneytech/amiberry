#include <cstdio>

#include <guisan.hpp>
#include <SDL_ttf.h>
#include <guisan/sdl.hpp>
#include "SelectorEntry.hpp"

#include "sysdeps.h"
#include "options.h"
#include "custom.h"
#include "gui_handling.h"

const int amigawidth_values[] = { 320, 362, 384, 640, 704, 720 };
const int amigaheight_values[] = { 200, 216, 240, 256, 262, 270, 284 };

class StringListModel : public gcn::ListModel
{
	std::vector<std::string> values;
public:
	StringListModel(const char* entries[], const int count)
	{
		for (auto i = 0; i < count; ++i)
			values.emplace_back(entries[i]);
	}

	int getNumberOfElements() override
	{
		return values.size();
	}

	int AddElement(const char* Elem)
	{
		values.emplace_back(Elem);
		return 0;
	}

	std::string getElementAt(const int i) override
	{
		if (i < 0 || i >= static_cast<int>(values.size()))
			return "---";
		return values[i];
	}
};

const char* fullscreen_modes[] = { "Windowed", "Fullscreen", "Full-window" };
StringListModel fullscreen_modes_list(fullscreen_modes, 3);

static gcn::Window* grpScalingMethod;
static gcn::RadioButton* optAuto;
static gcn::RadioButton* optNearest;
static gcn::RadioButton* optLinear;

static gcn::Window* grpLineMode;
static gcn::RadioButton* optSingle;
static gcn::RadioButton* optDouble;
static gcn::RadioButton* optScanlines;

static gcn::Window* grpAmigaScreen;
static gcn::Label* lblAmigaWidth;
static gcn::TextField* txtAmigaWidth;
static gcn::Slider* sldAmigaWidth;

static gcn::Label* lblAmigaHeight;
static gcn::TextField* txtAmigaHeight;
static gcn::Slider* sldAmigaHeight;

static gcn::CheckBox* chkAutoHeight;

static gcn::CheckBox* chkFrameskip;
static gcn::Slider* sldRefresh;
static gcn::CheckBox* chkAspect;

static gcn::Label* lblScreenmode;
static gcn::DropDown* cboScreenmode;

static gcn::Window* grpCentering;
static gcn::CheckBox* chkHorizontal;
static gcn::CheckBox* chkVertical;
static gcn::CheckBox* chkFlickerFixer;

class AmigaScreenActionListener : public gcn::ActionListener
{
public:
	void action(const gcn::ActionEvent& actionEvent) override
	{
		if (actionEvent.getSource() == sldAmigaWidth)
		{
			if (changed_prefs.gfx_monitor.gfx_size.width != amigawidth_values[static_cast<int>(sldAmigaWidth->getValue())])
				changed_prefs.gfx_monitor.gfx_size.width = amigawidth_values[static_cast<int>(sldAmigaWidth->getValue())];
		}
		else if (actionEvent.getSource() == sldAmigaHeight)
		{
			if (changed_prefs.gfx_monitor.gfx_size.height != amigaheight_values[static_cast<int>(sldAmigaHeight->getValue())])
				changed_prefs.gfx_monitor.gfx_size.height = amigaheight_values[static_cast<int>(sldAmigaHeight->getValue())];
		}
		else if (actionEvent.getSource() == chkAutoHeight)
			changed_prefs.gfx_auto_height = chkAutoHeight->isSelected();

		else if (actionEvent.getSource() == chkFrameskip)
		{
			changed_prefs.gfx_framerate = chkFrameskip->isSelected() ? 2 : 1;
			sldRefresh->setEnabled(chkFrameskip->isSelected());
			sldRefresh->setValue(changed_prefs.gfx_framerate);
		}

		else if (actionEvent.getSource() == sldRefresh)
			changed_prefs.gfx_framerate = static_cast<int>(sldRefresh->getValue());
		
		else if (actionEvent.getSource() == chkAspect)
			changed_prefs.gfx_correct_aspect = chkAspect->isSelected();

		else if (actionEvent.getSource() == cboScreenmode)
		{
			if (cboScreenmode->getSelected() == 0)
			{
				changed_prefs.gfx_apmode[0].gfx_fullscreen = GFX_WINDOW;
				changed_prefs.gfx_apmode[1].gfx_fullscreen = GFX_WINDOW;
			}
			else if (cboScreenmode->getSelected() == 1)
			{
				changed_prefs.gfx_apmode[0].gfx_fullscreen = GFX_FULLSCREEN;
				changed_prefs.gfx_apmode[1].gfx_fullscreen = GFX_FULLSCREEN;
			}
			else if (cboScreenmode->getSelected() == 2)
			{
				changed_prefs.gfx_apmode[0].gfx_fullscreen = GFX_FULLWINDOW;
				changed_prefs.gfx_apmode[1].gfx_fullscreen = GFX_FULLWINDOW;
			}
		}

		else if (actionEvent.getSource() == chkHorizontal)
			changed_prefs.gfx_xcenter = chkHorizontal->isSelected() ? 2 : 0;

		else if (actionEvent.getSource() == chkVertical)
			changed_prefs.gfx_ycenter = chkVertical->isSelected() ? 2 : 0;

		else if (actionEvent.getSource() == chkFlickerFixer)
			changed_prefs.gfx_scandoubler = chkFlickerFixer->isSelected();

		RefreshPanelDisplay();
	}
};

AmigaScreenActionListener* amigaScreenActionListener;

class ScalingMethodActionListener : public gcn::ActionListener
{
public:
	void action(const gcn::ActionEvent& actionEvent) override
	{
		if (actionEvent.getSource() == optAuto)
			changed_prefs.scaling_method = -1;
		else if (actionEvent.getSource() == optNearest)
			changed_prefs.scaling_method = 0;
		else if (actionEvent.getSource() == optLinear)
			changed_prefs.scaling_method = 1;
	}
};

static ScalingMethodActionListener* scalingMethodActionListener;

class LineModeActionListener : public gcn::ActionListener
{
public:
	void action(const gcn::ActionEvent& action_event) override
	{
		if (action_event.getSource() == optSingle)
		{
			changed_prefs.gfx_vresolution = VRES_NONDOUBLE;
			changed_prefs.gfx_pscanlines = 0;
		}
		else if (action_event.getSource() == optDouble)
		{
			changed_prefs.gfx_vresolution = VRES_DOUBLE;
			changed_prefs.gfx_pscanlines = 0;
		}
		else if (action_event.getSource() == optScanlines)
		{
			changed_prefs.gfx_vresolution = VRES_DOUBLE;
			changed_prefs.gfx_pscanlines = 1;
		}
	}
};

static LineModeActionListener* lineModeActionListener;

void InitPanelDisplay(const struct _ConfigCategory& category)
{
	amigaScreenActionListener = new AmigaScreenActionListener();
	scalingMethodActionListener = new ScalingMethodActionListener();
	lineModeActionListener = new LineModeActionListener();
	
	auto posY = DISTANCE_BORDER;

	lblAmigaWidth = new gcn::Label("Width:");
	lblAmigaWidth->setAlignment(gcn::Graphics::LEFT);
	sldAmigaWidth = new gcn::Slider(0, AMIGAWIDTH_COUNT - 1);
	sldAmigaWidth->setSize(135, SLIDER_HEIGHT);
	sldAmigaWidth->setBaseColor(gui_baseCol);
	sldAmigaWidth->setMarkerLength(20);
	sldAmigaWidth->setStepLength(1);
	sldAmigaWidth->setId("sldWidth");
	sldAmigaWidth->addActionListener(amigaScreenActionListener);
	txtAmigaWidth = new gcn::TextField();
	txtAmigaWidth->setSize(35, TEXTFIELD_HEIGHT);
	txtAmigaWidth->setBackgroundColor(colTextboxBackground);

	lblAmigaHeight = new gcn::Label("Height:");
	lblAmigaHeight->setAlignment(gcn::Graphics::LEFT);
	sldAmigaHeight = new gcn::Slider(0, AMIGAHEIGHT_COUNT - 1);
	sldAmigaHeight->setSize(135, SLIDER_HEIGHT);
	sldAmigaHeight->setBaseColor(gui_baseCol);
	sldAmigaHeight->setMarkerLength(20);
	sldAmigaHeight->setStepLength(1);
	sldAmigaHeight->setId("sldHeight");
	sldAmigaHeight->addActionListener(amigaScreenActionListener);
	txtAmigaHeight = new gcn::TextField();
	txtAmigaHeight->setSize(35, TEXTFIELD_HEIGHT);
	txtAmigaHeight->setBackgroundColor(colTextboxBackground);

	chkAutoHeight = new gcn::CheckBox("Auto Height");
	chkAutoHeight->setId("chkAutoHeight");
	chkAutoHeight->addActionListener(amigaScreenActionListener);
	
	chkHorizontal = new gcn::CheckBox("Horizontal");
	chkHorizontal->setId("chkHorizontal");
	chkHorizontal->addActionListener(amigaScreenActionListener);
	chkVertical = new gcn::CheckBox("Vertical");
	chkVertical->setId("chkVertical");
	chkVertical->addActionListener(amigaScreenActionListener);

	chkFlickerFixer = new gcn::CheckBox("Remove interlace artifacts");
	chkFlickerFixer->setId("chkFlickerFixer");
	chkFlickerFixer->addActionListener(amigaScreenActionListener);
	
	chkAspect = new gcn::CheckBox("Correct Aspect Ratio");
	chkAspect->setId("CorrectAR");
	chkAspect->addActionListener(amigaScreenActionListener);

	chkFrameskip = new gcn::CheckBox("Frameskip");
	chkFrameskip->setId("chkFrameskip");
	chkFrameskip->addActionListener(amigaScreenActionListener);

	sldRefresh = new gcn::Slider(1, 10);
	sldRefresh->setSize(100, SLIDER_HEIGHT);
	sldRefresh->setBaseColor(gui_baseCol);
	sldRefresh->setMarkerLength(20);
	sldRefresh->setStepLength(1);
	sldRefresh->setId("sldRefresh");
	sldRefresh->addActionListener(amigaScreenActionListener);
	
	lblScreenmode = new gcn::Label("Screen mode:");
	lblScreenmode->setAlignment(gcn::Graphics::RIGHT);
	cboScreenmode = new gcn::DropDown(&fullscreen_modes_list);
	cboScreenmode->setSize(150, cboScreenmode->getHeight());
	cboScreenmode->setBaseColor(gui_baseCol);
	cboScreenmode->setBackgroundColor(colTextboxBackground);
	cboScreenmode->setId("cboScreenmode");
	cboScreenmode->addActionListener(amigaScreenActionListener);
	
	grpAmigaScreen = new gcn::Window("Amiga Screen");
	grpAmigaScreen->setPosition(DISTANCE_BORDER, DISTANCE_BORDER);

	grpAmigaScreen->add(lblAmigaWidth, DISTANCE_BORDER, posY);
	grpAmigaScreen->add(sldAmigaWidth, lblAmigaWidth->getX() + lblAmigaHeight->getWidth() + DISTANCE_NEXT_X, posY);
	grpAmigaScreen->add(txtAmigaWidth, sldAmigaWidth->getX() + sldAmigaWidth->getWidth() + DISTANCE_NEXT_X, posY);
	posY += sldAmigaWidth->getHeight() + DISTANCE_NEXT_Y;

	grpAmigaScreen->add(lblAmigaHeight, DISTANCE_BORDER, posY);
	grpAmigaScreen->add(sldAmigaHeight, lblAmigaHeight->getX() + lblAmigaHeight->getWidth() + DISTANCE_NEXT_X, posY);
	grpAmigaScreen->add(txtAmigaHeight, sldAmigaHeight->getX() + sldAmigaHeight->getWidth() + DISTANCE_NEXT_X,
		posY);
	
	posY += sldAmigaHeight->getHeight() + DISTANCE_NEXT_Y;
	grpAmigaScreen->add(chkAutoHeight, DISTANCE_BORDER, posY);
	posY += chkAutoHeight->getHeight() + DISTANCE_NEXT_Y;
	grpAmigaScreen->add(lblScreenmode, DISTANCE_BORDER, posY);
	grpAmigaScreen->add(cboScreenmode, lblScreenmode->getX() + lblScreenmode->getWidth() + 8, posY);
	posY += cboScreenmode->getHeight() + DISTANCE_NEXT_Y;

	grpAmigaScreen->setMovable(false);
	grpAmigaScreen->setSize(lblAmigaWidth->getX() + lblAmigaWidth->getWidth() + sldAmigaWidth->getWidth() + lblAmigaWidth->getWidth() + txtAmigaHeight->getWidth() + DISTANCE_BORDER, posY + DISTANCE_BORDER * 4);
	grpAmigaScreen->setTitleBarHeight(TITLEBAR_HEIGHT);
	grpAmigaScreen->setBaseColor(gui_baseCol);
	category.panel->add(grpAmigaScreen);

	grpCentering = new gcn::Window("Centering");
	grpCentering->setPosition(DISTANCE_BORDER + grpAmigaScreen->getWidth() + DISTANCE_NEXT_X, DISTANCE_BORDER);
	grpCentering->add(chkHorizontal, DISTANCE_BORDER, DISTANCE_BORDER);
	grpCentering->add(chkVertical, DISTANCE_BORDER, chkHorizontal->getY() + chkHorizontal->getHeight() + DISTANCE_NEXT_Y);
	grpCentering->setMovable(false);
	grpCentering->setSize(chkHorizontal->getX() + chkHorizontal->getWidth() + DISTANCE_BORDER * 2, TITLEBAR_HEIGHT + chkVertical->getY() + chkVertical->getHeight() + DISTANCE_NEXT_Y);
	grpCentering->setTitleBarHeight(TITLEBAR_HEIGHT);
	grpCentering->setBaseColor(gui_baseCol);
	category.panel->add(grpCentering);	
	posY = DISTANCE_BORDER + grpAmigaScreen->getHeight() + DISTANCE_NEXT_Y;

	optAuto = new gcn::RadioButton("Auto", "radioscalingmethodgroup");
	optAuto->setId("Auto");
	optAuto->addActionListener(scalingMethodActionListener);

	optNearest = new gcn::RadioButton("Nearest Neighbor (pixelated)", "radioscalingmethodgroup");
	optNearest->setId("Nearest Neighbor (pixelated)");
	optNearest->addActionListener(scalingMethodActionListener);

	optLinear = new gcn::RadioButton("Linear (smooth)", "radioscalingmethodgroup");
	optLinear->setId("Linear (smooth)");
	optLinear->addActionListener(scalingMethodActionListener);

	grpScalingMethod = new gcn::Window("Scaling method");
	grpScalingMethod->setPosition(DISTANCE_BORDER, posY);
	grpScalingMethod->add(optAuto, 10, 10);
	grpScalingMethod->add(optNearest, optAuto->getX(), optAuto->getY() + optAuto->getHeight() + DISTANCE_NEXT_Y);
	grpScalingMethod->add(optLinear, optNearest->getX(), optNearest->getY() + optNearest->getHeight() + DISTANCE_NEXT_Y);
	grpScalingMethod->setMovable(false);
	grpScalingMethod->setSize(grpAmigaScreen->getWidth(),
	                          optLinear->getY() + optLinear->getHeight() + DISTANCE_BORDER * 3);
	grpScalingMethod->setTitleBarHeight(TITLEBAR_HEIGHT);
	grpScalingMethod->setBaseColor(gui_baseCol);

	category.panel->add(grpScalingMethod);
	posY += grpScalingMethod->getHeight() + DISTANCE_NEXT_Y;

	optSingle = new gcn::RadioButton("Single", "linemodegroup");
	optSingle->setId("Single");
	optSingle->addActionListener(lineModeActionListener);

	optDouble = new gcn::RadioButton("Double", "linemodegroup");
	optDouble->setId("Double");
	optDouble->addActionListener(lineModeActionListener);

	optScanlines = new gcn::RadioButton("Scanlines", "linemodegroup");
	optScanlines->setId("Scanlines");
	optScanlines->addActionListener(lineModeActionListener);

	grpLineMode = new gcn::Window("Line mode");
	grpLineMode->setPosition(grpCentering->getX(), grpCentering->getY() + grpCentering->getHeight() + DISTANCE_NEXT_Y);
	grpLineMode->add(optSingle, 5, 10);
	grpLineMode->add(optDouble, 5, 40);
	grpLineMode->add(optScanlines, 5, 70);
	grpLineMode->setMovable(false);
	grpLineMode->setSize(grpCentering->getWidth(), TITLEBAR_HEIGHT + optScanlines->getY() + optScanlines->getHeight() + DISTANCE_NEXT_Y);
	grpLineMode->setTitleBarHeight(TITLEBAR_HEIGHT);
	grpLineMode->setBaseColor(gui_baseCol);
	
	category.panel->add(grpLineMode);
	category.panel->add(chkAspect, DISTANCE_BORDER, posY);
	posY += chkAspect->getHeight() + DISTANCE_NEXT_Y;
	
	category.panel->add(chkFlickerFixer, DISTANCE_BORDER, posY);
	posY += chkFlickerFixer->getHeight() + DISTANCE_NEXT_Y;
	
	category.panel->add(chkFrameskip, DISTANCE_BORDER, posY);
	category.panel->add(sldRefresh, chkFrameskip->getX() + chkFrameskip->getWidth() + DISTANCE_NEXT_X, posY);

	RefreshPanelDisplay();
}


void ExitPanelDisplay()
{
	delete chkFrameskip;
	delete sldRefresh;
	delete amigaScreenActionListener;
	delete lblAmigaWidth;
	delete sldAmigaWidth;
	delete txtAmigaWidth;
	delete lblAmigaHeight;
	delete sldAmigaHeight;
	delete txtAmigaHeight;
	delete chkAutoHeight;
	delete grpAmigaScreen;

	delete chkHorizontal;
	delete chkVertical;
	delete chkFlickerFixer;
	delete grpCentering;
	
	delete chkAspect;
	delete lblScreenmode;
	delete cboScreenmode;

	delete optSingle;
	delete optDouble;
	delete optScanlines;
	delete grpLineMode;
	delete lineModeActionListener;

	delete optAuto;
	delete optNearest;
	delete optLinear;
	delete grpScalingMethod;
	delete scalingMethodActionListener;
}


void RefreshPanelDisplay()
{
	chkFrameskip->setSelected(changed_prefs.gfx_framerate > 1);
	sldRefresh->setEnabled(chkFrameskip->isSelected());
	sldRefresh->setValue(changed_prefs.gfx_framerate);
	
	int i;
	char tmp[32];

	for (i = 0; i < AMIGAWIDTH_COUNT; ++i)
	{
		if (changed_prefs.gfx_monitor.gfx_size.width == amigawidth_values[i])
		{
			sldAmigaWidth->setValue(i);
			snprintf(tmp, 32, "%d", changed_prefs.gfx_monitor.gfx_size.width);
			txtAmigaWidth->setText(tmp);
			break;
		}
		// if we reached the end and didn't find anything, set the maximum value
		if (i == AMIGAWIDTH_COUNT - 1)
		{
			snprintf(tmp, 32, "%d", changed_prefs.gfx_monitor.gfx_size.width);
			txtAmigaWidth->setText(tmp);
			break;
		}
	}

	for (i = 0; i < AMIGAHEIGHT_COUNT; ++i)
	{
		if (changed_prefs.gfx_monitor.gfx_size.height == amigaheight_values[i])
		{
			sldAmigaHeight->setValue(i);
			snprintf(tmp, 32, "%d", changed_prefs.gfx_monitor.gfx_size.height);
			txtAmigaHeight->setText(tmp);
			break;
		}
		// if we reached the end and didn't find anything, set the maximum value
		if (i == AMIGAHEIGHT_COUNT - 1)
		{
			snprintf(tmp, 32, "%d", changed_prefs.gfx_monitor.gfx_size.height);
			txtAmigaHeight->setText(tmp);
			break;
		}
	}
	chkAutoHeight->setSelected(changed_prefs.gfx_auto_height);

	lblAmigaHeight->setEnabled(!chkAutoHeight->isSelected());
	sldAmigaHeight->setEnabled(!chkAutoHeight->isSelected());
	txtAmigaHeight->setEnabled(!chkAutoHeight->isSelected());
	
	chkHorizontal->setSelected(changed_prefs.gfx_xcenter == 2);
	chkVertical->setSelected(changed_prefs.gfx_ycenter == 2);

	chkFlickerFixer->setSelected(changed_prefs.gfx_scandoubler);
	chkAspect->setSelected(changed_prefs.gfx_correct_aspect);

	if (changed_prefs.gfx_apmode[0].gfx_fullscreen == GFX_WINDOW)
		cboScreenmode->setSelected(0);
	else if (changed_prefs.gfx_apmode[0].gfx_fullscreen == GFX_FULLSCREEN)
		cboScreenmode->setSelected(1);
	else if (changed_prefs.gfx_apmode[0].gfx_fullscreen == GFX_FULLWINDOW)
		cboScreenmode->setSelected(2);

#ifdef USE_DISPMANX
	lblScreenmode->setEnabled(false);
	cboScreenmode->setEnabled(false);
#endif
	
	if (changed_prefs.scaling_method == -1)
		optAuto->setSelected(true);
	else if (changed_prefs.scaling_method == 0)
		optNearest->setSelected(true);
	else if (changed_prefs.scaling_method == 1)
		optLinear->setSelected(true);

#ifdef USE_DISPMANX
	grpScalingMethod->setEnabled(false);
	optAuto->setEnabled(false);
	optNearest->setEnabled(false);
	optLinear->setEnabled(false);
#endif
	
	if (changed_prefs.gfx_vresolution == VRES_NONDOUBLE && changed_prefs.gfx_pscanlines == 0)
		optSingle->setSelected(true);
	else if (changed_prefs.gfx_vresolution == VRES_DOUBLE && changed_prefs.gfx_pscanlines == 0)
		optDouble->setSelected(true);
	else if (changed_prefs.gfx_vresolution == VRES_DOUBLE && changed_prefs.gfx_pscanlines == 1)
		optScanlines->setSelected(true);
}

bool HelpPanelDisplay(std::vector<std::string>& helptext)
{
	helptext.clear();
	helptext.emplace_back("Select the required width and height of the Amiga screen. If you select \"NTSC\"");
	helptext.emplace_back("in Chipset, a value greater than 240 for \"Height\" makes no sense. When the game,");
	helptext.emplace_back("Demo or Workbench uses HiRes mode and you selected a value for \"Width\" lower than 640,");
	helptext.emplace_back("you will only see half of the pixels.");
	helptext.emplace_back(" ");
	helptext.emplace_back("You can use the Horizontal/Vertical centering options to center the image automatically");
	helptext.emplace_back(" ");
	helptext.emplace_back("The Aspect Ratio option allows you to choose if you want the correct Aspect Ratio");
	helptext.emplace_back("to be kept always (default), or have the image stretched to fill the screen instead.");
	helptext.emplace_back(" ");
	helptext.emplace_back("The Full Screen option allows you to switch from Windowed to Full screen and back.");
	helptext.emplace_back("This only works when running under X11 with SDL2, as KMSDRM is always Fullscreen.");
	helptext.emplace_back(" ");
	helptext.emplace_back("Select the scaling method for the Amiga screen. The default option \"Auto\", ");
	helptext.emplace_back("will try to find the best looking scaling method depending on your monitor's resolution. ");
	helptext.emplace_back("\"Nearest Neighbor\" will give you a more pixelated and crisp image, but it may come with ");
	helptext.emplace_back("some distortion if your resolution is not an exact multiple. ");
	helptext.emplace_back("\"Linear\" will give you a smoother scaling but some people might find it a bit blurry.");
	helptext.emplace_back(" ");
	helptext.emplace_back(
		"Activate line doubling to remove flicker in interlace modes, or Scanlines for the CRT effect.");
	helptext.emplace_back(" ");
	helptext.emplace_back("When you activate \"Frameskip\", only every second frame is drawn.");
	helptext.emplace_back("This will improve performance and some more games are playable.");
	return true;
}
