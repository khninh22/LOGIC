#include "SanBong.h"
#include <iomanip>
#include <algorithm>

// Helper function to format VND
static string fmtVND(double v) {
    long long x = (long long)(v + (v >= 0 ? 0.5 : -0.5));
    bool neg = x < 0;
    if (neg) x = -x;

    string s;
    int cnt = 0;
    do {
        s.push_back(char('0' + (x % 10)));
        x /= 10;
        if (++cnt == 3 && x > 0) { s.push_back(','); cnt = 0; }
    } while (x > 0);

    if (neg) s.push_back('-');
    reverse(s.begin(), s.end());
    return s;
}

SanBong::SanBong(string ma, string ten, int loai, double gia, bool bt)
    : maSan(ma), tenSan(ten), loaiSan(loai), giaThue(gia), baoTri(bt) {}

string SanBong::getMaSan() const { return maSan; }
string SanBong::getTenSan() const { return tenSan; }
int SanBong::getLoaiSan() const { return loaiSan; }
double SanBong::getGiaThue() const { return giaThue; }
bool SanBong::getBaoTri() const { return baoTri; }

void SanBong::setTenSan(const string& ten) { tenSan = ten; }

void SanBong::setLoaiSan(int loai) {
    if (loai == 5 || loai == 7 || loai == 11)
        loaiSan = loai;
}

void SanBong::setGiaThue(double gia) {
    if (gia >= 0)
        giaThue = gia;
}

void SanBong::setBaoTri(bool bt) {
    baoTri = bt;
}

void SanBong::hienThi() const {
    cout << left << setw(10) << maSan
         << left << setw(20) << tenSan
         << left << setw(15) << ("San " + to_string(loaiSan) + " nguoi")
         << right << setw(20) << fmtVND(giaThue) << " "
         << left << setw(15) << (baoTri ? "Dang Bao Tri" : "Hoat Dong")
         << '\n';
}
