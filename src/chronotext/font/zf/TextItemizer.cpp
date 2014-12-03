/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * USING ICU 52.1 FOR SCRIPT-DETECTION AND FOR BIDI:
 * http://www.icu-project.org/apiref/icu4c/ubidi_8h.html
 */

/*
 * INSPIRED BY THE MAPNIK PROJECT:
 * https://github.com/mapnik/mapnik/blob/64d5153aeaeb1c9e736bfead297dfea39b066d2c/src/text/itemizer.cpp
 */

#include "chronotext/font/zf/TextItemizer.h"

#include "scrptrun.h"

using namespace std;

namespace chronotext
{
    namespace zf
    {
        hb_script_t TextItemizer::icuScriptToHB(UScriptCode script)
        {
            if (script == USCRIPT_INVALID_CODE)
            {
                return HB_SCRIPT_INVALID;
            }
            
            return hb_script_from_string(uscript_getShortName(script), -1);
        }
        
        hb_direction_t TextItemizer::icuDirectionToHB(UBiDiDirection direction)
        {
            return (direction == UBIDI_RTL) ? HB_DIRECTION_RTL : HB_DIRECTION_LTR;
        }
        
        TextItemizer::TextItemizer(std::shared_ptr<LangHelper> langHelper)
        :
        langHelper(langHelper)
        {}
        
        void TextItemizer::processLine(TextLine &line) const
        {
            if (line.scriptAndLanguageItems.empty())
            {
                itemizeScriptAndLanguage(line);
            }
            if (line.directionItems.empty())
            {
                itemizeDirection(line);
            }
            if (line.tagItems.empty())
            {
                line.tagItems.emplace_back(0, line.text.length(), 0);
            }
            
            mergeItems(line);
            
            if (!line.runs.empty())
            {
                if (line.langHint.empty())
                {
                    line.langHint = line.runs.front().language;
                }
                
                if (line.overallDirection == HB_DIRECTION_INVALID)
                {
                    line.overallDirection = line.runs.front().direction;
                }
            }
        }
        
        void TextItemizer::itemizeScriptAndLanguage(TextLine &line) const
        {
            ScriptRun scriptRun(line.text.getBuffer(), line.text.length());
            
            while (scriptRun.next())
            {
                auto start = scriptRun.getScriptStart();
                auto end = scriptRun.getScriptEnd();
                auto code = scriptRun.getScriptCode();
                
                auto script = icuScriptToHB(code);
                auto language = langHelper->detectLanguage(script, line.langHint);
                
                line.scriptAndLanguageItems.emplace_back(start, end, make_pair(script, language));
            }
        }
        
        void TextItemizer::itemizeDirection(TextLine &line) const
        {
            /*
             * IF overallDirection IS UNDEFINED: THE PARAGRAPH-LEVEL WILL BE DETERMINED FROM THE TEXT
             *
             * SEE: http://www.icu-project.org/apiref/icu4c/ubidi_8h.html#abdfe9e113a19dd8521d3b7ac8220fe11
             */
            UBiDiLevel paraLevel = (line.overallDirection == HB_DIRECTION_INVALID) ? UBIDI_DEFAULT_LTR : ((line.overallDirection == HB_DIRECTION_RTL) ? 1 : 0);
            
            auto length = line.text.length();
            UErrorCode error = U_ZERO_ERROR;
            UBiDi *bidi = ubidi_openSized(length, 0, &error);
            
            ubidi_setPara(bidi, line.text.getBuffer(), length, paraLevel, 0, &error);
            auto direction = ubidi_getDirection(bidi);
            
            if (direction != UBIDI_MIXED)
            {
                line.directionItems.emplace_back(0, length, icuDirectionToHB(direction));
            }
            else
            {
                auto count = ubidi_countRuns(bidi, &error);
                
                for (int i = 0; i < count; ++i)
                {
                    int32_t start, length;
                    direction = ubidi_getVisualRun(bidi, i, &start, &length);
                    line.directionItems.emplace_back(start, start + length, icuDirectionToHB(direction));
                }
            }
            
            ubidi_close(bidi);
        }
        
        void TextItemizer::mergeItems(TextLine &line) const
        {
            for (auto &directionItem : line.directionItems)
            {
                auto position = directionItem.start;
                auto end = directionItem.end;
                auto rtlInsertionPoint = line.runs.end();
                
                auto scriptAndLanguageIterator = findItem(line.scriptAndLanguageItems, position);
                auto tagIterator = findItem(line.tagItems, position);
                
                while (position < end)
                {
                    TextRun run;
                    run.start = position;
                    run.end = std::min(scriptAndLanguageIterator->end, std::min(tagIterator->end, end));
                    run.script = scriptAndLanguageIterator->data.first;
                    run.language = scriptAndLanguageIterator->data.second;
                    run.tag = tagIterator->data;
                    run.direction = directionItem.data;
                    
                    if (directionItem.data == HB_DIRECTION_LTR)
                    {
                        line.runs.push_back(run);
                    }
                    else
                    {
                        rtlInsertionPoint = line.runs.insert(rtlInsertionPoint, run);
                    }
                    
                    position = run.end;
                    
                    if (scriptAndLanguageIterator->end == position)
                    {
                        ++scriptAndLanguageIterator;
                    }
                    if (tagIterator->end == position)
                    {
                        ++tagIterator;
                    }
                }
            }
        }
        
        template <typename T>
        typename T::const_iterator TextItemizer::findItem(const T &items, int32_t position) const
        {
            for (auto it = items.begin(); it != items.end(); ++it)
            {
                if ((it->start <= position) && (it->end > position))
                {
                    return it;
                }
            }
            
            return items.end();
        }
    }
}
