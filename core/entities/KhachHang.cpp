#include "KhachHang.h"
#include <iomanip>

KhachHang::KhachHang(string ma, string ten, string sdt_val)
    : maKH(ma), hoTen(ten), sdt(sdt_val) {}

string KhachHang::getMaKH() const { return maKH; }
string KhachHang::getHoTen() const { return hoTen; }
string KhachHang::getSdt() const { return sdt; }

void KhachHang::hienThi() const {
    cout << left
         << setw(10) << maKH
         << setw(25) << hoTen
         << setw(15) << sdt
         << endl;
}
