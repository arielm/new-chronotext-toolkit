/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/utils/Utils.h"

#include "rapidxml/rapidxml_print.hpp"

#include "utf8.h"

#include "boost/date_time/local_time/local_time.hpp"

using namespace std;
using namespace ci;
using namespace utf8;

using namespace boost::gregorian;
using namespace boost::local_time;
using namespace boost::posix_time;

namespace chr
{
    namespace utils
    {
        string toString(const wstring &s)
        {
            vector<char> tmp;
            
#if defined(CINDER_MSW)
            unchecked::utf16to8(s.data(), s.data() + s.size(), back_inserter(tmp));
#else
            unchecked::utf32to8(s.data(), s.data() + s.size(), back_inserter(tmp));
#endif
            
            return string(tmp.data(), tmp.size());
        }
        
        wstring toWideString(const string &s)
        {
            vector<wchar_t> tmp;
            
#if defined(CINDER_MSW)
            unchecked::utf8to16(s.data(), s.data() + s.size(), back_inserter(tmp));
#else
            unchecked::utf8to32(s.data(), s.data() + s.size(), back_inserter(tmp));
#endif
            
            return wstring(tmp.data(), tmp.size());
        }
        
        // ---
        
        template <>
        string readText<string>(InputSource::Ref inputSource)
        {
            Buffer buffer(inputSource->loadDataSource());
            return string(static_cast<const char*>(buffer.getData()), buffer.getDataSize());
        }
        
        template <>
        wstring readText<wstring>(InputSource::Ref inputSource)
        {
            return toWideString(readText<string>(inputSource));
        }
        
        template <>
        const vector<std::string> readLines<string>(InputSource::Ref inputSource)
        {
            vector<string> lines;
            IStreamRef in = inputSource->loadDataSource()->createStream();
            
            while (!in->isEof())
            {
                lines.emplace_back(in->readLine());
            }
            
            return lines;
        }
        
        template <>
        const vector<wstring> readLines<wstring>(InputSource::Ref inputSource)
        {
            vector<wstring> lines;
            IStreamRef in = inputSource->loadDataSource()->createStream();
            
            while (!in->isEof())
            {
                lines.emplace_back(toWideString(in->readLine()));
            }
            
            return lines;
        }
        
        const vector<string> readInstructions(InputSource::Ref inputSource)
        {
            vector<string> instructions;
            
            for (const auto &line : readLines<string>(inputSource))
            {
                auto trimmed = boost::algorithm::trim_copy(line);
                
                if (!trimmed.empty() && !boost::starts_with(trimmed, "#"))
                {
                    instructions.emplace_back(move(trimmed));
                }
            }
            
            return instructions;
        }
        
        string readTextFile(const fs::path &filePath)
        {
            fs::ifstream in(filePath);
            return string((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
        }
        
        void writeTextFile(const fs::path &filePath, const string &text) // TODO: (RE)TEST
        {
            fs::ofstream out(filePath);
            out << text;
        }
        
        void writeXmlFile(const fs::path &filePath, const XmlTree &tree) // TODO: (RE)TEST
        {
            fs::ofstream out(filePath);
            out << *tree.createRapidXmlDoc(true);
        }
        
        // ---
        
        namespace time
        {
            uint64_t millisSinceEpoch()
            {
                ptime time_t_epoch(date(1970, 1, 1));
                ptime now = microsec_clock::local_time();
                time_duration diff = now - time_t_epoch;
                
                return diff.total_milliseconds();
            }
        }
        
        namespace format
        {
            /*
             * REFERENCE: http://stackoverflow.com/a/10096779/50335
             */
            string bytes(int64_t value, int precision, const string &separator)
            {
                string sign = (value < 0) ? "-" : "";
                value = fabs(value);
                
                static const vector<string> abbrevs {"TB", "GB", "MB", "KB"};
                double maximum = pow(1024.0, abbrevs.size());
                
                for (auto &abbrev : abbrevs)
                {
                    if (value > maximum)
                    {
                        stringstream ss;
                        ss << fixed << setprecision(precision) << sign << (value / maximum) << separator << abbrev;
                        return ss.str();
                    }
                    
                    maximum /= 1024;
                }
                
                stringstream ss;
                ss << sign << value << separator << "B";
                return ss.str();
            }
            
            string percent(double ratio, int precision, const string &separator)
            {
                stringstream ss;
                ss << fixed << setprecision(precision) << (fabs(ratio) * 100) << separator << "%"; // FIXME: "%" IS NEVER PRINTED (REPRODUCED ON ANDROID)
                return ss.str();
            }
        }
    }
}
