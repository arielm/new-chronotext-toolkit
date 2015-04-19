/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/Log.h"
#include "chronotext/InputSource.h"
#include "chronotext/system/FileHelper.h"
#include "chronotext/system/DisplayHelper.h"

#include "utf8.h"

#include "cinder/Utilities.h"
#include "cinder/Xml.h"

#include <atomic>
#include <memory>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <fstream>
#include <iomanip>

#include <boost/algorithm/string.hpp>

#if defined(CINDER_MSW)
#   define WSTRING_TO_STRING utf8::unchecked::utf16to8
#   define STRING_TO_WSTRING utf8::unchecked::utf8to16
#else
#   define WSTRING_TO_STRING utf8::unchecked::utf32to8
#   define STRING_TO_WSTRING utf8::unchecked::utf8to32
#endif

namespace chr
{
    namespace utils
    {
        template<typename TO, typename FROM>
        TO to(const FROM&);
        
        template <>
        inline std::string to(const std::wstring &in)
        {
            std::string out;
            WSTRING_TO_STRING(in.data(), in.data() + in.size(), back_inserter(out));
            return out;
        }
        
        template <>
        inline std::wstring to(const std::string &in)
        {
            std::wstring out;
            STRING_TO_WSTRING(in.data(), in.data() + in.size(), back_inserter(out));
            return out;
        }

        // ---
        
        template<typename T>
        T readText(InputSource::Ref inputSource);
        
        template<typename T>
        std::vector<T> readLines(InputSource::Ref inputSource);
        
        std::vector<std::string> readInstructions(InputSource::Ref inputSource);
        
        std::string readTextFile(const ci::fs::path &filePath);
        void writeTextFile(const ci::fs::path &filePath, const std::string &text);
        
        ci::XmlTree readXmlFile(const ci::fs::path &filePath, ci::XmlTree::ParseOptions parseOptions = ci::XmlTree::ParseOptions());
        void writeXmlFile(const ci::fs::path &filePath, const ci::XmlTree &tree);
        
        // ---
        
        template<typename T>
        size_t search(T *array, T value, size_t min, size_t max)
        {
            size_t mid = (min + max) >> 1;
            
            while (min < mid)
            {
                if (array[mid - 1] < value)
                {
                    min = mid;
                }
                else if (array[mid - 1] > value)
                {
                    max = mid;
                }
                else
                {
                    min = max = mid;
                }
                
                mid = (min + max) >> 1;
            }
            
            return mid - 1;
        }
        
        template<typename T>
        inline size_t search(const std::vector<T> &array, T value, size_t min, size_t max)
        {
            return search(const_cast<T*>(array.data()), value, min, max);
        }
        
        // ---
        
        namespace time
        {
            uint64_t millisSinceEpoch();
        }
        
        namespace format
        {
            const std::string bytes(int64_t bytes, int precision = 1, const std::string &separator = "");
            const std::string time(double seconds, bool rounded = false);
            const std::string duration(double seconds, int precision = 1, const std::string &separator = "");
            const std::string percent(double ratio, int precision = 1, const std::string &separator = "");
            
            template<typename T>
            inline const std::string write(const T &t)
            {
                std::stringstream tmp;
                tmp << t;
                return tmp.str();
            }
        }
    }
}
