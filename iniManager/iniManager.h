#pragma once

#include <filesystem>
#include <string>

std::string iniRead(std::filesystem::path directory, const char* section, const char* IniKey);
int iniWrite(std::filesystem::path directory, const char* section, const char* IniKey, std::string str);