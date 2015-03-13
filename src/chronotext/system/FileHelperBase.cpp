/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/system/FileHelperBase.h"

using namespace std;
using namespace ci;

namespace chr
{
    namespace file
    {
#if !defined(CINDER_ANDROID)
        
        fs::path getDefaultPath()
        {
            return fs::current_path();
        }
        
#endif
      
        /*
         * TODO: (RE)TEST
         */
        int64_t HelperBase::getAvailableStorage(const fs::path &folderPath)
        {
            fs::path target = folderPath.empty() ? getDefaultPath() : folderPath;
            
            if (fs::exists(target))
            {
                return fs::space(target).available;
            }
            
            return 0;
        }
        
        /*
         * PATH NORMALIZATION WITHOUT THE NEED FOR FILES TO EXIST
         * REFERENCE: http://stackoverflow.com/a/12797413/50335
         */
        fs::path HelperBase::normalizePath(const fs::path &absolutePath)
        {
            fs::path::iterator it = absolutePath.begin();
            fs::path result = *it++;
            
            for (; it != absolutePath.end(); ++it)
            {
                if (*it == "..")
                {
                    result = result.parent_path();
                }
                else if (*it != ".")
                {
                    result /= *it;
                }
            }
            
            return result;
        }
        
        /*
         * PATH RELATIVIZATION WITHOUT THE NEED FOR FILES TO EXIST
         * REFERENCE: http://stackoverflow.com/questions/10167382/boostfilesystem-get-relative-path
         */
        fs::path HelperBase::relativizePath(const fs::path &from, const fs::path &to)
        {
            fs::path a_From = fs::absolute(from);
            fs::path a_To = fs::absolute(to);
            
            fs::path ret;
            fs::path::const_iterator itrFrom(a_From.begin()), itrTo(a_To.begin());
            
            // Find common base
            for (fs::path::const_iterator toEnd(a_To.end()), fromEnd(a_From.end()); itrFrom != fromEnd && itrTo != toEnd && *itrFrom == *itrTo; ++itrFrom, ++itrTo);
            
            // Navigate backwards in directory to reach previously found base
            for (fs::path::const_iterator fromEnd(a_From.end()); itrFrom != fromEnd; ++itrFrom)
            {
                if ((*itrFrom) != ".")
                {
                    ret /= "..";
                }
            }
            
            // Now navigate down the directory branch
            for (; itrTo != a_To.end() ; ++itrTo)
            {
                ret /= *itrTo;
            }
            
            return ret;
        }
        
        bool HelperBase::arePathsRelative(const fs::path &ancestor, const fs::path &heir)
        {
            if (ancestor.empty())
            {
                return true;
            }
            else
            {
                fs::path parent = heir;
                
                while (!parent.empty())
                {
                    if (parent == ancestor)
                    {
                        return true;
                    }
                    
                    parent = parent.parent_path();
                }
                
                return false;
            }
        }
        
        fs::path HelperBase::getFolderOrCreateIt(const fs::path &folderPath)
        {
            if (!fs::exists(folderPath))
            {
                fs::create_directories(folderPath);
            }
            
            return folderPath;
        }
    }
}
