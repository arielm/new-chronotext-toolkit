/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/font/zf/VirtualFont.h"

class TextSpiral
{
public:
    float ox;
    float oy;
    float r1;
    float r2;
    float turns;
    
    std::vector<ci::Vec2f> vertices;
    
    void update(float ox, float oy, float r1, float r2, float turns, float DD1, float DD2);
    void drawWire();
    void drawText(chr::ZFont &font, const chr::zf::LineLayout &layout, float offsetX = 0, float offsetY = 0);
};

// ---

/*
 * RUN-TIME DIRECTIONAL-RANGE, CURRENTLY USED FOR DRAWING LTR/RTL TEXT
 *
 * USAGE:
 * std::vector<string> container = {"A", "B", "C", "D"};
 * for (auto &item : DirectionalRange(container, true)) { cout << item << endl; }
 *
 * BASED ON: http://stackoverflow.com/a/14920606/50335
 * ADAPTED IN ORDER TO RETURN A REFERENCE INSTEAD OF AN ITERATOR
 *
 * UPDATE: IN "INCUBATOR" MODE AS FOR NOW (NOT SURE IT'S THE RIGHT WAY TO GO...)
 */

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
