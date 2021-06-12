#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

std::vector<std::string> Tokenize(const std::string& strToTokenize, const char delimiter);

string arrange(string _line);



int get_index(vector<int> _v, int _y);