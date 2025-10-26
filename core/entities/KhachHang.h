#ifndef KHACHHANG_H
#define KHACHHANG_H

#include <string>
#include <iostream>
using namespace std;

class KhachHang {
private:
    string maKH;
    string hoTen;
    string sdt;

public:
    // Constructors
    KhachHang(string ma = "", string ten = "", string sdt = "");

    // Getters
    string getMaKH() const;
    string getHoTen() const;
    string getSdt() const;

    // Display
    void hienThi() const;
};

#endif // KHACHHANG_H
