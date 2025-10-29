#include "DonDichVu.h"
#include "../utils/DateTimeUtils.h"
#include <iostream>
#include <iomanip>

DonDichVu::DonDichVu(string maDon, string maLich, string maKH, time_t tgDat)
    : maDon(maDon), maLichDat(maLich), maKhachHang(maKH),
      thoiGianDat(tgDat), tongTien(0.0), trangThai("Cho Xu Ly") {}

// Getters
string DonDichVu::getMaDon() const { return maDon; }
string DonDichVu::getMaLichDat() const { return maLichDat; }
string DonDichVu::getMaKhachHang() const { return maKhachHang; }
time_t DonDichVu::getThoiGianDat() const { return thoiGianDat; }
const MangDong<ChiTietDichVu>& DonDichVu::getChiTiet() const { return chiTiet; }
double DonDichVu::getTongTien() const { return tongTien; }
string DonDichVu::getTrangThai() const { return trangThai; }

// Setters
void DonDichVu::setTrangThai(const string& tt) { trangThai = tt; }

// Methods
void DonDichVu::themDichVu(const string& maDV, const string& tenDV, int soLuong, double donGia) {
    // Kiểm tra xem dịch vụ đã tồn tại chưa
    for (int i = 0; i < chiTiet.getKichThuoc(); ++i) {
        if (chiTiet[i].maDichVu == maDV) {
            // Nếu đã có, cộng thêm số lượng
            chiTiet[i].soLuong += soLuong;
            chiTiet[i].thanhTien = chiTiet[i].soLuong * chiTiet[i].donGia;
            tinhLaiTongTien();
            return;
        }
    }
    
    // Nếu chưa có, thêm mới
    ChiTietDichVu ct(maDV, tenDV, soLuong, donGia);
    chiTiet.them(ct);
    tinhLaiTongTien();
}

void DonDichVu::xoaDichVu(const string& maDV) {
    for (int i = 0; i < chiTiet.getKichThuoc(); ++i) {
        if (chiTiet[i].maDichVu == maDV) {
            chiTiet.xoaTheoViTri(i);
            tinhLaiTongTien();
            return;
        }
    }
}

void DonDichVu::capNhatSoLuong(const string& maDV, int soLuongMoi) {
    for (int i = 0; i < chiTiet.getKichThuoc(); ++i) {
        if (chiTiet[i].maDichVu == maDV) {
            chiTiet[i].soLuong = soLuongMoi;
            chiTiet[i].thanhTien = soLuongMoi * chiTiet[i].donGia;
            tinhLaiTongTien();
            return;
        }
    }
}

void DonDichVu::tinhLaiTongTien() {
    tongTien = 0;
    for (int i = 0; i < chiTiet.getKichThuoc(); ++i) {
        tongTien += chiTiet[i].thanhTien;
    }
}

void DonDichVu::hienThi() const {
    using namespace std;
    cout << "========================================" << endl;
    cout << "Mã đơn: " << maDon << endl;
    cout << "Mã lịch đặt: " << maLichDat << endl;
    cout << "Mã KH: " << maKhachHang << endl;
    cout << "Thời gian đặt: " << DateTimeUtils::timeToString(thoiGianDat) << endl;
    cout << "Trạng thái: " << trangThai << endl;
    cout << "----------------------------------------" << endl;
    cout << "Chi tiết dịch vụ:" << endl;
    
    for (int i = 0; i < chiTiet.getKichThuoc(); ++i) {
        cout << "  - " << chiTiet[i].tenDichVu
             << " x" << chiTiet[i].soLuong
             << " = " << fixed << setprecision(0) << chiTiet[i].thanhTien << " VND" << endl;
    }
    
    cout << "----------------------------------------" << endl;
    cout << "TỔNG TIỀN: " << fixed << setprecision(0) << tongTien << " VND" << endl;
    cout << "========================================" << endl;
}
