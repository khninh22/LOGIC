#ifndef QUANLYSANBONG_H
#define QUANLYSANBONG_H

#include "../entities/SanBong.h"
#include "../utils/ThuatToan.h"
#include <string>
using namespace std;

class QuanLySanBong {
private:
    MangDong<SanBong>* dsSanBong;

public:
    // Constructor nhận con trỏ đến danh sách
    QuanLySanBong(MangDong<SanBong>* ds);

    // Tìm kiếm
    SanBong* timSanBong(const string& maSan);

    // CRUD operations
    bool themSanBong(const string& ma, const string& ten, int loai, double gia);
    bool suaSanBong(const string& ma, const string& ten, int loai, double gia);
    bool xoaSanBong(const string& ma);

    // Bảo trì
    bool batBaoTri(const string& ma);
    bool tatBaoTri(const string& ma);

    // Lấy danh sách theo loại
    MangDong<SanBong> laySanTheoLoai(int loai) const;

    // Hiển thị
    void hienThiTatCa() const;
    void hienThiDangBaoTri() const;
};

#endif // QUANLYSANBONG_H
