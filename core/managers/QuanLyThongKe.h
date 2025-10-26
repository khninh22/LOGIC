#ifndef QUANLYTHONGKE_H
#define QUANLYTHONGKE_H

#include "../entities/LichDatSan.h"
#include "../entities/SanBong.h"
#include "../utils/ThuatToan.h"
#include <string>
#include <ctime>
#include <map>
using namespace std;

// Struct để lưu kết quả thống kê
struct ThongKeDoanhThu {
    double tongDoanhThu = 0;
    int soLichDaDat = 0;
    int soLichDaHuy = 0;
    map<string, double> doanhThuTheoSan;
};

struct ThongKeSan {
    string maSan;
    string tenSan;
    int soLanDat = 0;
    double tongGio = 0.0;
};

// Enum cho khoảng thời gian
enum KhoangThoiGian {
    TAT_CA,
    HOM_NAY,
    TUAN_NAY,
    THANG_NAY,
    QUY_NAY,
    NAM_NAY,
    TUY_CHINH
};

class QuanLyThongKe {
private:
    MangDong<LichDatSan>* dsLichDatSan;
    MangDong<SanBong>* dsSanBong;

    // Helper: Kiểm tra time_t có trong khoảng không
    bool trongKhoang(time_t t, time_t from, time_t to) const;

public:
    // Constructor
    QuanLyThongKe(MangDong<LichDatSan>* dsLich, MangDong<SanBong>* dsSan);

    // ✅ NÂNG CẤP: Thống kê theo khoảng thời gian
    ThongKeDoanhThu tinhDoanhThu(KhoangThoiGian khoang = TAT_CA,
                                 time_t from = 0, time_t to = 0) const;

    MangDong<ThongKeSan> thongKeTanSuat(KhoangThoiGian khoang = TAT_CA,
                                        time_t from = 0, time_t to = 0) const;

    // ✅ NÂNG CẤP: So sánh kỳ trước
    double tinhDoanhThuKyTruoc(KhoangThoiGian khoang) const;
    double tinhTyLeThayDoi(KhoangThoiGian khoang) const;

    // Hiển thị
    void hienThiDoanhThu(KhoangThoiGian khoang = TAT_CA) const;
    void hienThiTanSuat(KhoangThoiGian khoang = TAT_CA) const;

    // ✅ MỚI: Top sân được đặt nhiều nhất
    MangDong<ThongKeSan> topSanPhoBien(int top = 5) const;
};

#endif // QUANLYTHONGKE_H
