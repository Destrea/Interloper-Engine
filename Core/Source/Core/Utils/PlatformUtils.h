

#pragma once


#include <string>
#include "nfd.h"


namespace Core {

    class FileDialogs
    {
    public:
        static std::string OpenFile(const char* filter);
        static std::string SaveFile(const char* filter);


    };

}
