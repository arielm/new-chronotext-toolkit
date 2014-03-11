/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * GENERATING A FEW FONTS (SAVED IN THE DOCUMENTS FOLDER)
 *
 *
 * REQUIREMENTS:
 * - OSX (CAN BE TRIVIALLY ADAPTED TO WINDOWS)
 * - FREETYPE BLOCK: https://github.com/arielm/Freetype
 *
 *
 * FONTS USED:
 * - Georgia - AVAILABLE ON OSX AND WINDOWS
 * - Roboto - AVAILABLE ON ANDROID AND AS GOOGLE-FONT: http://www.google.com/fonts/specimen/Roboto
 * - FrankRuehl - AVAILABLE ON WINDOWS (WITH HEBREW SUPPORT): http://www.microsoft.com/typography/fonts/font.aspx?FMID=1886
 */

#include "cinder/app/AppNative.h"

#include "chronotext/font/xf/FontManager.h"
#include "chronotext/font/xf/TextHelper.h"
#include "chronotext/tools/font/XFontCreator.h"
#include "chronotext/tools/font/Characters.h"
#include "chronotext/utils/Utils.h"

using namespace std;
using namespace ci;
using namespace app;
using namespace chr;
using namespace xf;

const wstring ISO_8859_15_EXTENDED = ISO_8859_15 + L"\u2013\u2014"; // ADDING "N" AND "M" DASHES
const wstring HEBREW_BIBLICAL = L":,;.-\u05d0\u05d1\u05d2\u05d3\u05d4\u05d5\u05d6\u05d7\u05d8\u05d9\u05da\u05db\u05dc\u05dd\u05de\u05df\u05e0\u05e1\u05e2\u05e3\u05e4\u05e5\u05e6\u05e7\u05e8\u05e9\u05ea";

class Application : public AppNative
{
    shared_ptr<FreetypeHelper> ftHelper;
    FontManager fontManager;
    
    vector<shared_ptr<XFont>> fonts;
    
public:
    void setup();
    void prepareSettings(Settings *settings);
    
    void draw();
    
    void createFontSafely(const FontDescriptor &descriptor, float size, const wstring &characters, const XParams &params);
    void loadFontSafely(const string &fileName, float direction = +1);
    void drawFonts(float size, const ColorA &color);
};

void Application::setup()
{
    ftHelper = make_shared<FreetypeHelper>();
    
    /*
     * - PROVIDING ENOUGH MARGIN AND PADDING, TO ALLOW FOR MIPMAPPING WITHOUT BLEEDING EDGES
     * - DEMONSTRATES HOW TO LOAD FONTS LIKE Georgia ON OSX
     */
    createFontSafely(FontDescriptor("/Library/Fonts/Georgia.ttf"), 64, ISO_8859_15_EXTENDED, XParams(3, 2));
    createFontSafely(FontDescriptor("/Library/Fonts/AmericanTypewriter.ttc"), 64, ISO_8859_15_EXTENDED, XParams(3, 2));
    
    /*
     * - PROVIDING ENOUGH MARGIN AND PADDING, TO ALLOW FOR MIPMAPPING WITHOUT BLEEDING EDGES
     * - DEMONSTRATES HOW TO LOAD A CUSTOM FONT FROM THE RESOURCE-BUNDLE
     */
    createFontSafely(FontDescriptor(getResourcePath("Roboto-Regular.ttf")), 64, ISO_8859_15_EXTENDED, XParams(3, 2));

    /*
     * - PROVIDING ENOUGH MARGIN AND PADDING, TO ALLOW FOR MIPMAPPING WITHOUT BLEEDING EDGES
     * - DEMONSTRATES HOW TO LOAD A CUSTOM FONT FROM THE DOCUMENTS FOLDER
     */
    createFontSafely(FontDescriptor(getDocumentsDirectory() / "frank.ttf"), 64, HEBREW_BIBLICAL, XParams(3, 2));

    // ---
    
    /*
     * LOADING OUR GENERATED FONTS
     */
    
    loadFontSafely("Georgia_Regular_64.fnt");
    loadFontSafely("American Typewriter_Regular_64.fnt");
    loadFontSafely("Roboto_Regular_64.fnt");
    loadFontSafely("FrankRuehl_Regular_64.fnt", -1);
    
    // ---
    
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
}

void Application::prepareSettings(AppBasic::Settings *settings)
{
    Area area = settings->getDisplay()->getBounds();
    settings->setWindowSize(area.getWidth(), area.getWidth() * 0.25f);
    
    settings->enableHighDensityDisplay();
}

void Application::draw()
{
    gl::clear(Color::gray(0.5f), false);
    drawFonts(32, ColorA::white());
}

void Application::createFontSafely(const FontDescriptor &descriptor, float size, const wstring &characters, const XParams &params)
{
    try
    {
        XFontCreator(ftHelper, descriptor, size, characters, params).writeToFolder(getDocumentsDirectory());
    }
    catch (exception &e)
    {
        LOGI << e.what() << endl;
    }
}

void Application::loadFontSafely(const string &fileName, float direction)
{
    try
    {
        auto font = fontManager.getCachedFont(InputSource::getFileInDocuments(fileName), XFont::Properties2d());
        font->setDirection(direction);
        fonts.push_back(font);
    }
    catch (exception &e)
    {
        LOGI << e.what() << endl;
    }
}

void Application::drawFonts(float size, const ColorA &color)
{
    int current = 0;
    int count = fonts.size();
    
    for (auto &font : fonts)
    {
        current++;
        float y = getWindowHeight() * current / float(count + 1);
        
        font->setColor(color);
        font->setSize(size);
        
        TextHelper::drawAlignedText(*font, font->getCharacters(), getWindowWidth() * 0.5f, y);
    }
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
