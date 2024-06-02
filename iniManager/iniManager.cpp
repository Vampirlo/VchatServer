#include "iniManager.h"
#include <Windows.h>

std::string iniRead(std::filesystem::path directory, const char* section, const char* IniKey)
{
    std::filesystem::path iniFilePath = directory / "config.ini"; // метод concat
    std::string iniPathString = iniFilePath.string(); // сохраняем строку в std::string
    const char* iniPathCString = iniPathString.c_str(); // используем c_str() только после сохранения строки в std::string
    //`c_str()` используется для получения указателя на константный массив символов (`const char*`),
    //который содержит ту же строку в формате C-style (завершающийся нулевым символом).
    char buffer[256];
    GetPrivateProfileStringA(section, IniKey, "", buffer, sizeof(buffer), iniPathCString);

    return std::string(buffer);
}

int iniWrite(std::filesystem::path directory, const char* section, const char* IniKey, std::string str)
{
    std::filesystem::path iniFilePath = directory / "config.ini";
    std::string iniPathString = iniFilePath.string();
    const char* iniPathCString = iniPathString.c_str();

    WritePrivateProfileStringA(section, IniKey, str.c_str(), iniPathCString);

    return 0;
}