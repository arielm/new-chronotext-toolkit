#pragma once

#include "chronotext/incubator/ui/Shape.h"
#include "chronotext/incubator/ui/ButtonStyle.h"
#include "chronotext/incubator/ui/Icon.h"
#include "chronotext/incubator/ui/Touchable.h"

#include <map>

namespace chronotext
{
    typedef boost::shared_ptr<class Button> ButtonRef;

    class ButtonDelegate
    {
    public:
        virtual void buttonClicked(int tag) = 0;
        virtual void buttonToggled(int tag) = 0;
    };

    class Button : public Shape, public Touchable
    {
        float getContentWidth();
        float getContentHeight();

    public:
        ButtonStyleRef style;
        
        std::wstring text;
        IconRef icon;
        
        ButtonDelegate *delegate;
        
        Button(ButtonStyleRef style);
        
        float getWidth();
        float getHeight();

        void draw();

        std::vector<Touchable*> getTouchables();
        void touchStateChanged(Touchable *touchable, int state);
        void touchActionPerformed(Touchable *touchable, int action);

        bool hitTest(const ci::Vec2f &point, float *distance);
    };
}

namespace chr = chronotext;
