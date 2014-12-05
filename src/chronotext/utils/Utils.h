/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/FileHelper.h"
#include "chronotext/InputSource.h"
#include "chronotext/Log.h"

#include "cinder/Utilities.h"
#include "cinder/Xml.h"

#include <fstream>
#include <set>

#include <boost/algorithm/string.hpp>

namespace chr
{
    template<typename T>
    static int search(T *array, T value, int min, int max)
    {
        int mid = (min + max) >> 1;
        
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
    static inline int search(const std::vector<T> &array, float value, int min, int max)
    {
        return search((T*)array.data(), value, min, max);
    }
    
    // ---
    
    std::string wstringToUtf8(const std::wstring &s);
    std::wstring utf8ToWstring(const std::string &s);

    template<typename T> T loadString(InputSourceRef source);
    template<typename T> std::vector<T> readLines(InputSourceRef source);
    
    std::vector<std::string> readInstructions(InputSourceRef source);
    
    std::string readTextFile(const ci::fs::path &filePath);
    void writeTextFile(const ci::fs::path &filePath, const std::string &text);
    
    void writeXmlFile(const ci::fs::path &filePath, const ci::XmlTree &tree);
    
    // ---
    
    std::string hexDump(const char *data, int size);
    std::string prettyBytes(uint64_t numBytes, int precision = 2);
    std::string percent(float ratio, int precision = 1);
    
    // ---
    
    uint64_t millisSinceEpoch();
    
    // ---
    
    /*
     * RUN-TIME DIRECTIONAL-RANGE
     *
     * USAGE:
     * std::vector<string> container = {"A", "B", "C", "D"};
     * for (auto &item : DirectionalRange(container, true)) { cout << item << endl; }
     *
     * BASED ON: http://stackoverflow.com/a/14920606/50335
     * ADAPTED IN ORDER TO RETURN A REFERENCE INSTEAD OF AN ITERATOR
     */

#if !defined(CINDER_MSW)
    
    template <typename T> struct Iterator
    {
        T& container;
        bool reverse;
        
        typedef decltype(container.begin()) I;
        typedef typename std::iterator_traits<I>::reference R; // WORKS FOR CONST AND NON-CONST STL CONTAINERS
        
        struct InnerIterator
        {
            I i;
            bool reverse;
            
            InnerIterator(I i, bool reverse) : i(i), reverse(reverse) {}
            R operator*() { return *i; }
            I operator++() { return (reverse ? --i : ++i); }
            bool operator!=(const InnerIterator& o) { return i != o.i; }
        };
        
        Iterator(T& container, bool reverse) : container(container), reverse(reverse) {}
        InnerIterator begin() { return InnerIterator(reverse ? --container.end() : container.begin(), reverse); }
        InnerIterator end() { return InnerIterator(reverse ? --container.begin() : container.end(), reverse); }
    };
    
    template <typename T> Iterator<T> DirectionalRange(T& container, bool reverse = false)
    {
        return Iterator<T>(container, reverse);
    }
    
#endif
}
