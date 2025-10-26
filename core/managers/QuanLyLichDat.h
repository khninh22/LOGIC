#ifndef QUANLYLICHDAT_H
#define QUANLYLICHDAT_H

#include "../entities/LichDatSan.h"
#include "../entities/SanBong.h"
#include "../utils/ThuatToan.h"
#include <string>
#include <ctime>
using namespace std;

class QuanLyLichDat {
private:
    MangDong<LichDatSan>* dsLichDatSan;
    MangDong<SanBong>* dsSanBong;

public:
    // Constructor
    QuanLyLichDat(MangDong<LichDatSan>* dsLich, MangDong<SanBong>* dsSan);

    // Tìm kiếm
    LichDatSan* timLichDat(const string& maLichDat);

    // Kiểm tra trùng lịch
    bool kiemTraTrungLich(const string& maSan, time_t batDau, time_t ketThuc) const;

    // Lấy danh sách sân trống
    MangDong<SanBong> laySanTrongKhoang(int loai, time_t tBD, time_t tKT) const;

    // CRUD operations
    bool datSan(const string& maKH, const string& maSan, time_t tBD, time_t tKT,
                string& outMaLD, double& outTien);
    bool huyLichDat(const string& maLichDat);
    bool thanhToan(const string& maLichDat);

    // Sinh mã lịch mới
    string sinhMaLichMoi() const;

    // Hiển thị
    void hienThiTatCa() const;
    void hienThiTheoSan(const string& maSan) const;
    void hienThiTheoKhach(const string& maKH) const;
    void hienThiChuaThanhToan() const;
};

#endif // QUANLYLICHDAT_H
