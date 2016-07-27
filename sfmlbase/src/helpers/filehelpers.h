#pragma once

#include "constants.h"

#include <string>
#include <vector>

std::string DirnameOf(const std::string& fname);

bool FileRead(const std::string& file, std::vector<std::string>& out_v);

bool FileSave(const std::string& file, const std::vector<std::string>& v);

bool FileAdd(const std::string& file, std::string s);

void FileList(const std::string& s, std::vector<std::string>& out_noms);