#include "Core/Utils/PlatformUtils.h"



namespace Core {


    std::string FileDialogs::OpenFile(const char* filter)
    {
        nfdchar_t *outPath = NULL;
        nfdresult_t result = NFD_OpenDialog( filter, NULL, &outPath);
        if(result == NFD_OKAY)
        {
            //free(outPath);
            return std::string(outPath);
        }
        else
        {
            return "";
        }

    }


    std::string FileDialogs::SaveFile(const char* filter)
    {
        nfdchar_t *savePath = NULL;
        nfdresult_t result = NFD_SaveDialog("sctxt", NULL, &savePath);
        if(result == NFD_OKAY)
        {
            //free(savePath);
            return std::string(savePath);
        }
        else
        {
            return "";
        }

    }

}
