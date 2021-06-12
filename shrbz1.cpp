// akin_calc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "Utils.h"

using namespace std;
namespace fs = std::filesystem;

#define AKIN_NODATA -9999

struct veri
{
    vector<int> yil; vector<int> ay; vector<double> deger;
    string bolge; /// dosya ismidir.
    vector<int> yillar; /// kaydederken gerekli olan yillar
    vector<double> hesaplanan; /// hesaplanan deger.

    map<int, double> yil_hesap;

    void oku();
    void kaydet(string);
    void cal();
    void init();
};

void veri::init()
{
    ///td::cout << bolge << " ... okundu." << "\n\n";// << "init cagrildi. --------------------------------\n";
    oku();
    cal();
}

/// dosya sonuna kadar tum satirlar okunarak veri yapisinda saklanir.
void veri::oku()
{
    cout << " oku() cagrildi. --------------------------------\n";
    int _index, _yil, _ay;
    double _deger;
    string _line;
    ifstream iFile(bolge.c_str());
    getline(iFile, _line);
    _line = arrange(_line);
    vector<string> _l = Tokenize(_line, '\t'); /// aradaki bosluk onemli
    string _bolge = _l[1];

    while (getline(iFile, _line)) {
        _line = arrange(_line);
        _l = Tokenize(_line, ' '); /// aradaki bosluk onemli
        _yil   = stoi(_l[0]);
        _ay    = stoi(_l[1]);
        _deger = stod(_l[2]);
        yil.push_back(_yil); ay.push_back(_ay); deger.push_back(_deger);
    }
    iFile.close();
    bolge = _bolge;
    cout << bolge << " okundu. --------------------------------\n";
}

void kaydet_yillar(vector<int> _y) {
    std::string path_string{ fs::current_path().u8string() };
    string _out_path = path_string + "\\SONUC\\sonuc.txt";

    ofstream myfile;
    myfile.open(_out_path);
    myfile << "\t";
    for (auto& _ : _y) {
        myfile << _ << "\t";
    }
    myfile << "\n";
    myfile.close();
    return;

}

void kaydet_bolgeler(vector<int> _y, veri _v) {
    std::string path_string{ fs::current_path().u8string() };
    string _out_path = path_string + "\\SONUC\\sonuc.txt";

    ofstream myfile;
    myfile.open(_out_path, std::fstream::in | std::fstream::out | std::fstream::app);
    myfile << _v.bolge << "\t";
    int _find;

    for (auto& _y1 : _y) {
        _find = 0;
        for (auto& _yh1 : _v.yil_hesap) {
            if (_y1 == _yh1.first){
                myfile << _yh1.second << "\t";
                _find = 1;
                break;
            }
        }
        if (_find == 0){
            myfile << 0 << "\t";
        }
        //myfile << "\t";
    }
    myfile << "\n";
    myfile.close();
    cout << " kaydet() cagrildi. --------------------------------\n";
    cout << _v.bolge <<" kaydedildi. --------------------------------\n";

    return;

}

void veri::kaydet(string _out_path)
{
    std::string path_string{ fs::current_path().u8string() };
    _out_path = path_string + "\\SONUC\\sonuc.txt";

    ofstream myfile;
    myfile.open(_out_path);
    
    int _size = yil.size();
    int _prev_yil, _next_yil; /// onceki ve sonraki yil degerleri
    _prev_yil = yil[0]; _next_yil = _prev_yil;
    myfile << "\t" << yil[0] << "\t";
    for (int _i = 1; _i < _size; ++_i) /// yillari ekle
    {
        _next_yil = yil[_i];
        if (_prev_yil != _next_yil) {
            myfile << yil[_i] << "\t";
            _prev_yil = _next_yil;            
        }
    }
    myfile << "\n" << bolge << "\t"; /// bolge ismini ekle

    for (int _i = 0; _i < _size; ++_i) /// hesaplanan degerleri ekle
    {
        myfile << hesaplanan[_i] << "\t";
    }
    myfile << "\n";
    myfile.close();
    return;
}

void veri::cal()
{
    //std::cout << " calc() cagrildi. --------------------------------\n";
    int _text_size = yil.size();
    
    int y_prev = yil[0]; int y_next = y_prev;
    vector<int> _tmp_year; _tmp_year.push_back(y_prev);
    double _toplam = 0; int _index = 0;
    if (deger[0] <= -1.0){
        _toplam = deger[0];
        _index = 1;
    }
    for (int _i = 1; _i <= _text_size; ++_i)
    {
        y_next = yil[_i];
        if (y_prev == y_next) {
            if (deger[_i] <= -1.0){/// ayni yila ait degerleri toplar.
                _toplam += deger[_i];
                _index++; /// yila ait varolan ay sayisini tutuyoruz.

            }
        }
        else {
            _toplam = (_toplam / 12) * _index;
            hesaplanan.push_back(_toplam);
            _tmp_year.push_back(y_prev);
            yil_hesap.insert(make_pair(y_prev, _toplam));
            _toplam = 0; _index = 0; y_prev = y_next;
            if (deger[_i] <= -1.0) {
                _toplam = deger[_i];
                _index = 1;
            }
        }
    }
    yillar = _tmp_year;
    return;
}

vector<int> com_results(vector<veri> lis)
{
    vector<int> _yil; // = lis[0].yil;
    vector<int> _fin_yil;

    /// Tum yillar burada birlestirilir.
    for (auto& _v : lis) {
        _yil.insert(_yil.end(), _v.yil.begin(), _v.yil.end());
    }
    sort(_yil.begin(), _yil.end());
    _fin_yil.push_back(_yil[0]);
    /// Tum tekrarlar burada elenir.
    for (auto& _y : _yil) { 
        if (_fin_yil.back() != _y) {
            _fin_yil.push_back(_y);
        }
    }
    return _fin_yil;
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
        std::cout <<"\n" <<"*******Merhaba!********\n";
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
            if(_txt_list.size() > 0){
                fs::create_directory("SONUC");
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

            vector<int> _kayit_yillari = com_results(_lis);

            kaydet_yillar(_kayit_yillari);

            /// _yillar icinde tum degerler var, olmayan yillar icin yil ve hesaplanan=0 degeri giremedim.
            for (auto& _v : _lis) {
                for (int _i = 0; _i < _kayit_yillari.size(); ++_i) {
                    int _index = get_index(_v.yillar, _kayit_yillari[_i]);
                    if (_index > 0) {
                            _v.yillar.insert(_v.yillar.begin() + _index, _kayit_yillari[_i]);
                            //_v.hesaplanan.insert(_v.hesaplanan.begin() + _index, 0);
                            _v.yil_hesap.insert(make_pair(_kayit_yillari[_i], 0));
                        }
                }
                kaydet_bolgeler(_kayit_yillari, _v);
            }
        }
        else
        {
            std::cout << "Gecersiz Dosya Yolu Girildi!!!\n\n";
        }
    }
}
