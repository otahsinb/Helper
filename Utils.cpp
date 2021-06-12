// Utils.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Utils.h"

std::vector<std::string> Tokenize(const std::string& strToTokenize, const char delimiter)
{
    std::vector<std::string> tokens;
    if (strToTokenize.empty()) {
        return tokens;
    }


    std::stringstream sstream(strToTokenize);
    std::string item;

    while (std::getline(sstream, item, delimiter)) {
        tokens.push_back(item);
    }
    return tokens;
}

string arrange(string _line)
{
    vector<string> _rv = Tokenize(_line, ' ');
    int _size = _rv.size(); string _r = "";
    for (int _i = 0; _i < _size; ++_i)
    {
        string _t = _rv[_i];
        if (_t != "" && _r != "") {
            _r = _r + " " + _t;
        }
        else if (_t != "" && _r == "")
        {
            _r = _t;
        }
    }
    return _r;
}

int get_index(vector<int> _v, int _y)
{
    int _size = _v.size();
    int y_prev = _v[0]; int y_next = _v[1];
    for (int _i = 1; _i < _v.size(); ++_i) {
        if (y_prev > _y) return _i - 1; /// ilk elemandan once ise.
        else if (y_prev == _y || y_next == _y) return -1;
        else if (y_prev < _y && y_next > _y) return _i; /// iki elemanin arasinda ise.
        else if (_i + 1 == _v.size()) return _i + 1; /// dizinin son elemani ise.
        else {
            y_prev = y_next;
            y_next = _v[_i + 1];
        }
    }
    return -1;
}