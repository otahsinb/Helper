// akin_cla.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <map>
#include "Utils.h"


using namespace std;
namespace fs = std::filesystem;

#define AKIN_NODATA -9999

struct veri
{
    vector<int> yil; vector<int> ay; vector<double> deger;
    string bolge= " "; /// dosya ismidir.
    vector<int> aralik;

    void oku();
    void kaydet_sayi();
    void cal();
    void init();
};

void veri::init()
{
    ///td::cout << bolge << " ... okundu." << "\n\n";// << "init cagrildi. --------------------------------\n";    
    oku();    
    kaydet_sayi();
}

/// dosya sonuna kadar tum satirlar okunarak veri yapisinda saklanir.
void veri::oku()
{
    cout << " oku() cagrildi. --------------------------------\n";
    aralik.clear();
    for (int _i = 0; _i < 60; ++_i) aralik.push_back(0);
    double _deger;
    string _line;
    ifstream iFile(bolge.c_str());
    getline(iFile, _line); getline(iFile, _line); getline(iFile, _line);
    getline(iFile, _line); getline(iFile, _line); getline(iFile, _line);
    string _bolge = bolge;
    int _row = 0; int _col = 0;
    while (getline(iFile, _line)) {
        int _index = 0;
        vector<string> _l = Tokenize(_line, ' '); /// aradaki bosluk onemli
        while (_index < _l.size()) {
            if (_l[_index] != "\n" &&
                stod(_l[_index]) != AKIN_NODATA &&
                stod(_l[_index]) <= 0 &&
                stod(_l[_index]) >= -6) {

                double _d = stod(_l[_index]) * (-10);
                int _r = (int) _d;
                aralik[_r]++;
            }
            _index++;
        }
        _line.clear();
    }
    iFile.close();
    vector<string> _l2 = Tokenize(_bolge, '\\');
    bolge = _l2.back();
    cout << bolge << " okundu. --------------------------------\n";
}

void kaydet() {
    std::string path_string{ fs::current_path().u8string() };
    string _out_path = path_string + "\\SONUC2\\sonuc_aralik.txt";
    vector<int> _aralik;
    for (int _i = 0; _i < 60; ++_i) {
        _aralik.push_back(_i);
    }
    ofstream myfile;
    myfile.open(_out_path);
    myfile << "\t";
    for (auto& _ : _aralik) {
        myfile <<"(-0." 
                << _ 
                <<")-(-0."
                << _ + 1
                <<")" 
                << "\t";
    }
    myfile << "\n";
    myfile.close();
    return;
}

void veri::kaydet_sayi() {
    std::string path_string{ fs::current_path().u8string() };
    string _out_path = path_string + "\\SONUC2\\sonuc_aralik.txt";
    ofstream myfile;
    myfile.open(_out_path, std::fstream::in | std::fstream::out | std::fstream::app);
    vector<string> _l = Tokenize(bolge, '.');
    myfile << _l[0] << "\t";
    int _find;
    for (auto& _y1 : aralik) {
        myfile << _y1 << "\t\t";
    }
    myfile << "\n";
    myfile.close();
    cout << " kaydet() cagrildi. --------------------------------\n";
    cout << bolge << " kaydedildi. --------------------------------\n";
    return;
}

void veri::cal()
{
    int _text_size = yil.size();
    int y_prev = yil[0]; int y_next = y_prev;
    double _toplam = 0; int _index = 0;
    if (deger[0] <= -1.0) {
        _toplam = deger[0];
        _index = 1;
    }
    for (int _i = 1; _i <= _text_size; ++_i)
    {
        y_next = yil[_i];
        if (y_prev == y_next) {
            if (deger[_i] <= -1.0) {/// ayni yila ait degerleri toplar.
                _toplam += deger[_i];
                _index++; /// yila ait varolan ay sayisini tutuyoruz.
            }
        }
        else {
            //_toplam = (_toplam / 12) * _index;
            _toplam = 0; _index = 0; y_prev = y_next;
            if (deger[_i] <= -1.0) {
                _toplam = deger[_i];
                _index = 1;
            }
        }
    }
    return;
}

bool check_exist(const fs::path& p, fs::file_status s = fs::file_status{})
{
    std::cout << p;
    if (fs::status_known(s) ? fs::exists(s) : fs::exists(p)) {
        std::cout << " exists\n\n";
        return true;
    }
    else {
        std::cout << " does not exist\n\n";
        return false;
    }
}

int main()
{
    while (true) {
        vector<veri> _lis;
        vector<string> _txt_list;
        std::cout << "\n" << "*******Merhaba!********\n";
        std::cout << "Lutfen Dosya Yolunu (klasor ismini) Giriniz!\n\n";
        std::string s;
        std::cin >> s;
        //int _i = stoi(s); 
        int _k = 1;
        if (check_exist(s)) /// girilen dosya yolunun var olup olmadigi kontrol edilir.
        {
            for (auto& p : fs::directory_iterator(s)) { /// girilen dosya yolunda bulunan txt dosyalar aranir ve okunur.
                ///std::cout << p.path() << '\n';
                std::string path_string{ p.path().u8string() };
                vector<string> _l = Tokenize(path_string, '.');
                string _ext = _l[_l.size() - 1];
                if (_ext == "txt") {
                    _txt_list.push_back(path_string);
                    std::cout << p.path() << '\n';
                }
            }
            if (_txt_list.size() > 0) {
                fs::create_directory("SONUC2");
                kaydet();
                for (auto& _t : _txt_list) {
                    veri _obj;
                    _obj.bolge = _t;
                    _k++;
                    _obj.init();
                    _lis.push_back(_obj);
                }
            }
            std::cout << "--------------------------------\n";
            std::cout << --_k << " adet txt dosyasi okundu\n";
        }
        else
        {
            std::cout << "Gecersiz Dosya Yolu Girildi!!!\n\n";
        }
    }
}

