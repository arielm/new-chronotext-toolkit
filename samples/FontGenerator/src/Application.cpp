/*
 * GENERATING A FEW FONTS (SAVED IN THE DOCUMENTS FOLDER)
 *
 *
 * REQUIREMENTS:
 * - OSX
 * - https://github.com/arielm/Freetype
 *
 *
 * CREDITS:
 * - FrankRuehl FONT, VERSION 1.00: http://www.microsoft.com/typography/fonts/font.aspx?FMID=1886
 */

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Utilities.h"

#include "chronotext/font/XFont.h"
#include "chronotext/text/FontHelper.h"
#include "chronotext/tools/font/XFontCreator.h"
#include "chronotext/tools/font/Characters.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace chr;

static std::wstring HEBREW_BIBLICAL = L":,;.-\u05d0\u05d1\u05d2\u05d3\u05d4\u05d5\u05d6\u05d7\u05d8\u05d9\u05da\u05db\u05dc\u05dd\u05de\u05df\u05e0\u05e1\u05e2\u05e3\u05e4\u05e5\u05e6\u05e7\u05e8\u05e9\u05ea";

class Application : public AppNative
{
    XFont *font1;
    XFont *font2;
    XFont *font3;
    
public:
    void setup();
    void shutdown();
    void prepareSettings(Settings *settings);
    
    void draw();
};

void Application::setup()
{
    FreetypeHelper ftHelper;
    
    /*
     * - NO NEED FOR MARGIN AND PADDING BECAUSE THE FONT IS ONLY INTENDED TO BE RENDERED AT ITS NATIVE-FONT-SIZE
     * - DEMONSTRATES HOW TO LOAD FONTS LIKE Helevetica ON OSX
     */
    XFontCreator(ftHelper.getLib(), FontDescriptor("/System/Library/Fonts/Helvetica.dfont", 0), 16, ASCII, XParams(0, 0)).writeToFolder(getDocumentsDirectory()); // FACE-INDEX 0 CORRESPONDS TO "Helvetica Regular"

    /*
     * - PROVIDING ENOUGH MARGIN AND PADDING, TO ALLOW FOR MIPMAPPING WITHOUT BLEEDING EDGES
     * - DEMONSTRATES HOW TO LOAD FONTS LIKE Georgia ON OSX
     */
    XFontCreator(ftHelper.getLib(), FontDescriptor("/Library/Fonts/Georgia.ttf"), 64, ISO_8859_15, XParams(3, 2)).writeToFolder(getDocumentsDirectory());
    
    /*
     * - PROVIDING ENOUGH MARGIN AND PADDING, TO ALLOW FOR MIPMAPPING WITHOUT BLEEDING EDGES
     * - DEMONSTRATES HOW TO LOAD A CUSTOM FONT
     */
    XFontCreator(ftHelper.getLib(), FontDescriptor(getResourcePath("frank.ttf")), 64, HEBREW_BIBLICAL, XParams(3, 2)).writeToFolder(getDocumentsDirectory());
    
    // ---
    
    /*
     * LOADING OUR GENERATED FONTS
     */
    
    font1 = new XFont(InputSource::getFileInDocuments("Helvetica_Regular_16.fnt"));
    font2 = new XFont(InputSource::getFileInDocuments("Georgia_Regular_64.fnt"), true); // USING MIPMAP
    font3 = new XFont(InputSource::getFileInDocuments("FrankRuehl_Regular_64.fnt"), true); // USING MIPMAP
    
    // ---
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
}

void Application::shutdown()
{
    delete font1;
    delete font2;
    delete font3;
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
    
    font1->setSize(16); // THIS FONT IS NOT INTENDED TO BE RENDERED AT ANOTHER SIZE
    FontHelper::drawText(font1, NULL, font1->getCharacters(), 10, getWindowHeight() / 4.0f, true); // USING SNAP
    
    font2->setSize(32); // ANY SIZE CAN BE USED
    FontHelper::drawText(font2, NULL, font2->getCharacters(), 10, getWindowHeight() * 2 / 4.0f);
    
    font3->setSize(64); // ANY SIZE CAN BE USED
    font3->setDirection(-1);
    FontHelper::drawText(font3, NULL, font3->getCharacters(), getWindowWidth() - 10, getWindowHeight() * 3 / 4.0f);
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
