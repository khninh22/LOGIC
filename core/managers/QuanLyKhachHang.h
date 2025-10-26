#ifndef QUANLYKHACHHANG_H
#define QUANLYKHACHHANG_H

#include "../entities/KhachHang.h"
#include "../utils/ThuatToan.h"
#include <string>
using namespace std;

class QuanLyKhachHang {
private:
    MangDong<KhachHang>* dsKhachHang;

public:
    // Constructor nhận con trỏ đến danh sách
    QuanLyKhachHang(MangDong<KhachHang>* ds);

    // Tìm kiếm
    KhachHang* timKhachHang(const string& maKH);
    KhachHang* timKhachHangLinhHoat(const string& thongTin);

    // CRUD operations
    bool themKhachHang(const string& ma, const string& ten, const string& sdt);
    bool suaKhachHang(const string& ma, const string& ten, const string& sdt);
    bool xoaKhachHang(const string& ma);

    // Sinh mã tự động
    string sinhMaKHTiepTheo() const;

    // Hiển thị
    void hienThiTatCa() const;
};

#endif // QUANLYKHACHHANG_H
