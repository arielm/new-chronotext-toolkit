#include "chronotext/incubator/ui/Shape.h"
#include "chronotext/incubator/ui/Container.h"

using namespace std;
using namespace ci;

namespace chronotext
{
    Shape::Shape(ShapeStyleRef style)
    :
    tag(0),
    container(NULL)
    {
        setLocation(0, 0);
        setPadding(style->paddingLeft, style->paddingTop, style->paddingRight, style->paddingBottom);
        setMargin(style->marginLeft, style->marginTop, style->marginRight, style->marginBottom);
        setVisible(style->visible);

        if (style->width > 0)
        {
            setWidth(style->width);
        }
        else
        {
            setAutoWidth(style->autoWidth);
            width = 0;
        }
        
        if (style->height > 0)
        {
            setHeight(style->height);
        }
        else
        {
            setAutoHeight(style->autoHeight);
            height = 0;
        }
    }
    
    void Shape::setLocation(float x, float y)
    {
        this->x = x;
        this->y = y;
    }
    
    void Shape::setWidth(float width)
    {
        this->width = width;
        autoWidth = false;
        requestContainerLayout();
    }
    
    void Shape::setHeight(float height)
    {
        this->height = height;
        autoHeight = false;
        requestContainerLayout();
    }
    
    void Shape::setAutoWidth(bool autoWidth)
    {
        this->autoWidth = autoWidth;
        requestContainerLayout();
    }
    
    void Shape::setAutoHeight(bool autoHeight)
    {
        this->autoHeight = autoHeight;
        requestContainerLayout();
    }
    
    void Shape::setBounds(const Rectf &bounds)
    {
        x = bounds.x1;
        y = bounds.y1;
        
        setWidth(bounds.getWidth());
        setHeight(bounds.getHeight());
    }
    
    void Shape::setPadding(float left, float top, float right, float bottom)
    {
        paddingLeft = left;
        paddingTop = top;
        paddingRight = right;
        paddingBottom = bottom;
    }
    
    void Shape::setMargin(float left, float top, float right, float bottom)
    {
        marginLeft = left;
        marginTop = top;
        marginRight = right;
        marginBottom = bottom;
        
        requestContainerLayout();
    }
    
    void Shape::setVisible(bool visible)
    {
        this->visible = visible;
        requestContainerLayout();
    }

    ci::Vec2f Shape::getLocation()
    {
        return Vec2f(x, y);
    }
    
    float Shape::getWidth()
    {
        return width;
    }
    
    float Shape::getHeight()
    {
        return height;
    }
    
    ci::Rectf Shape::getBounds()
    {
        return Rectf(x, y, x + getWidth(), y + getHeight());
    }
    
    vector<Touchable*> Shape::getTouchables()
    {
        return vector<Touchable*>();
    }
    
    void Shape::setContainer(Container *container)
    {
        this->container = container;
    }
    
    void Shape::requestContainerLayout()
    {
        if (container)
        {
            container->requestLayout();
        }
    }
}
