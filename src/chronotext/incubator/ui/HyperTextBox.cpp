/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/incubator/ui/HyperTextBox.h"

using namespace std;
using namespace ci;

namespace chronotext
{
    HyperTextBox::HyperTextBox()
    :
    TextBox()
    {}
    
    HyperTextBox::HyperTextBox(shared_ptr<Style> style)
    :
    TextBox(style),
    style(style),
    updateLocationRequest(false),
    delegate(NULL),
    selectedLinkIndex(-1)
    {}
    
    void HyperTextBox::setLocation(float x, float y)
    {
        if ((x != this->x) || (y != this->y))
        {
            updateLocationRequest = true;
            Shape::setLocation(x, y);
        }
    }
    
    void HyperTextBox::setText(const wstring &text)
    {
        pair<vector<ExtractedLink>, wstring> extracted = LinkExtractor::extract(text);
        
        links.clear();
        links = extracted.first;
        
        TextBox::setText(extracted.second);
    }
    
    void HyperTextBox::selectLink(int linkIndex)
    {
        if (linkIndex < links.size())
        {
            selectedLinkIndex = linkIndex;
        }
        else
        {
            throw runtime_error("SELECTED-LINK INDEX OUT-OF-RANGE");
        }
    }
    
    void HyperTextBox::deselectLink()
    {
        selectedLinkIndex = -1;
    }
    
    void HyperTextBox::layout()
    {
        bool updateRequired = (updateWrapRequest || updateWidthRequest || updateLocationRequest);
        
        TextBox::layout();
        
        if (updateRequired)
        {
            linkSpans.clear();
            
            for (vector<ExtractedLink>::const_iterator it = links.begin(); it != links.end(); ++it)
            {
                linkSpans.push_back(getLinkSpan(*it));
            }
            
            // ---
            
            touchableLinks.clear();
            
            float offsetTop = font->getSize() * style->linkPaddingFactor;
            float offsetBottom = font->getHeight() + offsetTop;
            
            vector<LinkSpan>::const_iterator it1 = linkSpans.begin();
            int index = 0;
            for (; it1 != linkSpans.end(); ++it1, index++)
            {
                for (vector< pair<Vec2f, Vec2f> >::const_iterator it2 = it1->positions.begin(); it2 != it1->positions.end(); ++it2)
                {
                    Vec2f p1 = it2->first;
                    Vec2f p2 = it2->second;
                    Rectf bounds = Rectf(p1.x, p1.y - offsetTop, p2.x, p2.y + offsetBottom);
                    
                    touchableLinks.push_back(new TouchableLink(this, index, bounds, Touchable::TYPE_CLICKABLE, style->linkHitExtra));
                }
            }
            
            updateLocationRequest = false;
        }
    }
    
    void HyperTextBox::draw()
    {
        layout();
        fill.draw(getBounds());
        
        /*
         * TODO: BEGIN CLIP
         */
        
        drawSelectedLinkBackground();
        drawText();
        drawSelectedLinkText();
        drawLinkUnderlines();
        
        /*
         * TODO: END CLIP
         */
    }
    
    vector<Touchable*> HyperTextBox::getTouchables()
    {
        vector<Touchable*> touchables;
        
        for (boost::ptr_vector<Touchable>::iterator it = touchableLinks.begin(); it != touchableLinks.end(); ++it)
        {
            touchables.push_back(&*it);
        }
        
        return touchables;
    }
    
    void HyperTextBox::touchStateChanged(Touchable *touchable, int nextState, int prevState)
    {
        if (nextState == Touchable::STATE_NORMAL)
        {
            selectedLinkIndex = -1;
        }
        else if (nextState == Touchable::STATE_PRESSED)
        {
            TouchableLink *touchableLink = static_cast<TouchableLink*>(touchable);
            selectedLinkIndex = touchableLink->linkIndex;
        }
    }
    
    void HyperTextBox::touchActionPerformed(Touchable *touchable, int action)
    {
        if (delegate)
        {
            TouchableLink *touchableLink = static_cast<TouchableLink*>(touchable);
            delegate->linkClicked(links[touchableLink->linkIndex].url);
        }
    }
    
    LinkSpan HyperTextBox::getLinkSpan(const ExtractedLink &link)
    {
        int startIndex = link.offset;
        int endIndex = link.offset + link.length;
        
        int startLine = wrapper.getLine(startIndex);
        int endLine = wrapper.getLine(endIndex);
        
        int n = endLine - startLine + 1;
        
        LinkSpan linkSpan(startIndex, endIndex, startLine, endLine);
        
        for (int i = 0; i < n; i++)
        {
            Vec2f p1;
            Vec2f p2;
            
            if (i == 0)
            {
                p1 = getLocationAt(startLine, startIndex);
                
                if (n == 1)
                {
                    p2 = getLocationAt(endLine, endIndex);
                }
                else
                {
                    p2 = getLocationAt(startLine, numeric_limits<int>::max());
                }
            }
            else if (i == n - 1)
            {
                p1 = getLocationAt(endLine, numeric_limits<int>::min());
                p2 = getLocationAt(endLine, endIndex);
            }
            else
            {
                p1 = getLocationAt(startLine + i, numeric_limits<int>::min());
                p2 = getLocationAt(startLine + i, numeric_limits<int>::max());
            }
            
            linkSpan.positions.emplace_back(p1, p2);
        }
        
        return linkSpan;
    }
    
