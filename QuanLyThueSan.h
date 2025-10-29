#ifndef QUANLYTHUESAN_H
#define QUANLYTHUESAN_H

#include "core/entities/KhachHang.h"
#include "core/entities/SanBong.h"
#include "core/entities/LichDatSan.h"
#include "core/entities/DichVu.h"
#include "core/entities/DonDichVu.h"
#include "core/managers/QuanLyKhachHang.h"
#include "core/managers/QuanLySanBong.h"
#include "core/managers/QuanLyLichDat.h"
#include "core/managers/QuanLyThongKe.h"
#include "core/managers/QuanLyDichVu.h"
#include "data/DataManager.h"
#include "core/utils/ThuatToan.h"
#include <string>
using namespace std;

/**
 * QuanLyThueSan - Facade Pattern
 * Giao diện tổng hợp tất cả chức năng của hệ thống
 */
class QuanLyThueSan {
private:
    // Dữ liệu
    MangDong<SanBong> dsSanBong;
    MangDong<KhachHang> dsKhachHang;
    MangDong<LichDatSan> dsLichDatSan;
    MangDong<DichVu> dsDichVu;
    MangDong<DonDichVu> dsDonDichVu;

    // Managers
    QuanLyKhachHang* mgrKhach;
    QuanLySanBong* mgrSan;
    QuanLyLichDat* mgrLich;
    QuanLyThongKe* mgrThongKe;
    QuanLyDichVu* mgrDichVu;

public:
    // Constructor & Destructor
    QuanLyThueSan();
    ~QuanLyThueSan();

    // ==================== GETTERS ====================
    const MangDong<SanBong>& getDanhSachSan() const { return dsSanBong; }
    const MangDong<KhachHang>& getDanhSachKhach() const { return dsKhachHang; }
    const MangDong<LichDatSan>& getDanhSachLich() const { return dsLichDatSan; }
    const MangDong<DichVu>& getDanhSachDichVu() const { return dsDichVu; }
    const MangDong<DonDichVu>& getDanhSachDonDichVu() const { return dsDonDichVu; }

    // ==================== API CHO GUI ====================
    // Khách hàng
    bool themKhachHangAPI(const string& ma, const string& ten, const string& sdt);
    bool suaKhachHangAPI(const string& ma, const string& ten, const string& sdt);
    bool xoaKhachHangAPI(const string& ma);

    // Sân bóng
    bool themSanBongAPI(const string& ma, const string& ten, int loai, double gia);
    bool suaSanBongAPI(const string& ma, const string& ten, int loai, double gia);
    bool xoaSanBongAPI(const string& ma);
    bool doiBaoTriAPI(const string& ma, bool trangThai);

    // Lịch đặt
    bool datSanAPI(const string& maKH, const string& maSan, time_t tBD, time_t tKT,
                   string& outMaLD, double& outTien);
    bool huyLichDatAPI(const string& maLich);
    bool thanhToanAPI(const string& maLich);

    // Kiểm tra
    bool isTrungLich(const string& maSan, time_t tBD, time_t tKT) const;

    // ==================== API DỊCH VỤ ====================
    // Quản lý dịch vụ
    bool themDichVuAPI(const string& ma, const string& ten, const string& donVi,
                       double donGia, int ton, const string& loai);
    bool suaDichVuAPI(const string& ma, const string& ten, const string& donVi,
                      double donGia, int ton, const string& loai);
    bool xoaDichVuAPI(const string& ma);
    bool nhapHangAPI(const string& ma, int soLuong);
    
    // Quản lý đơn dịch vụ
    bool taoDonDichVuAPI(const string& maLich, const string& maKH, string& outMaDon);
    bool themDichVuVaoDonAPI(const string& maDon, const string& maDV, int soLuong);
    bool xoaDichVuKhoiDonAPI(const string& maDon, const string& maDV);
    bool huyDonDichVuAPI(const string& maDon);
    bool daGiaoDonAPI(const string& maDon);
    
    // Lấy thông tin
    MangDong<DonDichVu> layDonTheoLichAPI(const string& maLich) const;
    double tinhTongTienDonAPI(const string& maDon) const;

    // ==================== UTILS ====================
    static time_t parseDateTime(const string& s);
    static string formatDateTime(time_t t);

    // ==================== FILE I/O ====================
    void taiDuLieu();
    void luuDuLieu();

    // ✅ MỚI: Backup/Restore
    bool backup(const string& backupName);
    bool restore(const string& backupName);
};

#endif // QUANLYTHUESAN_H
