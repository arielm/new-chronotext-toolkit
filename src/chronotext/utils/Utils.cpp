/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/utils/Utils.h"

#include "rapidxml/rapidxml_print.hpp"

#if defined(CINDER_ANDROID)
#   include "boost/date_time/local_time/local_time.hpp" // TODO IN FUTURE ANDROID REWORK: REMOVE DEPENDENCY ON boost/date_time
#else
#   include <chrono>
#endif

using namespace std;
using namespace ci;

namespace chr
{
    namespace utils
    {
        template <>
        string readText(InputSource::Ref inputSource)
        {
            /*
             * SUB-OPTIMAL (IDEALLY: TEXT SHOULD BE LOADED "DIRECTLY" INTO STRING-STORAGE
             */
            Buffer buffer(inputSource->loadDataSource());
            return string(static_cast<const char*>(buffer.getData()), buffer.getDataSize());
        }
        
        template <>
        u16string readText(InputSource::Ref inputSource)
        {
            Buffer buffer(inputSource->loadDataSource());
            const char *begin = static_cast<const char*>(buffer.getData());
            const char *end = begin + buffer.getDataSize();
            
            u16string result;
            utf8::unchecked::utf8to16(begin, end, back_inserter(result));
            return result;
        }
        
        template <>
        vector<string> readLines(InputSource::Ref inputSource)
        {
            vector<string> lines;
            IStreamRef in = inputSource->loadDataSource()->createStream();
            
            while (!in->isEof())
            {
                lines.emplace_back(in->readLine()); // TODO: CHECK IF MOVE-CONSTRUCTION IS ACTUALLY TAKING PLACE
            }
            
            return lines;
        }
        
        template <>
        vector<u16string> readLines(InputSource::Ref inputSource)
        {
            vector<u16string> lines;
            IStreamRef in = inputSource->loadDataSource()->createStream();
            
            while (!in->isEof())
            {
                lines.emplace_back(to<u16string>(in->readLine())); // TODO: CHECK IF MOVE-CONSTRUCTION IS ACTUALLY TAKING PLACE
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
#if defined(CINDER_ANDROID)
                using namespace boost::gregorian;
                using namespace boost::local_time;
                using namespace boost::posix_time;
                
                auto now = microsec_clock::local_time();
                auto diff = now - ptime(date(1970, 1, 1));
                return diff.total_milliseconds();
#else
                using namespace chrono;
                
                auto now = system_clock::now();
                auto diff = now.time_since_epoch();
                return duration_cast<milliseconds>(diff).count();
#endif
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
