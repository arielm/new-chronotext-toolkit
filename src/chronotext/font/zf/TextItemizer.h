/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * REFERENCE: https://github.com/arielm/Unicode/tree/master/Projects/BIDI
 */

/*
 * USING ICU 52.1 FOR SCRIPT-DETECTION AND FOR BIDI:
 * http://www.icu-project.org/apiref/icu4c/ubidi_8h.html
 */

/*
 * INSPIRED BY THE MAPNIK PROJECT:
 * https://github.com/mapnik/mapnik/blob/64d5153aeaeb1c9e736bfead297dfea39b066d2c/src/text/itemizer.cpp
 *
 *
 * SOME DIFFERENCES:
 *
 * 1) WE'RE DOING "LANGUAGE DETECTION" IN ADDITION TO "SCRIPT DETECTION"
 *    NECESSARY IN ORDER TO HANDLE PROPERLY LANGUAGES SHARING THE SAME SCRIPT
 *
 * 2) WE ALLOW TO SPECIFY A DIRECTION (IN ADDITION TO THE POSSIBILITY
 *    OF DETERMINING THE PARAGRAPH-LEVEL FROM THE TEXT)
 *    NECESSARY, E.G. IN ORDER TO FORCE MIXED-TEXT STARTING WITH A "NATURALLY LTR RUN" TO BE CONSIDERED AS "OVERALL RTL"
 */

#pragma once

#include "chronotext/font/zf/LangHelper.h"
#include "chronotext/font/zf/TextLine.h"

#include "unicode/unistr.h"
#include "unicode/uscript.h"
#include "unicode/ubidi.h"

namespace chronotext
{
    namespace zf
    {
        class TextItemizer
        {
        public:
            static hb_script_t icuScriptToHB(UScriptCode script);
            static hb_direction_t icuDirectionToHB(UBiDiDirection direction);

            TextItemizer(std::shared_ptr<LangHelper> langHelper);
            void processLine(TextLine &line) const;
            
        protected:
            std::shared_ptr<LangHelper> langHelper;
            
            void itemizeScriptAndLanguage(TextLine &line) const;
            void itemizeDirection(TextLine &line) const;
            void mergeItems(TextLine &line) const;
            
            template<typename T> typename T::const_iterator findItem(const T &items, int32_t position) const;
        };
    }
}

namespace chr = chronotext;
