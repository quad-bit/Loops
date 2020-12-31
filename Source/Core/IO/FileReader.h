#pragma once

#include <vector>
#include <string>

#include <experimental\filesystem>
namespace fs = std::experimental::filesystem;

void GetFilesInFolder(const fs::v1::path & folderPath, std::vector<std::string> & result);
const char* ReadSpvFile(uint32_t& length, const char* filename);
const char* ReadFile(size_t& length, const char* filename);
const char* ReadTxtFile(uint32_t& length, const char* filename);

