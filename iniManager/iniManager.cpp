#include "iniManager.h"
#include <Windows.h>

std::string iniRead(std::filesystem::path directory, const char* section, const char* IniKey)
{
    std::filesystem::path iniFilePath = directory / "config.ini"; // ����� concat
    std::string iniPathString = iniFilePath.string(); // ��������� ������ � std::string
    const char* iniPathCString = iniPathString.c_str(); // ���������� c_str() ������ ����� ���������� ������ � std::string
    //`c_str()` ������������ ��� ��������� ��������� �� ����������� ������ �������� (`const char*`),
    //������� �������� �� �� ������ � ������� C-style (������������� ������� ��������).
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