    void HyperTextBox::drawText()
    {
        font->setColor(textColor);
        
        float innerWidth = width - paddingLeft - paddingRight;
        float innerHeight = height - paddingTop - paddingBottom;
        
        float limitTop = y + paddingTop + font->getAscent();
        float limitBottom = limitTop + innerHeight;
        float yy = limitTop + getOffsetY();
        
        font->beginSequence(NULL);
        
        for (int i = 0; i < wrapper.size && yy < limitBottom; i++)
        {
            if (yy + lineHeight > limitTop)
            {
                int start = wrapper.offsets[i];
                int end = start + wrapper.lengths[i];
                
                float limitLeft = x + paddingLeft;
                float limitRight = limitLeft + innerWidth;
                float xx = limitLeft + getOffsetX(start, end);
                
                bool skip = false;
                
                if (selectedLinkIndex != -1)
                {
                    const LinkSpan &linkSpan = linkSpans[selectedLinkIndex];
                    
                    if (i > linkSpan.startLine && i < linkSpan.endLine)
                    {
                        skip = true;
                    }
                    else
                    {
                        if (i == linkSpan.startLine)
                        {
                            drawTextSpan(xx, yy, start, linkSpan.startIndex, limitLeft, limitRight);
                            skip = true;
                        }
                        
                        if (i == linkSpan.endLine)
                        {
                            xx += font->getSubStringAdvance(text, start, linkSpan.endIndex);
                            drawTextSpan(xx, yy, linkSpan.endIndex, end, limitLeft, limitRight);
                            skip = true;
                        }
                    }
                }
                
                if (!skip)
                {
                    drawTextSpan(xx, yy, start, end, limitLeft, limitRight);
                }
            }
            
            yy += lineHeight;
        }
        
        font->endSequence();
    }
    
    void HyperTextBox::drawSelectedLinkText()
    {
        if (selectedLinkIndex != -1)
        {
            const LinkSpan &linkSpan = linkSpans[selectedLinkIndex];
            
            font->setColor(style->selectedLinkTextColor);
            
            float innerWidth = width - paddingLeft - paddingRight;
            float innerHeight = height - paddingTop - paddingBottom;
            
            float limitTop = y + paddingTop + font->getAscent();
            float limitBottom = limitTop + innerHeight;
            float yy = limitTop + getOffsetY() + linkSpan.startLine * lineHeight;
            
            if ((yy < limitBottom) && (yy + lineHeight > limitTop))
            {
                font->beginSequence(NULL);
                
                for (int i = linkSpan.startLine; i <= linkSpan.endLine; i++)
                {
                    int start = wrapper.offsets[i];
                    int end = start + wrapper.lengths[i];
                    
                    float limitLeft = x + paddingLeft;
                    float limitRight = limitLeft + innerWidth;
                    float xx = limitLeft + getOffsetX(start, end);
                    
                    if (i == linkSpan.startLine)
                    {
                        xx += font->getSubStringAdvance(text, start, linkSpan.startIndex);
                        start = linkSpan.startIndex;
                    }
                    
                    if (i == linkSpan.endLine)
                    {
                        end = linkSpan.endIndex;
                    }
                    
                    drawTextSpan(xx, yy, start, end, limitLeft, limitRight);
                    yy += lineHeight;
                }
                
                font->endSequence();
            }
        }
    }
    
    void HyperTextBox::drawSelectedLinkBackground()
    {
        if (selectedLinkIndex != -1)
        {
            if (style->selectedLinkBackgroundColor.a > 0)
            {
                gl::color(style->selectedLinkBackgroundColor);
                
                float offsetTop = font->getSize() * style->linkPaddingFactor;
                float offsetBottom = font->getHeight() + offsetTop;
                
                vector<LinkSpan>::const_iterator it1 = linkSpans.begin();
                int index = 0;
                for (; it1 != linkSpans.end(); ++it1, index++)
                {
                    if (index == selectedLinkIndex)
                    {
                        for (vector< pair<Vec2f, Vec2f> >::const_iterator it2 = it1->positions.begin(); it2 != it1->positions.end(); ++it2)
                        {
                            Vec2f p1 = it2->first;
                            Vec2f p2 = it2->second;
                            
                            gl::drawSolidRect(Rectf(p1.x, p1.y - offsetTop, p2.x, p2.y + offsetBottom));
                        }
                    }
                }
            }
        }
    }
    
    void HyperTextBox::drawLinkUnderlines()
    {
        gl::color(textColor);
        
        float offsetBottom = font->getAscent() + font->getSize() * style->linkUnderlineFactor;
        
        vector<LinkSpan>::const_iterator it1 = linkSpans.begin();
        int index = 0;
        for (; it1 != linkSpans.end(); ++it1, index++)
        {
            if (index != selectedLinkIndex)
            {
                for (vector< pair<Vec2f, Vec2f> >::const_iterator it2 = it1->positions.begin(); it2 != it1->positions.end(); ++it2)
                {
                    Vec2f p1 = it2->first;
                    Vec2f p2 = it2->second;
                    
                    p1.y = math<float>::floor(p1.y + offsetBottom);
                    p2.y = math<float>::floor(p2.y + offsetBottom);
                    
                    gl::drawLine(p1, p2);
                }
            }
        }
    }
}
