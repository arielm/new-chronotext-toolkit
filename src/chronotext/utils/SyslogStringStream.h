/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * REDIRECTION OF std::cout TO THE OSX CONSOLE (Applications/Utilities/Console.app)
 * BEFORE OSX 10.8, THIS WAS OCCURING AUTOMATICALLY IN PARALLEL TO std::cout
 *
 * TO ACTIVATE IT: #define FORCE_SYSLOG
 * IF ACTIVATED: std::count WILL NOT BE USED IN PARALLEL
 *
 * BASED ON:
 * https://github.com/cinder/Cinder/blob/master/include/cinder/msw/OutputDebugStringStream.h
 * https://developer.apple.com/library/mac/documentation/Darwin/Reference/ManPages/man3/syslog.3.html
 */

#pragma once

#include <ostream>
#include <sstream>
#include <string>

#include <syslog.h>

namespace chr
{
    namespace mac
    {
        template <class CharT, class TraitsT = std::char_traits<CharT>>
        class basic_debugbuf : public std::basic_stringbuf<CharT, TraitsT>
        {
        public:
            virtual ~basic_debugbuf()
            {
                sync();
            }
            
        protected:
            int sync()
            {
                output_debug_string(this->str().data());
                this->str(std::basic_string<CharT>()); // CLEARS THE STRING BUFFER
                
                return 0;
            }
            
            void output_debug_string(const CharT *text) {}
        };
        
        template<>
        inline void basic_debugbuf<char>::output_debug_string(const char *text)
        {
            syslog(LOG_CONS, "%s", text); // WRITES TO THE CONSOLE
        }
        
        template<class CharT, class TraitsT = std::char_traits<CharT>>
        class basic_dostream : public std::basic_ostream<CharT, TraitsT>
        {
        public:
            basic_dostream()
            :
            std::basic_ostream<CharT, TraitsT>(new basic_debugbuf<CharT, TraitsT>())
            {}
            
            ~basic_dostream()
            {
                delete this->rdbuf();
            }
        };
        
        typedef basic_dostream<char> dostream;
    }
}
