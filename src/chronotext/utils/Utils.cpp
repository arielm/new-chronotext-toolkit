/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
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
            /*
             * SUB-OPTIMAL (IDEALLY: TEXT SHOULD BE LOADED "DIRECTLY" INTO STRING-STORAGE
             */
            
            Buffer buffer(inputSource->loadDataSource());
            return string(static_cast<const char*>(buffer.getData()), buffer.getDataSize());
        }
        
        template <>
        wstring readText<wstring>(InputSource::Ref inputSource)
        {
            return toWideString(readText<string>(inputSource)); // TODO: CONSIDER USING MOVE-CONSTRUCTION
        }
        
        template <>
        vector<std::string> readLines<string>(InputSource::Ref inputSource)
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
        vector<wstring> readLines<wstring>(InputSource::Ref inputSource)
        {
            vector<wstring> lines;
            IStreamRef in = inputSource->loadDataSource()->createStream();
            
            while (!in->isEof())
            {
                lines.emplace_back(toWideString(in->readLine())); // TODO: CONSIDER USING MOVE-CONSTRUCTION
            }
            
            return lines;
        }
        
        vector<string> readInstructions(InputSource::Ref inputSource)
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
            fs::ifstream in(filePath, ios::in | ios::binary | ios::ate);

            auto fileSize = in.tellg();
            in.seekg(0, ios::beg);
            
            string result(fileSize, 0);
            in.read(&result[0], fileSize);
            
            return result;
        }
        
        void writeTextFile(const fs::path &filePath, const string &text)
        {
            fs::ofstream out(filePath);
            out << text;
        }
        
        XmlTree readXmlFile(const fs::path &filePath, XmlTree::ParseOptions parseOptions)
        {
            return XmlTree(readTextFile(filePath), parseOptions);
        }
        
        void writeXmlFile(const fs::path &filePath, const XmlTree &tree)
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
            const string bytes(int64_t bytes, int precision, const string &separator)
            {
                static const vector<string> abbrevs {"TB", "GB", "MB", "KB"};
                double maximum = pow(1024.0, abbrevs.size());

                double value = fabs(bytes);

                stringstream ss;
                if (bytes < 0) ss << "-";

                for (auto &abbrev : abbrevs)
                {
                    if (value > maximum)
                    {
                        ss << fixed << setprecision(precision);
                        ss << (value / maximum) << separator << abbrev;
                        return ss.str(); // TODO: CONSIDER ADAPTING TO RVO [1/2]
                    }
                    
                    maximum /= 1024;
                }
                
                ss << value << separator << "B";
                return ss.str(); // TODO: CONSIDER ADAPTING TO RVO [2/2]
            }
            
            const string duration(double seconds, int precision, const string &separator)
            {
                if (fabs(seconds) >= 60)
                {
                    return time(seconds, true); // TODO: CONSIDER ADAPTING TO RVO [1/3]
                }
                
                static const vector<string> abbrevs {"s", "ms", "μs"};
                double maximum = pow(1000.0, abbrevs.size());
                
                double value = fabs(seconds) * maximum;

                stringstream ss;
                if (value < 0) ss << "-";
                ss << fixed << setprecision(precision);
                
                for (auto &abbrev : abbrevs)
                {
                    if (value > maximum)
                    {
                        ss << (value / maximum) << separator << abbrev;
                        return ss.str(); // TODO: CONSIDER ADAPTING TO RVO [2/3]
                    }
                    
                    maximum /= 1000;
                }
                
                ss << value << separator << "ns";
                return ss.str(); // TODO: CONSIDER ADAPTING TO RVO [3/3]
            }

            const string time(double seconds, bool rounded)
            {
                int time = fabs(seconds) + (rounded ? 0.5 : 0); // ROUNDING IS NOT APPROPRIATE FOR CLOCKS, SCORES, ETC.
                int hour = time / 3600;
                
                time = time % 3600;
                int min = time / 60;
                
                time = time % 60;
                int sec = time;
                
                stringstream ss;
                if (seconds < 0) ss << "-";
                ss << setfill('0');
                
                if (hour > 0) ss << hour << ":";
                ss << setw(2) << min << ":";
                ss << setw(2) << sec;
                
                return ss.str(); // TODO: CONSIDER ADAPTING TO RVO
            }
            
            const string percent(double ratio, int precision, const string &separator)
            {
                stringstream ss;
                ss << fixed << setprecision(precision);
                ss << (fabs(ratio) * 100) << separator << "%";
                
                return ss.str(); // TODO: CONSIDER ADAPTING TO RVO
            }
        }
    }
}
