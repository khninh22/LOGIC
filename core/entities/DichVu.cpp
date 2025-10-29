#include "DichVu.h"
#include <iomanip>

DichVu::DichVu(string ma, string ten, string dv, double gia, int ton,
               string loai, bool phucVu)
    : maDichVu(ma), tenDichVu(ten), donVi(dv), donGia(gia),
      soLuongTon(ton), loaiDichVu(loai), conPhucVu(phucVu) {}

// Getters
string DichVu::getMaDichVu() const { return maDichVu; }
string DichVu::getTenDichVu() const { return tenDichVu; }
string DichVu::getDonVi() const { return donVi; }
double DichVu::getDonGia() const { return donGia; }
int DichVu::getSoLuongTon() const { return soLuongTon; }
string DichVu::getLoaiDichVu() const { return loaiDichVu; }
bool DichVu::getConPhucVu() const { return conPhucVu; }

// Setters
void DichVu::setTenDichVu(const string& ten) { tenDichVu = ten; }
void DichVu::setDonVi(const string& dv) { donVi = dv; }
void DichVu::setDonGia(double gia) { donGia = gia >= 0 ? gia : 0; }
void DichVu::setSoLuongTon(int ton) { soLuongTon = ton >= 0 ? ton : 0; }
void DichVu::setLoaiDichVu(const string& loai) { loaiDichVu = loai; }
void DichVu::setConPhucVu(bool phucVu) { conPhucVu = phucVu; }

// Methods
bool DichVu::tangSoLuong(int them) {
    if (them <= 0) return false;
    soLuongTon += them;
    return true;
}

bool DichVu::giamSoLuong(int giam) {
    if (giam <= 0 || giam > soLuongTon) return false;
    soLuongTon -= giam;
    return true;
}

double DichVu::tinhTien(int soLuong) const {
    return donGia * soLuong;
}

void DichVu::hienThi() const {
    cout << "Mã DV: " << maDichVu << endl;
    cout << "Tên: " << tenDichVu << endl;
    cout << "Đơn vị: " << donVi << endl;
    cout << "Đơn giá: " << fixed << setprecision(0) << donGia << " VND" << endl;
    cout << "Tồn kho: " << soLuongTon << " " << donVi << endl;
    cout << "Loại: " << loaiDichVu << endl;
    cout << "Trạng thái: " << (conPhucVu ? "Còn phục vụ" : "Ngừng phục vụ") << endl;
}
