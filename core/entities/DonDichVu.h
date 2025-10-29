#ifndef DONDICHVU_H
#define DONDICHVU_H

#include <string>
#include <ctime>
#include "../../core/utils/ThuatToan.h"
using namespace std;

/**
 * ChiTietDichVu - Chi tiết một dịch vụ trong đơn
 */
struct ChiTietDichVu {
    string maDichVu;
    string tenDichVu;
    int soLuong;
    double donGia;
    double thanhTien;  // = soLuong * donGia

    ChiTietDichVu(string ma = "", string ten = "", int sl = 0, double gia = 0.0)
        : maDichVu(ma), tenDichVu(ten), soLuong(sl), donGia(gia),
          thanhTien(sl * gia) {}
};

/**
 * DonDichVu - Đơn đặt dịch vụ của khách hàng
 * Gắn với một lịch đặt sân cụ thể
 */
class DonDichVu {
private:
    string maDon;                         // VD: DDV001
    string maLichDat;                     // Gắn với lịch đặt sân nào
    string maKhachHang;                   // Khách hàng
    time_t thoiGianDat;                   // Thời gian đặt dịch vụ
    MangDong<ChiTietDichVu> chiTiet;     // Danh sách dịch vụ
    double tongTien;                      // Tổng tiền dịch vụ
    string trangThai;                     // "Cho Xu Ly", "Da Giao", "Da Huy"

public:
    // Constructor
    DonDichVu(string maDon = "", string maLich = "", string maKH = "",
              time_t tgDat = 0);

    // Getters
    string getMaDon() const;
    string getMaLichDat() const;
    string getMaKhachHang() const;
    time_t getThoiGianDat() const;
    const MangDong<ChiTietDichVu>& getChiTiet() const;
    double getTongTien() const;
    string getTrangThai() const;

    // Setters
    void setTrangThai(const string& tt);

    // Methods
    void themDichVu(const string& maDV, const string& tenDV, int soLuong, double donGia);
    void xoaDichVu(const string& maDV);
    void capNhatSoLuong(const string& maDV, int soLuongMoi);
    void tinhLaiTongTien();

    // Display
    void hienThi() const;
};

#endif // DONDICHVU_H
