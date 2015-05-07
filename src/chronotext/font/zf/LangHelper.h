/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * INCLUDES DATA FROM PANGO:
 * https://developer.gnome.org/pango/stable/pango-Scripts-and-Languages.html
 */

#pragma once

#include "hb.h"

#include <vector>
#include <map>
#include <set>
#include <string>

namespace chr
{
    namespace zf
    {
        class LangHelper
        {
        public:
            LangHelper();
            
            /*
             * EXPECTS A LIST OF LANGUAGES SEPARATED BY COLONS
             */
            void setDefaultLanguages(const std::string &languages);
            
            /*
             * DETERMINES THE SCRIPTS USED TO WRITE lang
             *
             * QUOTING PANGO:
             *
             * Most languages use only one script for writing, but there are
             * some that use two (Latin and Cyrillic for example), and a few
             * use three (Japanese for example).
             */
            const std::vector<hb_script_t>& getScriptsForLang(hb_language_t lang) const;
            
            /*
             * DETERMINES IF script MAY BE USED TO WRITE lang
             */
            bool includesScript(hb_language_t lang, hb_script_t script) const;
            
            /*
             * RETURNS THE RESOLVED LANGUAGE IF script MAY BE USED TO WRITE ONE OF THE "DEFAULT LANGUAGES"
             */
            hb_language_t getDefaultLanguage(hb_script_t script) const;
            
            /*
             * QUOTING PANGO:
             *
             * Given a script, finds a language tag that is reasonably
             * representative of that script. This will usually be the
             * most widely spoken or used language written in that script:
             * for instance, the sample language for %HB_SCRIPT_CYRILLIC
             * is <literal>ru</literal> (Russian), the sample language
             * for %HB_SCRIPT_ARABIC is <literal>ar</literal>.
             *
             * For some
             * scripts, no sample language will be returned because there
             * is no language that is sufficiently representative. The best
             * example of this is %HB_SCRIPT_HAN, where various different
             * variants of written Chinese, Japanese, and Korean all use
             * significantly different sets of Han characters and forms
             * of shared characters. No sample language can be provided
             * for many historical scripts as well.
             */
            hb_language_t getSampleLanguage(hb_script_t script) const;
            
            /*
             * TRYING TO DETECT A LANGUAGE FOR script BY ASKING 3 QUESTIONS:
             *
             * 1. CAN script BE USED TO WRITE langHint?
             * 2. CAN script BE USED TO WRITE ONE OF THE "DEFAULT LANGUAGES"?
             * 3. IS THERE A PREDOMINANT LANGUAGE THAT IS LIKELY FOR script?
             */
            hb_language_t detectLanguage(hb_script_t script, hb_language_t langHint = HB_LANGUAGE_INVALID) const;
            
            static hb_language_t toHBLang(const std::string &s);
            static hb_language_t toHBLang(const char *c);
            
        protected:
            std::map<hb_language_t, std::vector<hb_script_t>> scriptMap;
            std::map<hb_script_t, hb_language_t> sampleLanguageMap;
            std::set<hb_language_t> defaultLanguageSet;
        };
        
        // ---
        
        inline hb_language_t LangHelper::toHBLang(const std::string &s)
        {
            return hb_language_from_string(s.data(), s.size());
        }
        
        inline hb_language_t LangHelper::toHBLang(const char *c)
        {
            return hb_language_from_string(c, -1);
        }
    }
}
