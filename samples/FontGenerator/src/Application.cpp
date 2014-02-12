/*
 * GENERATING A FEW FONTS (SAVED IN THE DOCUMENTS FOLDER)
 *
 *
 * REQUIREMENTS:
 * - OSX
 * - FREETYPE BLOCK: https://github.com/arielm/Freetype
 *
 *
 * FONTS USED:
 * - Georgia - AVAILABLE ON OSX AND WINDOWS
 * - Roboto - AVAILABLE ON ANDROID AND AS GOOGLE-FONT: http://www.google.com/fonts/specimen/Roboto
 * - FrankRuehl - AVAILABLE ON WINDOWS (WITH HEBREW SUPPORT): http://www.microsoft.com/typography/fonts/font.aspx?FMID=1886
 */

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "chronotext/font/FontManager.h"
#include "chronotext/text/TextHelper.h"
#include "chronotext/tools/font/XFontCreator.h"
#include "chronotext/tools/font/Characters.h"
#include "chronotext/utils/Utils.h"

using namespace std;
using namespace ci;
using namespace app;
using namespace chr;

const std::wstring HEBREW_BIBLICAL = L":,;.-\u05d0\u05d1\u05d2\u05d3\u05d4\u05d5\u05d6\u05d7\u05d8\u05d9\u05da\u05db\u05dc\u05dd\u05de\u05df\u05e0\u05e1\u05e2\u05e3\u05e4\u05e5\u05e6\u05e7\u05e8\u05e9\u05ea";

class Application : public AppNative
{
    shared_ptr<FreetypeHelper> ftHelper;
    FontManager fontManager;
    
    shared_ptr<XFont> font1;
    shared_ptr<XFont> font2;
    shared_ptr<XFont> font3;
    
public:
    void setup();
    void prepareSettings(Settings *settings);
    
    void draw();
    
    void createFontSafely(const FontDescriptor &descriptor, float size, const wstring &characters, const XParams &params);
    shared_ptr<XFont> loadFontSafely(const string &fileName);
    void drawFontSafely(shared_ptr<XFont> &font, float size, float x, float y, float direction = +1);
};

void Application::setup()
{
    ftHelper = make_shared<FreetypeHelper>();
    
    /*
     * - PROVIDING ENOUGH MARGIN AND PADDING, TO ALLOW FOR MIPMAPPING WITHOUT BLEEDING EDGES
     * - DEMONSTRATES HOW TO LOAD FONTS LIKE Georgia ON OSX
     */
    createFontSafely(FontDescriptor("/Library/Fonts/Georgia.ttf"), 64, ISO_8859_15, XParams(3, 2));
    
    /*
     * - PROVIDING ENOUGH MARGIN AND PADDING, TO ALLOW FOR MIPMAPPING WITHOUT BLEEDING EDGES
     * - DEMONSTRATES HOW TO LOAD A CUSTOM FONT FROM THE RESOURCE-BUNDLE
     */
    createFontSafely(FontDescriptor(getResourcePath("Roboto-Regular.ttf")), 64, ISO_8859_15, XParams(3, 2));

    /*
     * - PROVIDING ENOUGH MARGIN AND PADDING, TO ALLOW FOR MIPMAPPING WITHOUT BLEEDING EDGES
     * - DEMONSTRATES HOW TO LOAD A CUSTOM FONT FROM THE DOCUMENTS FOLDER
     */
    createFontSafely(FontDescriptor(getDocumentsDirectory() / "frank.ttf"), 64, HEBREW_BIBLICAL, XParams(3, 2));

    // ---
    
    /*
     * LOADING OUR GENERATED FONTS
     */
    
    font1 = loadFontSafely("Georgia_Regular_64.fnt");
    font2 = loadFontSafely("Roboto_Regular_64.fnt");
    font3 = loadFontSafely("FrankRuehl_Regular_64.fnt");
    
    // ---
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
}

void Application::prepareSettings(AppBasic::Settings *settings)
{
    Area area = settings->getDisplay()->getBounds();
    settings->setWindowSize(area.getWidth(), area.getWidth() * 0.25f);
}

void Application::draw()
{
    gl::clear(Color(0.5f, 0.5f, 0.5f), false);
    glColor4f(1, 1, 1, 1);
    
    drawFontSafely(font1, 32, 10, getWindowHeight() * 1 / 4.0f);
    drawFontSafely(font2, 32, 10, getWindowHeight() * 2 / 4.0f);
    drawFontSafely(font3, 64, getWindowWidth() - 10, getWindowHeight() * 3 / 4.0f, -1);
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

shared_ptr<XFont> Application::loadFontSafely(const string &fileName)
{
    try
    {
        return fontManager.getCachedFont(InputSource::getFileInDocuments(fileName), XFont::Properties::Default2D());
    }
    catch (exception &e)
    {
        LOGI << e.what() << endl;
    }
    
    return NULL;
}

void Application::drawFontSafely(shared_ptr<XFont> &font, float size, float x, float y, float direction)
{
    if (font)
    {
        font->setSize(size);
        font->setDirection(direction);
        TextHelper::drawText(*font, font->getCharacters(), x, y);
    }
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
