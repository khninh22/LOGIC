#ifndef SANBONG_H
#define SANBONG_H

#include <string>
#include <iostream>
using namespace std;

class SanBong {
private:
    string maSan;
    string tenSan;
    int loaiSan;      // 5, 7, 11
    double giaThue;   // Không dùng nữa (dùng bảng giá động)
    bool baoTri;

public:
    // Constructors
    SanBong(string ma = "", string ten = "", int loai = 0,
            double gia = 0.0, bool bt = false);

    // Getters
    string getMaSan() const;
    string getTenSan() const;
    int getLoaiSan() const;
    double getGiaThue() const;
    bool getBaoTri() const;

    // Setters
    void setTenSan(const string& ten);
    void setLoaiSan(int loai);
    void setGiaThue(double gia);
    void setBaoTri(bool bt);

    // Display
    void hienThi() const;
};

#endif // SANBONG_H
