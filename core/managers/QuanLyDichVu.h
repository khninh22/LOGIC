#ifndef QUANLYDICHVU_H
#define QUANLYDICHVU_H

#include "../entities/DichVu.h"
#include "../entities/DonDichVu.h"
#include "../utils/ThuatToan.h"
#include <string>
#include <ctime>
using namespace std;

/**
 * QuanLyDichVu - Manager quản lý dịch vụ và đơn dịch vụ
 */
class QuanLyDichVu {
private:
    MangDong<DichVu>* dsDichVu;
    MangDong<DonDichVu>* dsDonDichVu;

public:
    // Constructor
    QuanLyDichVu(MangDong<DichVu>* dsDV, MangDong<DonDichVu>* dsDon);

    // ==================== QUẢN LÝ DỊCH VỤ ====================
    
    // Tìm kiếm
    DichVu* timDichVu(const string& maDV);
    const DichVu* timDichVu(const string& maDV) const;
    
    // CRUD
    bool themDichVu(const string& ma, const string& ten, const string& donVi,
                    double donGia, int ton, const string& loai);
    bool suaDichVu(const string& ma, const string& ten, const string& donVi,
                   double donGia, int ton, const string& loai);
    bool xoaDichVu(const string& ma);
    bool doiTrangThaiPhucVu(const string& ma, bool phucVu);
    
    // Quản lý tồn kho
    bool nhapHang(const string& ma, int soLuong);
    bool xuatHang(const string& ma, int soLuong);
    
    // Lọc và thống kê
    MangDong<DichVu> locTheoLoai(const string& loai) const;
    MangDong<DichVu> locConPhucVu() const;
    MangDong<DichVu> locSapHetHang(int nguongTon = 10) const;
    
    // ==================== QUẢN LÝ ĐỖN DỊCH VỤ ====================
    
    // Tìm kiếm
    DonDichVu* timDonDichVu(const string& maDon);
    const DonDichVu* timDonDichVu(const string& maDon) const;
    
    // CRUD
    bool taoDonDichVu(const string& maDon, const string& maLich, 
                      const string& maKH, string& outMaDon);
    bool themDichVuVaoDon(const string& maDon, const string& maDV, int soLuong);
    bool xoaDichVuKhoiDon(const string& maDon, const string& maDV);
    bool capNhatSoLuongDon(const string& maDon, const string& maDV, int soLuongMoi);
    bool huyDon(const string& maDon);
    bool daGiaoDon(const string& maDon);
    
    // Lấy đơn theo lịch đặt
    MangDong<DonDichVu> layDonTheoLich(const string& maLich) const;
    
    // Sinh mã tự động
    string sinhMaDichVuMoi() const;
    string sinhMaDonMoi() const;
    
    // Hiển thị
    void hienThiDanhSachDichVu() const;
    void hienThiDanhSachDon() const;
    void hienThiDonTheoLich(const string& maLich) const;
};

#endif // QUANLYDICHVU_H
